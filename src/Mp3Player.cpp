#include<fftw3.h>
extern "C" {
	#include <libavutil/frame.h>
	#include<libavcodec/avcodec.h>
	#include<libavformat/avformat.h>
	#include<libavutil/avutil.h>
}
#include<iostream>
#include<math.h>

#include<QBuffer>
#include<QByteArray>

#include<QLabel>
#include<QPushButton>
#include<QWidget>
#include<QLineEdit>
#include<QVBoxLayout>
#include<QAudioSink>
#include<QAudioOutput>
#include<QAudioFormat>

#include "Mp3Player.h"

Mp3Player::Mp3Player(QWidget *parent) : QWidget{parent} {
	currFormatCtx = nullptr;
	currCodec = nullptr;
	currCodecCtx = nullptr;
	currFrame = nullptr;
	currPacket = nullptr;

	audioDevice = 0;
	currPacket = av_packet_alloc();
	currFrame = av_frame_alloc();
	currSample = -1;

	QVBoxLayout* layout = new QVBoxLayout(this);

	layout->addWidget(new QLabel("RAW AUDIO TEST:"));

	QLabel* label = new QLabel("NOTE: Now, this shouldn't care about length of audio. Shouldn't "
							   "create a malloc error even if you enter a normal length song...");
	label->setWordWrap(true);
	layout->addWidget(label);

	this->lineEdit = new QLineEdit("Add a real path", this);
	layout->addWidget(this->lineEdit);

	QPushButton* b = new QPushButton("Play this file!");
	b->setGeometry(10, 10, 80, 30);
	layout->addWidget(b);
	connect(b, SIGNAL (clicked()), this, SLOT (play()));

	b = new QPushButton("Pause the file??");
	b->setGeometry(10, 10, 80, 30);
	layout->addWidget(b);
	connect(b, SIGNAL (clicked()), this, SLOT (pause()));
}

Mp3Player::~Mp3Player() {
	av_packet_free(&currPacket);
	av_frame_free(&currFrame);
}

void Mp3Player::closeStream() {
	if (audioDevice > 0) {
		SDL_CloseAudioDevice(audioDevice);
		audioDevice = 0;
	}

	avcodec_free_context(&currCodecCtx);
	avformat_close_input(&currFormatCtx);
}

bool Mp3Player::openStream() {
	// TODO: prevent calling this twice? or handle it??
	closeStream();

	// init FFMPEG decoding things
	const char* file = this->lineEdit->text().toLocal8Bit().data();

	if (avformat_open_input(&currFormatCtx, file, nullptr, nullptr) != 0) {
		// couldn't open the file at all
		std::cout << "Couldn't open the file (path: \"" << file << "\")." << std::endl;
		return -1;
	}

	if (avformat_find_stream_info(currFormatCtx, nullptr) < 0) {
		// can't get stream info
		return -1;
	}

	// since MP3 only supports one stream, we can just use stream[0] and take it as is
	currCodec = avcodec_find_decoder(currFormatCtx->streams[0]->codecpar->codec_id);
	currCodecCtx = avcodec_alloc_context3(currCodec);
	if (avcodec_parameters_to_context(currCodecCtx, currFormatCtx->streams[0]->codecpar) < 0) {
		// can't get an audio context
		return -1;
	}

	// stream opening time
	if (avcodec_open2(currCodecCtx, currCodec, nullptr) < 0) {
		// couldn't open the stream
		return -1;
	}

	// init SDL audio device
	SDL_AudioSpec desired, actual;
	SDL_memset(&desired, 0, sizeof(desired));
	desired.freq = currCodecCtx->sample_rate;
	desired.format = AUDIO_F32LSB;		// TODO: use currCodecCtx->sample_fmt?
	desired.channels = currCodecCtx->ch_layout.nb_channels;
	desired.samples = DEFAULT_BUFFER_SIZE;
	desired.callback = Mp3Player::audioCallback;
	desired.userdata = this;

	audioDevice = SDL_OpenAudioDevice(nullptr, 0, &desired, &actual, SDL_AUDIO_ALLOW_SAMPLES_CHANGE);

	// av_dump_format(currFormatCtx, 0, file, 0);

	return true;
}

void Mp3Player::play() {
	if (!audioDevice) {
		if (!openStream()) {
			std::cerr << "Couldn't initialize the audio stream." << std::endl;
			return;
		}
	}

	SDL_PauseAudioDevice(audioDevice, false);
}

void Mp3Player::pause() {
	if (audioDevice) SDL_PauseAudioDevice(audioDevice, true);
}

void Mp3Player::audioCallback(void* userdata, Uint8* stream, int len) {
	Mp3Player* player = (Mp3Player*) userdata;
	bool error = false;

	// TODO: stream len doesn't align with num channels*float size?
	for (int i = 0; i < len; i++) {
		if (error) {
			stream[i] = 0;
			continue;
		}

		if (player->currSample < 0 || player->currSample >= player->currFrame->nb_samples) {
			// need new frame, could have a frame ready from the decoder (apparently,
			// some formats return multiple frames per packet)
			int res = avcodec_receive_frame(player->currCodecCtx, player->currFrame);
			player->currSample = 0;

			if (res == AVERROR(EAGAIN)) {
				// just needs a new packet

				// clean packet for use
				av_packet_unref(player->currPacket);
				// frame doesn't need to be cleaned because avcodec_receive_frame() does it

				res = av_read_frame(player->currFormatCtx, player->currPacket);
				if (res == AVERROR_EOF) {
					std::cerr << "Found the end of the file." << std::endl;
					stream[i] = 0;
					error = true;
					player->pause();
					continue;
				}
				else if (res < 0) {
					std::cerr << "Error reading the next packet." << std::endl;
					// TODO: handle error?!?!
					stream[i] = 0;
					error = true;
					continue;
				}
				// send the packet to the decoder to be processed
				if (avcodec_send_packet(player->currCodecCtx, player->currPacket) < 0) {
					std::cerr << "Error sending the next packet to the decoder." << std::endl;
					// TODO: handle error?!?!
					stream[i] = 0;
					error = true;
					continue;
				}
				// alright, try again
				if (avcodec_receive_frame(player->currCodecCtx, player->currFrame) < 0) {
					// ok what went wrong this time?!
					std::cerr << "Couldn't retrieve the next frame to be played." << std::endl;
					// TODO: handle error?!?!
					stream[i] = 0;
					error = true;
					continue;
				}
			}
			else if (res == AVERROR_EOF) {
				std::cerr << "Found the end of the file." << std::endl;
				stream[i] = 0;
				error = true;
				continue;
			}
			else if (res < 0) {
				// error getting the packet
				std::cerr << "Couldn't get the next frame..." << std::endl;
				// TODO: handle error?!?! Set silence?
				stream[i] = 0;
				error = true;
				continue;
			}

		}

		// read the current sample from the current frame (all channels)
		// assuming floats since it's MP3
		for (int ch = 0; ch < player->currCodecCtx->ch_layout.nb_channels; ++ch) {
			for (unsigned long long j = 0; j < sizeof(float); j++) {
				stream[i] = player->currFrame->data[ch][sizeof(float)*player->currSample + j];
				++i;
			}
		}

		--i;	// reset loop counter to the last byte written (so it works next loop)
		++player->currSample;
	}

	// TODO: run stream through equalizer if no error
}
