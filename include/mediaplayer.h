#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include<SDL_stdinc.h>
#include<SDL_audio.h>
#include<QPushButton>

// FFMPEG
struct AVFormatContext;	// <libavformat/avformat.h>
struct AVCodec;			// <libavcodec/avcodec.h>
struct AVCodecContext;	// <libavcodec/avcodec.h>
struct AVFrame;			// <libavutil/avcodec.h>
struct AVPacket;		// <libavutil/avcodec.h>

// Qt

#include<QWidget>


//Class Definitions
class Song;
class Playlist;
class QString;
class QLineEdit;
class QLabel;
class EqualizerWindow;


/**
 * Handles playing and displaying the current song. Assumes that the SDL audio
 * subsystem has already been initialized.
 */
class MediaPlayer: public QWidget
{
    Q_OBJECT
public:
    explicit MediaPlayer(Playlist* playlist, EqualizerWindow* eqWindow);

    //Determines if the playlist is looped or not
    bool isLooped = false;


    void setEqualizer(EqualizerWindow* eqWindow);
    EqualizerWindow* getEqualizer();


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

    //Skips to the next song
    void skip();

    //Goes back to the previous song
    void rewind();

    //Set the playlist the player reads and takes data from
    void setPlaylist(Playlist* playlist);

    //Public facing method for the closeStream function; only needed when importing a new playlist, which is not handled by this class
    void killStream();

    bool getShuffled();



protected:


    //Create the PixMap image for the current song being played, so it is displayed in the player
    void generateImage(QImage* songImage);

    //Update the current song
    void updateCurrentSong(QString songPath, QString title, QString artist, QString album, QImage* songImage);

    /**
     * Handle to the audio device playing music (used with SDL). Will be zero if
     * the stream is closed, nonzero otherwise.
     */
	SDL_AudioDeviceID audioDevice;


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





//When the file has been processed, via the use of the playlist function, this signal is emitted. Is done to control asynchronous actions, and to allow the song to skip to the next song when it is done playing.
signals:
    void callBackFinished();


//Slots that can receive signals from signals (not necessary, signals can properly work with any functions in modern Qt).
public slots:
    /**
     * Starts audio playback.
     */
    void play();

    void swapLoop();
    void updateLoopButtonStyle();

    void swapRandom();
    void updateRandomButtonStyle();


    /**
     * Pauses audio playback.
     */
    void pause();

private:
    //Currently playing song file path
    QString currentSongPath;

    //Current title of song
    QLabel* currentSongTitle;

    //Current artist of song
    QLabel* currentSongArtist;

    //Current album of song
    QLabel* currentSongAlbum;

    //The element containing the displaed image.
    QLabel* currentImage;

    //The Qt object that contains the actual image data.
    QImage* currentSongArt;



    //Determines if the playlist is random or not.
    bool isRandom = false;

    //Currently opened and playing playlist.
    Playlist* currentPlaylist;

    Playlist* shuffledPlaylist;  // New playlist to hold shuffled order


    QPushButton* randomButton;
    QPushButton* loopbutton;
    QPushButton* playbutton;
    QPushButton* pausebutton;

    EqualizerWindow* eqWindow;

    QVector<qint64> playOrder;


};

#endif // MEDIAPLAYER_H
