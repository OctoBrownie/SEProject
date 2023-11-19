extern "C" {
	#include<libavformat/avformat.h>
	#include<libavutil/avutil.h>
}
#include<iostream>
#include<SDL_audio.h>

#include "SongIODevice.h"

/**
 * @brief Creates a new SongIODevice with the given parameters.
 * @param path		path of the song to play
 * @param equal		equalizer to use for this song
 * @param bufSize	size of the audio buffer
 * @param parent	parent QObject
 */
SongIODevice::SongIODevice(const std::string& path, Equalizer& equal, qsizetype bufSize,
						   QObject *parent) : QIODevice{parent}, equalizer(equal), path(path) {
	buffer = new QByteArray();
	buffer->resize(bufSize);
	readHead = 0;
	writeHead = 0;
	doneReading = false;
	threadFuture = nullptr;
}

/**
 * @brief Destructor for SongIODevice.
 */
SongIODevice::~SongIODevice() {
	delete buffer;
}

/**
 * @brief Reads data from the internal audio buffer into the given array.
 * @param data		array to read bytes into
 * @param maxSize	maximum number of bytes to read
 * @return actual bytes read into data
 */
qint64 SongIODevice::readData(char* data, qint64 maxSize) {
	qint64 bytesRead = 0;
	while (bytesRead <= maxSize && readHead != writeHead) {
		data[bytesRead] = (*buffer)[readHead];
		readHead = (readHead + 1) % buffer->size();
		// TODO: Update readPos too
		++bytesRead;
	}

	return bytesRead;
}

/**
 * @brief Writing to this device is not supported.
 * @param data		unused parameter
 * @param maxSize	unused parameter
 * @return -1 because the operation always fails
 */
qint64 SongIODevice::writeData(const char* data, qint64 maxSize) {
	return -1;
}

/**
 * @brief Opens the song device and initializes streams for reading.
 * Always fails if the client passes in QIODeviceBase::WriteOnly or
 * QIODeviceBase::ReadWrite because those modes are not supported.
 * Opens streams and starts decoding audio immediately.
 * @param mode	the mode to open the IODevice in
 * @return true on open success, false otherwise
 */
bool SongIODevice::open(QIODeviceBase::OpenMode mode) {
	if (mode == QIODeviceBase::WriteOnly || mode == QIODeviceBase::ReadWrite) {
		std::cerr << "Writing to a SongIODevice is not supported." << std::endl;
		return false;
	}
	return QIODevice::open(mode);

	currFormatCtx = nullptr;
	if (avformat_open_input(&currFormatCtx, path.c_str(), nullptr, nullptr) != 0) {
		std::cerr << "Couldn't open \"" << path << "\" so that's fun." << std::endl;
		return false;
	}

	if (avformat_find_stream_info(currFormatCtx, nullptr) < 0) {
		std::cerr << "Couldn't get stream info for \"" << path << "\" so uhh..." << std::endl;
		avformat_close_input(&currFormatCtx);
		return false;
	}

	// since MP3 only supports one stream, we can just use stream[0]
	currCodec = avcodec_find_decoder(currFormatCtx->streams[0]->codecpar->codec_id);
	if (!currCodec) {
		std::cerr << "Codec not supported!" << std::endl;
		return false;
	}

	currCodecCtx = avcodec_alloc_context3(currCodec);
	if (avcodec_parameters_to_context(currCodecCtx, currFormatCtx->streams[0]->codecpar) < 0) {
		std::cerr << "Couldn't get an audio context..." << std::endl;
		return false;
	}

	// stream opening time
	if (avcodec_open2(currCodecCtx, currCodec, nullptr) < 0) {
		std::cerr << "Couldn't open a decoding stream..." << std::endl;
		return false;
	}

	// should be one frame per packet
	currPacket = av_packet_alloc();
	currFrame = av_frame_alloc();

	// TODO: create thread or something in the background decoding continuously
	// until either it finishes the data OR it reaches the end of the buffer and
	// suspends itself (use a condition variable?)

	return true;
}

/**
 * @brief Closes the device.
 */
void SongIODevice::close() {
	QIODevice::close();

	av_packet_free(&currPacket);
	av_frame_free(&currFrame);
	if (currCodecCtx != nullptr) avcodec_free_context(&currCodecCtx);
	if (currFormatCtx != nullptr) avformat_close_input(&currFormatCtx);
}

/**
 * @brief Reimplements QIODevice::size() to return the number of bytes in the file.
 */
qint64 SongIODevice::size() const {
	return sizeof(float)*(int)ceil(currFormatCtx->streams[0]->duration*
			av_q2d(currFormatCtx->streams[0]->time_base)*currCodecCtx->sample_rate);
}

/**
 * @brief Checks whether we're at the end of the song.
 */
bool SongIODevice::atEnd() const {
	return doneReading && readHead == writeHead;
}

/**
 * @brief Returns the number of bytes available for reading.
 */
qint64 SongIODevice::bytesAvailable() const {
	// add size to ensure a nonnegative number without
	// affecting output
	return (writeHead - readHead + buffer->size()) % buffer->size();
}

/**
 * @brief Checks if we can read a line (overrides QIODevice::canReadLine()).
 * There is no concept of lines in a music file, so always returns false.
 */
bool SongIODevice::canReadLine() const { return false; }

/**
 * @brief Seeks to the start of the file.
 * @return true on sucess, false otherwise
 */
bool SongIODevice::reset() {
	// TODO: implement reset
	return false;
}

/**
 * @brief Seeks to the given position in the file.
 * @param pos	the position to go to
 * @return true on success, false otherwise
 */
bool SongIODevice::seek(qint64 pos) {
	// TODO: implement seek
	return false;
}

/**
 * @brief Reads as many bytes as possible into the buffer.
 */
void SongIODevice::readIntoBuffer() {
	int retCode;
	float* currFloat;
	while (av_read_frame(currFormatCtx, currPacket) >= 0) {
		// send the packet to the decoder to be processed
		retCode = avcodec_send_packet(currCodecCtx, currPacket);

		// read all decoded frames (some file formats return multiple per packet)
		while (retCode >= 0) {
			retCode = avcodec_receive_frame(currCodecCtx, currFrame);
			for (int sample = 0; sample < currFrame->nb_samples; sample++) {
				// assuming floats since it's MP3
				/*
				for (int channelNum = 0; channelNum < codecCtx->ch_layout.nb_channels; channelNum++) {
					currAmplitude = *((float*)(&frame->data[channelNum][sizeof(float)*sample]));
				}
				*/

				// only doing one channel right now
				currFloat = (float*)(&currFrame->data[0][sizeof(float)*sample]);
				char* c = (char*)currFloat;
//				(*musicByteArray)[sizeof(float)*currPos] = *c;

//				++c;
//				(*musicByteArray)[sizeof(float)*currPos + 1] = *c;

//				++c;
//				(*musicByteArray)[sizeof(float)*currPos + 2] = *c;

//				++c;
//				(*musicByteArray)[sizeof(float)*currPos + 3] = *c;

//				++currPos;
			}


			// data[channel_num] is the data of the frame for each channel (mono/stereo things)
			// number of channels can be retrieved from codecCtx->ch_layout.nb_channels
			// bytes/sample comes from av_get_bytes_per_sample(codecCtx->sample_fmt)
			// number of samples in the frame is from frame->nb_samples
		}

		// clean packet for later use
		av_packet_unref(currPacket);
		// frame doesn't need to be cleaned because avcodec_receive_frame() does it
	}

	doneReading = true;

	// av_dump_format(formatCtx, 0, file, 0);
}

