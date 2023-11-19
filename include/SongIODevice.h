#ifndef SONGIODEVICE_H
#define SONGIODEVICE_H

#include<fftw3.h>

extern "C" {
#include<libavutil/frame.h>
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
}

#include<QObject>
#include<QList>
#include<QIODevice>
#include<QFuture>

#include "equalizer.h"


/**
 * Represents an IO device for a single song. Devices of this type are
 * read-only (the program isn't allowed to write to these). It is tied
 * to a single song, which isn't changeable. This means playing a new
 * song requires a new SongIODevice.
 */
class SongIODevice : public QIODevice {
protected:
	Q_OBJECT

	Equalizer& equalizer;
	QFuture<void>* threadFuture;

	// raw waveform data storage
	qint64 readHead, writeHead;
	QByteArray* buffer;

	const std::string path;
	qint64 readPos;
	bool doneReading;

	// for decoding with FFMPEG
	AVFormatContext* currFormatCtx;
	const AVCodec* currCodec;
	AVCodecContext* currCodecCtx;
	AVFrame* currFrame;
	AVPacket* currPacket;

	// overriden QIODevice methods
	virtual qint64 readData(char* data, qint64 maxSize) override;
	virtual qint64 writeData(const char* data, qint64 maxSize) override;

	// method to run in a separate thread (should only be one)
	void readIntoBuffer();

public:
	explicit SongIODevice(const std::string& path, Equalizer& equal, qint64 bufSize,
				 QObject *parent = nullptr);
	virtual ~SongIODevice();

	// overridden QIODevice methods
	virtual bool open(QIODeviceBase::OpenMode mode) override;
	virtual void close() override;
	virtual qint64 size() const override;
	virtual bool atEnd() const override;
	virtual qint64 bytesAvailable() const override;
	virtual bool canReadLine() const override;
	virtual bool isSequential() const override { return false; }
	virtual qint64 pos() const override { return readPos; }
	virtual bool reset() override;
	virtual bool seek(qint64 pos) override;

	// standard getters and setters
	const std::string getPath() const { return path; }
};

#endif // SONGIODEVICE_H
