#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include<SDL_stdinc.h>
#include<SDL_audio.h>

// FFMPEG
struct AVFormatContext;	// <libavformat/avformat.h>
struct AVCodec;			// <libavcodec/avcodec.h>
struct AVCodecContext;	// <libavcodec/avcodec.h>
struct AVFrame;			// <libavutil/avcodec.h>
struct AVPacket;		// <libavutil/avcodec.h>

// Qt
#include<QWidget>
class QLineEdit;

// project
class Equalizer;	// "equalizer.h"


#define DEFAULT_BUFFER_SIZE 4096

/**
 * Handles playing and displaying the current song. Assumes that the SDL audio
 * subsystem has been initialized.
 */
class MusicPlayer : public QWidget {
protected:
	Q_OBJECT
	// for playing music with SDL
	SDL_AudioDeviceID audioDevice;

	// for equalizing stuff
	Equalizer* equalizer;

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
	explicit MusicPlayer(QWidget *parent = nullptr);
	virtual ~MusicPlayer();

	// SDL callback for audio playback
	// userdata always has an Mp3Player
	static void audioCallback(void* userdata, Uint8* stream, int len);

public slots:
	void play();
	void pause();
signals:
};

#endif // MUSICPLAYER_H
