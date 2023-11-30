#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QFrame>
#include<QPushButton>
#include<QLabel>
#include<QString>
#include<fftw3.h>
extern "C" {
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


#include "mediaplayer.h"

#define DEFAULT_BUFFER_SIZE 4096



MediaPlayer::MediaPlayer(QWidget* parent): QWidget(parent)
{
    this->currFormatCtx = nullptr;
    this->currCodec = nullptr;
    this->currCodecCtx = nullptr;
    this->currFrame = nullptr;
    this->currPacket = nullptr;

    this->audioDevice = 0;
    this->currPacket = av_packet_alloc();
    this->currFrame = av_frame_alloc();
    this->currSample = -1;

    QVBoxLayout* playerLayout = new QVBoxLayout();
    playerLayout->setAlignment(Qt::AlignHCenter);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QVBoxLayout* songDataLayout = new QVBoxLayout();

    this->currentSongTitle = new QLabel("Title");
    this->currentSongArtist = new QLabel("Artist");
    this->currentSongAlbum = new QLabel("Album");
    this->currentImage = new QLabel();
    this->currentSongArt = new QImage();
    this->generateImage(this->currentSongArt);

    songDataLayout->addWidget(this->currentImage);
    songDataLayout->addWidget(this->currentSongTitle);
    songDataLayout->addWidget(this->currentSongArtist);
    songDataLayout->addWidget(this->currentSongAlbum);
    songDataLayout->setAlignment(Qt::AlignHCenter);

    QFrame* dataDisplay = new QFrame();
    dataDisplay->setFixedSize(600, 600);
    dataDisplay->setFrameStyle(QFrame::Box);
    dataDisplay->setStyleSheet("background-color: white");

    dataDisplay->setLayout(songDataLayout);

    QWidget* buttons = new QWidget();
    QPushButton* playbutton = new QPushButton();
    QPushButton* pausebutton = new QPushButton();
    QPushButton* skipbutton = new QPushButton();
    QPushButton* backbutton = new QPushButton();
    QPushButton* loopbutton = new QPushButton();
    QPushButton* eqbutton = new QPushButton();
    QPushButton* randomButton = new QPushButton();

    const QIcon playIcon = QIcon(":/resources/icons/play.svg");
    const QIcon pauseIcon = QIcon(":/resources/icons/pause.svg");
    const QIcon skipIcon= QIcon(":/resources/icons/skip.svg");
    const QIcon backIcon = QIcon(":/resources/icons/rewind.svg");
    const QIcon eqIcon = QIcon(":/resources/icons/equalizer.svg");
    const QIcon loopIcon = QIcon(":/resources/icons/loop.svg");
    const QIcon randomIcon = QIcon(":/resources/icons/randomize.svg");

    playbutton->setIcon(playIcon);
    pausebutton->setIcon(pauseIcon);
    skipbutton->setIcon(skipIcon);
    backbutton->setIcon(backIcon);
    eqbutton->setIcon(eqIcon);
    loopbutton->setIcon(loopIcon);
    randomButton->setIcon(randomIcon);

    buttonLayout->addWidget(loopbutton);
    buttonLayout->addWidget(randomButton);
    buttonLayout->addWidget(backbutton);
    buttonLayout->addWidget(playbutton);
    buttonLayout->addWidget(pausebutton);
    buttonLayout->addWidget(skipbutton);
    buttonLayout->addWidget(eqbutton);

    connect(skipbutton, &QPushButton::clicked, this, &MediaPlayer::skip);
    connect(backbutton, &QPushButton::clicked, this, &MediaPlayer::rewind);
    connect(playbutton, SIGNAL (clicked()), this, SLOT (play()));
    connect(pausebutton, SIGNAL (clicked()), this, SLOT (pause()));

    buttons->setLayout(buttonLayout);

    playerLayout->addWidget(dataDisplay);
    playerLayout->addWidget(buttons);
    setLayout(playerLayout);

    connect(this, &MediaPlayer::callBackFinished, this, &MediaPlayer::skip);
    connect(this, &MediaPlayer::closeStream, this->currentPlaylist, &Playlist::songRemoved);
}

void MediaPlayer::swapLoop() {
    if (!this->isLooped) {
        this->isLooped = true;
    } else {
        this-> isLooped = false;
    }
}

void MediaPlayer::setPlaylist(Playlist* playlist) {
    this->currentPlaylist = playlist;
    connect(this->currentPlaylist, &Playlist::newSelectedSong, this, &MediaPlayer::updateCurrentSong);
}

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


void MediaPlayer::skip() {
    if (this->currentPlaylist->getSelectedSong() == -1)
        return;
    this->currentPlaylist->setSelectedSong(this->currentPlaylist->getSelectedSong() + 1, false);
    this->closeStream();
    this->play();
}

void MediaPlayer::rewind() {
    if (this->currentPlaylist->getSelectedSong() == -1)
        return;
    this->currentPlaylist->setSelectedSong(this->currentPlaylist->getSelectedSong() - 1, false);
    this->closeStream();
    this->play();
}


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
    }
}



MediaPlayer::~MediaPlayer() {
    closeStream();
    av_packet_free(&currPacket);
    av_frame_free(&currFrame);
}

void MediaPlayer::closeStream() {
    if (audioDevice > 0) {
        SDL_PauseAudioDevice(audioDevice, true);
        SDL_CloseAudioDevice(audioDevice);
        audioDevice = 0;
    }

    avcodec_free_context(&currCodecCtx);
    avformat_close_input(&currFormatCtx);
}

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

void MediaPlayer::play() {
    if (this->currentSongPath == "") {
        return;
    }
    if (!audioDevice && !openStream()) {
        std::cerr << "Couldn't initialize the audio stream." << std::endl;
        return;
    }

    SDL_PauseAudioDevice(audioDevice, false);
}

void MediaPlayer::pause() {
    if (this->currentSongPath == "") {
        return;
    }
    if (audioDevice) SDL_PauseAudioDevice(audioDevice, true);
}

void MediaPlayer::audioCallback(void* userdata, Uint8* stream, int len) {
    MediaPlayer* player = (MediaPlayer*) userdata;
    bool error = false;
    bool endofSong = false;


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
            for (unsigned long long j = 0; j < sizeof(float); j++) {
                stream[i] = player->currFrame->data[ch][sizeof(float)*player->currSample + j];
                ++i;
            }
        }

        --i;	// reset loop counter to the last byte written (so it works next loop)
        ++player->currSample;
    }

    if (endofSong)
        emit player->callBackFinished();


    // TODO: run stream through equalizer if no error
}

