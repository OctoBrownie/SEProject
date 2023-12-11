#include <algorithm>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QFrame>
#include<QPushButton>
#include<QLabel>
#include<QString>
extern "C" {
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libavutil/avutil.h>
}
#include <QDebug>

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

#include <QWidget>
#include <QLabel>

#include "equalizerwindow.h"
#include "playlist.h"
#include "mediaplayer.h"
#include "equalizer.h"

#define DEFAULT_BUFFER_SIZE 4096


//Media Player class, inherits from QWidget
MediaPlayer::MediaPlayer(Playlist* playlist, EqualizerWindow* eqWindow): QWidget()
{
    //Media processing variables
    this->currFormatCtx = nullptr;
    this->currCodec = nullptr;
    this->currCodecCtx = nullptr;
    this->currFrame = nullptr;
    this->currPacket = nullptr;

    this->audioDevice = 0;
    this->currPacket = av_packet_alloc();
    this->currFrame = av_frame_alloc();
    this->currSample = -1;

    //Create a layout for the MP3 player
    QVBoxLayout* playerLayout = new QVBoxLayout();
    playerLayout->setAlignment(Qt::AlignHCenter);

    //Create layouts for the buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();

    //Create layout for the data from the song
    QVBoxLayout* songDataLayout = new QVBoxLayout();

    //Set generic titles and images for the default "loaded song"
    this->currentSongTitle = new QLabel("Title");
    this->currentSongArtist = new QLabel("Artist");
    this->currentSongAlbum = new QLabel("Album");
    this->currentImage = new QLabel();
    this->currentSongArt = new QImage();
    //Create the image, which sets this->currentImage
    this->generateImage(this->currentSongArt);

    this->setPlaylist(playlist);

    this->eqWindow = eqWindow;

    //Add the newly created data to the song data section
    songDataLayout->addWidget(this->currentImage);
    songDataLayout->addWidget(this->currentSongTitle);
    songDataLayout->addWidget(this->currentSongArtist);
    songDataLayout->addWidget(this->currentSongAlbum);
    songDataLayout->setAlignment(Qt::AlignHCenter);

    //Create a frame, and display the proper data
    QFrame* dataDisplay = new QFrame();
    dataDisplay->setFixedSize(600, 600);
    dataDisplay->setFrameStyle(QFrame::Box);
    dataDisplay->setStyleSheet("background-color: white");

    dataDisplay->setLayout(songDataLayout);

    //Create all of the buttons, and add them to one widget
    QWidget* buttons = new QWidget();
    this->playbutton = new QPushButton();
    this->pausebutton = new QPushButton();
    QPushButton* skipbutton = new QPushButton();
    QPushButton* backbutton = new QPushButton();
    this->loopbutton = new QPushButton();
    this->randomButton = new QPushButton();
    QPushButton* eqbutton = new QPushButton();

    const QIcon playIcon = QIcon(":/resources/icons/play.svg");
    const QIcon pauseIcon = QIcon(":/resources/icons/pause.svg");
    const QIcon skipIcon= QIcon(":/resources/icons/skip.svg");
    const QIcon backIcon = QIcon(":/resources/icons/rewind.svg");
    const QIcon eqIcon = QIcon(":/resources/icons/equalizer.svg");
    const QIcon loopIcon = QIcon(":/resources/icons/loop.svg");
    const QIcon randomIcon = QIcon(":/resources/icons/randomize.svg");

    this->playbutton->setIcon(playIcon);
    this->pausebutton->setIcon(pauseIcon);
    skipbutton->setIcon(skipIcon);
    backbutton->setIcon(backIcon);
    eqbutton->setIcon(eqIcon);
    this->loopbutton->setIcon(loopIcon);
    this->randomButton->setIcon(randomIcon);

    this->playbutton->setCheckable(true);
    this->pausebutton->setCheckable(true);


    buttonLayout->addWidget(this->loopbutton);
    buttonLayout->addWidget(this->randomButton);
    buttonLayout->addWidget(backbutton);
    buttonLayout->addWidget(this->playbutton);
    buttonLayout->addWidget(this->pausebutton);
    buttonLayout->addWidget(skipbutton);
    buttonLayout->addWidget(eqbutton);

    //Ties the buttons so that when they are clicked, they perform a function, using the connect API
    connect(skipbutton, &QPushButton::clicked, this, &MediaPlayer::skip);
    connect(backbutton, &QPushButton::clicked, this, &MediaPlayer::rewind);
    connect(playbutton, SIGNAL (clicked()), this, SLOT (play()));
    connect(pausebutton, SIGNAL (clicked()), this, SLOT (pause()));
    connect(loopbutton, SIGNAL(clicked()), this, SLOT(swapLoop()));
    connect(randomButton, SIGNAL(clicked()), this, SLOT(swapRandom()));
    connect(eqbutton, &QPushButton::clicked, this->eqWindow, &EqualizerWindow::show);
    buttons->setLayout(buttonLayout);

/*
    QVBoxLayout* masterVolumeLayout = new QVBoxLayout();
    QLabel* volumeName = new QLabel("Volume");
    QFont vFont;
    vFont.setPointSize(30);
    volumeName->setFont(vFont);
    QSlider* volumeSlider = new QSlider(Qt::Horizontal);
    masterVolumeLayout->addWidget(volumeName);
    masterVolumeLayout->addWidget(volumeSlider);

    QWidget* masterVolume = new QWidget();
    masterVolume->setLayout(masterVolumeLayout); */

    //Add the metadata element, the buttons, and set this to display theGUI environment
    playerLayout->addWidget(dataDisplay);
    playerLayout->addWidget(buttons);
    //playerLayout->addWidget(masterVolume);
    this->setLayout(playerLayout);

    //Set it so that the signal callBackFinished (the song finishes playing) it skips to the next song.
    connect(this, &MediaPlayer::callBackFinished, this, &MediaPlayer::skip);

    //Set the stream to close when the current song is removed from the playlist
    connect(this->currentPlaylist, &Playlist::songRemoved, this, &MediaPlayer::closeStream);
}

