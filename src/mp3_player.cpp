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

#include "mp3_player.h"

void Mp3_Player::printSinkError() {
	std::cout << "Current audio sink status: ";
	switch (aSink->error()) {
	case QAudio::NoError:
		std::cout << "no error";
		break;
	case QAudio::OpenError:
		std::cout << "error opening device";
		break;
	case QAudio::IOError:
		std::cout << "IO error";
		break;
	case QAudio::UnderrunError:
		std::cout << "Not enough sampling";
		break;
	case QAudio::FatalError:
		std::cout << "Not recoverable...?";
		break;
	}
	std::cout << std::endl;
}

Mp3_Player::Mp3_Player(QWidget *parent) : QWidget{parent} {
	// checking FFTW works

	// fftw_complex *in, *out;
	// fftw_plan p;
	// in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*10);
	// out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*10);

	// fftw_free(in);
	// fftw_free(out);

	musicBuffer = nullptr;
	musicByteArray = nullptr;

	QVBoxLayout* layout = new QVBoxLayout(this);

	layout->addWidget(new QLabel("NEXT TEST:"));
	layout->addWidget(new QLabel("NOTE: This should be a short audio file, since it buffers ALL of it before playing..."));

	this->lineEdit = new QLineEdit("Add a real path", this);
	layout->addWidget(this->lineEdit);

	QPushButton* b = new QPushButton("Play this file!");
	b->setGeometry(10, 10, 80, 30);
	layout->addWidget(b);
	connect(b, SIGNAL (clicked()), this, SLOT (startMusic()));

	b = new QPushButton("Sink error??");
	b->setGeometry(10, 10, 80, 30);
	layout->addWidget(b);
	connect(b, SIGNAL (clicked()), this, SLOT (printSinkError()));

	// music player setup
	QAudioFormat aFormat;
	aFormat.setSampleRate(44100);
	aFormat.setChannelCount(1);
	aFormat.setChannelConfig(QAudioFormat::ChannelConfigMono);
	aFormat.setSampleFormat(QAudioFormat::Float);

	this->aSink = new QAudioSink(aFormat);
}

Mp3_Player::~Mp3_Player() {
	delete this->aSink;
	delete this->musicBuffer;
	delete this->musicByteArray;
}

int Mp3_Player::populateBuffer() {
	if (this->musicBuffer != nullptr) {
		delete this->musicBuffer;
		delete this->musicByteArray;
	}

	const char* file = this->lineEdit->text().toLocal8Bit().data();

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
	musicBuffer = new QBuffer(musicByteArray);
	musicBuffer->open(QIODevice::ReadOnly);

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

	// av_dump_format(formatCtx, 0, file, 0);

	av_packet_free(&pack);
	av_frame_free(&frame);
	avcodec_close(codecCtx);
	avformat_close_input(&formatCtx);
	return 0;
}

void Mp3_Player::startMusic() {
	populateBuffer();
	aSink->setVolume(1);
	aSink->start(musicBuffer);
}
