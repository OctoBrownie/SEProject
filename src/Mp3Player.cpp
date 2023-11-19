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

int populateBuffer(const char* file, QByteArray*& musicByteArray);


Mp3Player::Mp3Player(QWidget *parent) : QWidget{parent} {
	currByte = 0;

	currFormatCtx = nullptr;
	currCodec = nullptr;
	currCodecCtx = nullptr;
	currFrame = nullptr;
	currPacket = nullptr;
	musicBuffer = nullptr;

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
	if (musicBuffer != nullptr) delete musicBuffer;

	av_packet_free(&currPacket);
	av_frame_free(&currFrame);
	if (currCodecCtx != nullptr) avcodec_free_context(&currCodecCtx);
	if (currFormatCtx != nullptr) avformat_close_input(&currFormatCtx);
}

bool Mp3Player::initAudio() {
	// TODO: prevent calling this twice? or handle it??

	// init FFMPEG decoding things
	const char* file = this->lineEdit->text().toLocal8Bit().data();
//	const char* file = "C:\\Users\\Crystal\\Downloads\\Music\\sine.mp3";

	if(populateBuffer(file, this->musicBuffer) == -1) {
		std::cerr << "The music buffer couldn't be initialized." << std::endl;
		return false;
	}

	// init SDL audio device
	SDL_AudioSpec desired, actual;
	SDL_memset(&desired, 0, sizeof(desired));
//	desired.freq = currCodecCtx->sample_rate;
	desired.freq = 44100;
	desired.format = AUDIO_F32LSB;		// TODO: use currCodecCtx->sample_fmt?
//	desired.channels = currCodecCtx->ch_layout.nb_channels;
	desired.channels = 1;
	desired.samples = DEFAULT_BUFFER_SIZE;
	desired.callback = Mp3Player::audioCallback;
	desired.userdata = this;

	audioDevice = SDL_OpenAudioDevice(nullptr, 0, &desired, &actual, SDL_AUDIO_ALLOW_SAMPLES_CHANGE);

//	av_dump_format(currFormatCtx, 0, file, 0);

	return true;
}

void Mp3Player::play() {
	if (!audioDevice) {
		if (!initAudio()) {
			std::cerr << "Couldn't initialize the audio stream." << std::endl;
			return;
		}
	}

	SDL_PauseAudioDevice(audioDevice, false);
}

void Mp3Player::pause() {
	if (audioDevice) SDL_PauseAudioDevice(audioDevice, true);
}

int populateBuffer(const char* file, QByteArray*& musicByteArray) {
	AVFormatContext *formatCtx = nullptr;
	if (avformat_open_input(&formatCtx, file, nullptr, nullptr) != 0) {
		// couldn't open the file at all
		std::cout << "Couldn't open the file (path: \"" << file << "\")." << std::endl;
		return -1;
	}

	if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
		// can't get stream info
		return -1;
	}

	// since MP3 only supports one stream, we can just use stream[0] and take it as is
	const AVCodec* codec = avcodec_find_decoder(formatCtx->streams[0]->codecpar->codec_id);
	AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
	if (avcodec_parameters_to_context(codecCtx, formatCtx->streams[0]->codecpar) < 0) {
		// can't get an audio context
		return -1;
	}

	// stream opening time
	if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
		// couldn't open the stream
		return -1;
	}

	// buffer setup
	musicByteArray = new QByteArray();
	musicByteArray->resize(sizeof(float)*(int)ceil(formatCtx->streams[0]->duration*
													  av_q2d(formatCtx->streams[0]->time_base)*codecCtx->sample_rate));
	quint32 currPos = 0;

	// should be one frame per packet
	AVPacket* pack = av_packet_alloc();
	AVFrame* frame = av_frame_alloc();
	int retCode;

	float* currFloat;

	while (av_read_frame(formatCtx, pack) >= 0) {
		// send the packet to the decoder to be processed
		retCode = avcodec_send_packet(codecCtx, pack);

		// read all decoded frames (some file formats return multiple per packet)
		while (retCode >= 0) {
			retCode = avcodec_receive_frame(codecCtx, frame);
			for (int sample = 0; sample < frame->nb_samples; sample++) {
				// assuming floats since it's MP3
				/*
				for (int channelNum = 0; channelNum < codecCtx->ch_layout.nb_channels; channelNum++) {
					currAmplitude = *((float*)(&frame->data[channelNum][sizeof(float)*sample]));
				}
				*/

				// only doing one channel right now
				currFloat = (float*)(&frame->data[0][sizeof(float)*sample]);
				char* c = (char*)currFloat;
				(*musicByteArray)[sizeof(float)*currPos] = *c;

				++c;
				(*musicByteArray)[sizeof(float)*currPos + 1] = *c;

				++c;
				(*musicByteArray)[sizeof(float)*currPos + 2] = *c;

				++c;
				(*musicByteArray)[sizeof(float)*currPos + 3] = *c;

				++currPos;
			}


			// data[channel_num] is the data of the frame for each channel (mono/stereo things)
			// number of channels can be retrieved from codecCtx->ch_layout.nb_channels
			// bytes/sample comes from av_get_bytes_per_sample(codecCtx->sample_fmt)
			// number of samples in the frame is from frame->nb_samples
		}

		// clean packet for later use
		av_packet_unref(pack);
		// frame doesn't need to be cleaned because avcodec_receive_frame() does it
	}

	av_dump_format(formatCtx, 0, file, 0);

	av_packet_free(&pack);
	av_frame_free(&frame);
	avcodec_close(codecCtx);
	avformat_close_input(&formatCtx);
	return 0;
}