//Destructor
MediaPlayer::~MediaPlayer() {
    closeStream();
    av_packet_free(&currPacket);
    av_frame_free(&currFrame);
}

/*  --------------------------
 *
 *      Public Functions
 *
 *  -------------------------- */

//Set the playlist, and link the playlist so that when a song is selected in a playlist, it is set in the media player
void MediaPlayer::setPlaylist(Playlist* playlist) {
    this->currentPlaylist = playlist;

    // Store the original order of songs when shuffle is off
    this->originalOrder.resize(this->currentPlaylist->getSongList()->size());
    for (qint64 i = 0; i < this->originalOrder.size(); ++i) {
        this->originalOrder[i] = i;
    }

    copyOrder = originalOrder;

    std::sort(copyOrder.begin(), copyOrder.end());

    connect(this->currentPlaylist, &Playlist::newSelectedSong, this, &MediaPlayer::updateCurrentSong);
}

void MediaPlayer::updateLoopButtonStyle() {
    if (isLooped) {
        loopbutton->setStyleSheet("background-color: lightgreen;");
    } else {
        loopbutton->setStyleSheet("background-color: white;");
    }
}

void MediaPlayer::updateRandomButtonStyle() {
    if (ShuffleisRandom) {
        randomButton->setStyleSheet("background-color: lightgreen;");
    } else {
        randomButton->setStyleSheet("background-color: white;");
    }
}

//Create a PixMap image from the image data
void MediaPlayer::generateImage(QImage* songImage)
{
    QSize changedSize(450,450);
    QPixmap map;

    //If the playlistArt is not NULL, create the map, else use a generic image
    if (!songImage->isNull()) {
        map = QPixmap::fromImage(*songImage).scaled(changedSize, Qt::KeepAspectRatio);
    } else {
        QPixmap defaultImage(":/resources/images/DefaultMusicImage.svg");
        map = defaultImage.scaled(changedSize, Qt::KeepAspectRatio);
    }

    //Set the label to have the PixMap
    this->currentImage->setPixmap(map);
}

//Skip the current song
void MediaPlayer::skip() {
    // If no song is currently selected, ignore
    if (this->currentPlaylist->getSelectedSong() == -5)
        return;

    if (this->ShuffleisRandom) {
        shufflePlaylist();
        shouldChangeSong = true;
    } else {
        qint64 originalIndex = this->currentPlaylist->getSelectedSong();
        qint64 nextIndex = (originalIndex + 1) % this->currentPlaylist->getSongList()->size();
        this->shouldChangeSong = true;

        // Use copyOrder only when shuffling is disabled
        this->currentPlaylist->setSelectedSong(copyOrder[nextIndex], false);
        qDebug() << "og order: " << copyOrder;
    }

    // Close the current stream, and then play the new stream
    this->closeStream();
    this->play();
}




//Rewind current song
void MediaPlayer::rewind() {
    //If no song is currently selected, ignore
    if (this->currentPlaylist->getSelectedSong() == -5) {
        this->playbutton->setChecked(false);
        this->pausebutton->setChecked(false);
        return;
    }

    //Set the song to the previous. The function will set it to -5 if rewide goes before the song began
    if(!this->isLooped)
        this->currentPlaylist->setSelectedSong(this->currentPlaylist->getSelectedSong() - 1, false);

    this->playbutton->setChecked(true);

    //Close the current stream and then play the new stream
    this->closeStream();
    this->play();
}

