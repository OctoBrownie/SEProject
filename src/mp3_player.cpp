#include<fftw3.h>
extern "C" {
	#include <libavutil/frame.h>
	#include<libavcodec/avcodec.h>
	#include<libavformat/avformat.h>
	#include<libavutil/avutil.h>
}
#include<iostream>

#include<QBuffer>
#include<QByteArray>

#include "mp3_player.h"

Mp3_Player::Mp3_Player() {
	fftw_complex *in, *out;
	// fftw_plan p;
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*10);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*10);

	fftw_free(in);
	fftw_free(out);

	AVFormatContext *formatCtx = nullptr;

	/* ********************************  ADD TEST FILE HERE  ********************************** */
	const char* file = "C:\\Users\\Crystal\\Downloads\\square.mp3";

	if (avformat_open_input(&formatCtx, file, nullptr, nullptr) != 0) {
		// couldn't open the file at all
		std::cout << "Couldn't open the file (path: \"" << file << "\"). Check mp3_player.cpp?" << std::endl;
		return;
	}

	if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
		// can't get stream info
		return;
	}

	// since MP3 only supports one stream, we can just use stream[0] and take it as is
	const AVCodec* codec = avcodec_find_decoder(formatCtx->streams[0]->codecpar->codec_id);
	AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
	if (avcodec_parameters_to_context(codecCtx, formatCtx->streams[0]->codecpar) < 0) {
		// can't get an audio context
		return;
	}

	// stream opening time
	if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
		// couldn't open the stream
		return;
	}

	// should be one frame per packet
	AVPacket* pack = av_packet_alloc();
	AVFrame* frame = av_frame_alloc();
	int retCode;

	std::cout << "sample format: " << av_get_sample_fmt_name(codecCtx->sample_fmt) << std::endl;

	std::cout << "\nRaw waveform amplitude data (if stereo, interleaves the two channels):\n";
	while (av_read_frame(formatCtx, pack) >= 0) {
		// send the packet to the decoder to be processed
		retCode = avcodec_send_packet(codecCtx, pack);

		// read all decoded frames (some file formats return multiple per packet)
		float currAmplitude;
		while (retCode >= 0) {
			retCode = avcodec_receive_frame(codecCtx, frame);
			for (int sample = 0; sample < frame->nb_samples; sample++) {
				// assuming floats since it's MP3
				for (int channelNum = 0; channelNum < codecCtx->ch_layout.nb_channels; channelNum++) {
					currAmplitude = *((float*)(&frame->data[channelNum][sizeof(float)*sample]));
					std::cout << currAmplitude << ' ';
				}
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
	std::cout << std::endl;

	av_dump_format(formatCtx, 0, file, 0);

	av_packet_free(&pack);
	av_frame_free(&frame);
	avcodec_close(codecCtx);
	avformat_close_input(&formatCtx);
}
