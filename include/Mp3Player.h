#ifndef MP3PLAYER_H
#define MP3PLAYER_H

#include<SDL_stdinc.h>
#include<SDL_audio.h>

extern "C" {
#include<libavutil/frame.h>
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
}

#include<QWidget>
#include<QAudioSink>
#include<QLineEdit>

#include<QAudioOutput>
#include<QByteArray>
#include<QBuffer>

#define DEFAULT_BUFFER_SIZE 4096

// #include "SongIODevice.h"

/**
 * Handles playing and displaying the current song. Assumes that the SDL audio
 * subsystem has been initialized.
 */
class Mp3Player : public QWidget {
protected:
	Q_OBJECT
	// stuff for playing music
	SDL_AudioDeviceID audioDevice;

	// widget for song input
	QLineEdit* lineEdit;

	// for decoding with FFMPEG
	AVFormatContext* currFormatCtx;
	const AVCodec* currCodec;
	AVCodecContext* currCodecCtx;
	AVFrame* currFrame;
	AVPacket* currPacket;
	int currSample;

	bool openStream();
	void closeStream();
public:
	explicit Mp3Player(QWidget *parent = nullptr);
	virtual ~Mp3Player();

	// SDL callback for audio playback
	// userdata always has an Mp3Player
	static void audioCallback(void* userdata, Uint8* stream, int len);

public slots:
	void play();
	void pause();
signals:
};

#endif // MP3PLAYER_H