//Just close the stream, a public interface for a private function
void MediaPlayer::killStream() {
    this->closeStream();
}


/*  -----------------
 *
 *      Protected
 *
 *  ----------------- */

//Update the currently playing song, needs to be done as a part of the connect API with the playlist. Essentially just does the constructor, but allows new titles
void MediaPlayer::updateCurrentSong(QString songPath, QString title, QString artist, QString album, QImage* songImage) {
    this->currentSongPath = songPath;
    this->currentSongTitle->setText(title);
    this->currentSongArtist->setText(artist);
    this->currentSongAlbum->setText(album);
    this->currentSongArt = songImage;
    generateImage(currentSongArt);
    this->closeStream();
    if (songPath != "") {
        this->play();
    } else {
        this->playbutton->setChecked(false);
    }
}



//Set the status of the loop variable back and forth
void MediaPlayer::swapLoop() {
    if (!this->isLooped) {
        this->isLooped = true;
    } else {
        this-> isLooped = false;
    }
    updateLoopButtonStyle();
    //std::cout << "Initial Loop Status: " << (this->isLooped ? "True" : "False") << std::endl;

}

void MediaPlayer::swapRandom() {
    ShuffleisRandom = !ShuffleisRandom;
    updateRandomButtonStyle();
    shouldChangeSong = false;
}

//Close the current decoding stream; no more file to audio
void MediaPlayer::closeStream() {
    if (audioDevice > 0) {
        SDL_PauseAudioDevice(audioDevice, true);
		SDL_CloseAudioDevice(audioDevice);
        audioDevice = 0;

		if (eqWindow->getEqualizer() != nullptr) eqWindow->getEqualizer()->flush();
    }

    avcodec_free_context(&currCodecCtx);
    avformat_close_input(&currFormatCtx);
}


//Create a new stream,allowing file to audio
bool MediaPlayer::openStream() {
    // in case another stream was open before
    closeStream();

    // init FFMPEG decoding things
    const char* file = this->currentSongPath.toLocal8Bit().data();

    if (avformat_open_input(&currFormatCtx, file, nullptr, nullptr) != 0) {
        // couldn't open the file at all
        std::cerr << "Couldn't open the file (path: \"" << file << "\")." << std::endl;
        return -1;
    }

    if (avformat_find_stream_info(currFormatCtx, nullptr) < 0) {
        // can't get stream info
        std::cerr << "Couldn't get the stream info for \"" << file << "\"" << std::endl;
        return -1;
    }

    // since MP3 only supports one stream, we can just use stream[0] and take it as is
    currCodec = avcodec_find_decoder(currFormatCtx->streams[0]->codecpar->codec_id);
    currCodecCtx = avcodec_alloc_context3(currCodec);
    if (avcodec_parameters_to_context(currCodecCtx, currFormatCtx->streams[0]->codecpar) < 0) {
        // can't get an audio context
        std::cerr << "Couldn't retrieve the audio codec for MP3s...?" << std::endl;
        return -1;
    }

    // stream opening time
    if (avcodec_open2(currCodecCtx, currCodec, nullptr) < 0) {
        // couldn't open the stream
        std::cerr << "Couldn't open the decoding stream." << std::endl;
        return -1;
    }

    // init SDL audio device
    SDL_AudioSpec desired, actual;
    SDL_memset(&desired, 0, sizeof(desired));
    desired.freq = currCodecCtx->sample_rate;
    desired.format = AUDIO_F32LSB;		// TODO: use currCodecCtx->sample_fmt?
    desired.channels = currCodecCtx->ch_layout.nb_channels;
    desired.samples = DEFAULT_BUFFER_SIZE;
    desired.callback = MediaPlayer::audioCallback;
    desired.userdata = this;

    audioDevice = SDL_OpenAudioDevice(nullptr, 0, &desired, &actual, SDL_AUDIO_ALLOW_SAMPLES_CHANGE);

    // av_dump_format(currFormatCtx, 0, file, 0);

    return true;
}


//Play a song, by disabling the pause
void MediaPlayer::play() {
    if (this->currentSongPath == "") {
        this->playbutton->setChecked(false);
        return;
    }
    if (!audioDevice && !openStream()) {
        std::cerr << "Couldn't initialize the audio stream." << std::endl;
        return;
    }

    this->pausebutton->setChecked(false);
    this->playbutton->setChecked(true);

    SDL_PauseAudioDevice(audioDevice, false);
}