void Mp3Player::audioCallback(void* userdata, Uint8* stream, int len) {
	Mp3Player* player = (Mp3Player*) userdata;
	bool error = false;

	for (int i = 0; i < len; ++i) {
		if (player->currByte < player->musicBuffer->size()) {
			stream[i] = (*player->musicBuffer)[player->currByte];
			++player->currByte;
		}
		else stream[i] = 0;
	}
	return;






	// TODO: stream len doesn't align with num channels*float size?
	Uint8* arr = new Uint8[len];
	for (int i = 0; i < len; i++) {
		if (error) {
			arr[i] = 0;
			continue;
		}
//		if (error) stream[i] = 0;

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
//					stream[i] = 0;
					arr[i] = 0;
					error = true;
					player->pause();
					continue;
				}
				else if (res < 0) {
					std::cerr << "Error reading the next packet." << std::endl;
					// TODO: handle error?!?!
//					stream[i] = 0;
					arr[i] = 0;
					error = true;
					continue;
				}
				// send the packet to the decoder to be processed
				if (avcodec_send_packet(player->currCodecCtx, player->currPacket) < 0) {
					std::cerr << "Error sending the next packet to the decoder." << std::endl;
					// TODO: handle error?!?!
//					stream[i] = 0;
					arr[i] = 0;
					error = true;
					continue;
				}
				// alright, try again
				if (avcodec_receive_frame(player->currCodecCtx, player->currFrame) < 0) {
					// ok what went wrong this time?!
					std::cerr << "Couldn't retrieve the next frame to be played." << std::endl;
					// TODO: handle error?!?!
//					stream[i] = 0;
					arr[i] = 0;
					error = true;
					continue;
				}
			}
			else if (res == AVERROR_EOF) {
				std::cerr << "Found the end of the file." << std::endl;
//				stream[i] = 0;
				arr[i] = 0;
				error = true;
				continue;
			}
			else if (res < 0) {
				// error getting the packet
				std::cerr << "Couldn't get the next frame..." << std::endl;
				// TODO: handle error?!?! Set silence?
//				stream[i] = 0;
				arr[i] = 0;
				error = true;
				continue;
			}

		}

		// read the current sample from the current frame (all channels)
		// assuming floats since it's MP3
		for (int ch = 0; ch < player->currCodecCtx->ch_layout.nb_channels; ++ch) {
			for (unsigned long long j = 0; j < sizeof(float); j++) {
//				stream[i] = player->currFrame->data[ch][sizeof(float)*player->currSample];
				arr[i] = player->currFrame->data[ch][sizeof(float)*player->currSample + j];
				++i;
			}
		}

		--i;	// reset loop counter to the last byte written (so it works next loop)
		++player->currSample;
	}

//	for (int i = 0; i < len; i++) {
//		std::cout << std::hex << ((int) arr[i]) << ' ';
//	}
//	std::cout << std::endl;

	SDL_memset(stream, 0, len);
	SDL_MixAudio(stream, arr, len, SDL_MIX_MAXVOLUME);

	delete [] arr;

	// TODO: run stream through equalizer if no error
}
