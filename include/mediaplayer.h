#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QWidget>
#include <QLabel>
#include "playlist.h"
#include <mutex>
#include <condition_variable>


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

class QString;


/**
 * Handles playing and displaying the current song. Assumes that the SDL audio
 * subsystem has already been initialized.
 */
class MediaPlayer: public QWidget
{
    Q_OBJECT
public:
    explicit MediaPlayer(QWidget* parent=nullptr);


    /**
     * Destructor for the music player. Closes the audio stream and releases
     * any memory it's still holding.
     */
    virtual ~MediaPlayer();

    /**
     * SDL callback for filling the audio buffer.
     * @param userdata	must be an Mp3Player instance for this to work
     * @param stream	audio buffer to write to
     * @param len		length of the buffer
     */
    static void audioCallback(void* userdata, Uint8* stream, int len);

    void skip();

    void rewind();

    void setPlaylist(Playlist* playlist);



protected:
    void swapLoop();

    void generateImage(QImage* songImage);

    void updateCurrentSong(QString songPath, QString title, QString artist, QString album, QImage* songImage);

    /**
     * Handle to the audio device playing music (used with SDL). Will be zero if
     * the stream is closed, nonzero otherwise.
     */
    SDL_AudioDeviceID audioDevice;

    // equalizer to send song data to
    Equalizer* equalizer;


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

signals:
    void callBackFinished();

public slots:
    /**
     * Starts audio playback.
     */
    void play();

    /**
     * Pauses audio playback.
     */
    void pause();

private:
    QString currentSongPath;
    QLabel* currentSongTitle;
    QLabel* currentSongAlbum;
    QLabel* currentSongArtist;
    QLabel* currentImage;
    QImage* currentSongArt;
    bool isLooped = false;

    Playlist* currentPlaylist;

    bool finishedPlaying = false;
    std::mutex audioMutex;
    std::condition_variable donePlaying;
};

#endif // MEDIAPLAYER_H