//Pause a song, by enabling the pause.
void MediaPlayer::pause() {
    if (this->currentSongPath == "") {
        this->pausebutton->setChecked(false);
        return;
    }

    this->playbutton->setChecked(false);
    this->pausebutton->setChecked(true);

    if (audioDevice) SDL_PauseAudioDevice(audioDevice, true);
}


//This function processes the audio, and if it reads the end of the MP3 file, it will skip to the next song (auto play feature)
void MediaPlayer::audioCallback(void* userdata, Uint8* stream, int len) {
    MediaPlayer* player = (MediaPlayer*) userdata;
    bool error = false;
    bool endofSong = false;

	// TODO: Make sure that equalizer lag is accounted for (don't skip the last like 10 samples)


    // Check if the end of the song is reached
    if (endofSong && player->shouldChangeSong) {
        emit player->callBackFinished();
        player->shouldChangeSong = false;
        return;
    }
    // TODO: stream len doesn't align with num channels*float size?
    for (int i = 0; i < len; i++) {
        if (error) {
            stream[i] = 0;
            continue;
        }

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
                    stream[i] = 0;
                    error = true;
                    endofSong = true;
                    continue;
                }
                else if (res < 0) {
                    std::cerr << "Error reading the next packet." << std::endl;
                    // TODO: handle error?!?!
                    stream[i] = 0;
                    error = true;
                    continue;
                }
                // send the packet to the decoder to be processed
                if (avcodec_send_packet(player->currCodecCtx, player->currPacket) < 0) {
                    std::cerr << "Error sending the next packet to the decoder." << std::endl;
                    // TODO: handle error?!?!
                    stream[i] = 0;
                    error = true;
                    continue;
                }
                // alright, try again
                if (avcodec_receive_frame(player->currCodecCtx, player->currFrame) < 0) {
                    // ok what went wrong this time?!
                    std::cerr << "Couldn't retrieve the next frame to be played." << std::endl;
                    // TODO: handle error?!?!
                    stream[i] = 0;
                    error = true;
                    endofSong = true;
                    continue;
                }
            }
            else if (res == AVERROR_EOF) {
                std::cerr << "Found the end of the file." << std::endl;
                stream[i] = 0;
                error = true;
                player->pause();
                continue;
            }
            else if (res < 0) {
                // error getting the packet
                std::cerr << "Couldn't get the next frame..." << std::endl;
                // TODO: handle error?!?! Set silence?
                stream[i] = 0;
                error = true;
                continue;
            }

        }

        // read the current sample from the current frame (all channels)
        // assuming floats since it's MP3
		for (int ch = 0; ch < player->currCodecCtx->ch_layout.nb_channels; ++ch) {
			if (player->eqWindow->getEqualizer() == nullptr) {
				// just send the samples directly
				for (unsigned long long j = 0; j < sizeof(float); j++) {
					stream[i] = player->currFrame->data[ch][sizeof(float)*player->currSample + j];
					++i;
				}
			}
			else {
				// reroute to the equalizer first
				float f = player->eqWindow->getEqualizer()->getSample(ch,
							*((float*) &(player->currFrame->data[ch][sizeof(float)*player->currSample])));
				char* c = (char*) (&f);
				for (unsigned long long j = 0; j < sizeof(float); j++) {
					stream[i] = *c;
					++c;
					++i;
				}
			}
		}

        --i;	// reset loop counter to the last byte written (so it works next loop)

        ++player->currSample;

    }

	if (endofSong) emit player->callBackFinished();
}




void MediaPlayer::setEqualizer(EqualizerWindow* eqWindow) {
    this->eqWindow = eqWindow;
}


EqualizerWindow* MediaPlayer::getEqualizer() {
    return this->eqWindow;
}

void MediaPlayer::shufflePlaylist() {

    if (currentPlaylist) {
        // Check if originalOrder is empty or if it's already shuffled
        if (originalOrder.isEmpty() || std::is_sorted(originalOrder.begin(), originalOrder.end())) {
            // If it's empty or already shuffled, initialize it to the natural order
            originalOrder.resize(currentPlaylist->getSongList()->size());
            for (qint64 i = 0; i < originalOrder.size(); ++i) {
                originalOrder[i] = i;
            }
        }
        // Shuffle the original order
        std::random_shuffle(originalOrder.begin(), originalOrder.end());

        // Apply the shuffled order to the current playlist
        if(ShuffleisRandom){
        currentPlaylist->setShuffledOrder(originalOrder);
        qDebug() << "Shuffleorder: " << originalOrder;
        }
    }


}

