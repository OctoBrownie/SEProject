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
class Equalizer;
class Song;


/**
 * Handles playing and displaying the current song. Assumes that the SDL audio
 * subsystem has already been initialized.
 */
class MusicPlayer : public QWidget {
protected:
	Q_OBJECT
	/**
	 * Handle to the audio device playing music (used with SDL). Will be zero if
	 * the stream is closed, nonzero otherwise.
	 */
	SDL_AudioDeviceID audioDevice;

	// equalizer to send song data to
	Equalizer* equalizer;

	// current song being played (null if nothing is being played)
	Song* currSong;

	// line edit widget representing which song the user wants to play
	QLineEdit* lineEdit;

	// ***********************  FFMPEG things  ************************

	// the current format context corresponding to the current song
	AVFormatContext* currFormatCtx;

	// current codec being used
	const AVCodec* currCodec;

	// context for the codec (where you send packets to decode)
	AVCodecContext* currCodecCtx;

	// cached frame of audio data
	AVFrame* currFrame;

	// cached packet of audio data
	AVPacket* currPacket;

	// current sample to read from in the current frame
	int currSample;

	/**
	 * Opens the stream with the current set song.
	 * @return true on success, false otherwise
	 */
	bool openStream();

	/**
	 * Closes the audio stream and frees data related to decoding.
	 */
	void closeStream();

public:
	/**
	 * Makes a new music player with the given parent object.
	 * @param parent	the new parent of the music player
	 */
	explicit MusicPlayer(QWidget *parent = nullptr);

	/**
	 * Destructor for the music player. Closes the audio stream and releases
	 * any memory it's still holding.
	 */
	virtual ~MusicPlayer();

	/**
	 * SDL callback for filling the audio buffer.
	 * @param userdata	must be an Mp3Player instance for this to work
	 * @param stream	audio buffer to write to
	 * @param len		length of the buffer
	 */
	static void audioCallback(void* userdata, Uint8* stream, int len);

public slots:
	/**
	 * Starts audio playback.
	 */
	void play();

	/**
	 * Pauses audio playback.
	 */
	void pause();
signals:
};

#endif // MUSICPLAYER_H
