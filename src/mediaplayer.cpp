#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QFrame>
#include<QPushButton>
#include<QLabel>
#include<QString>

#include "mediaplayer.h"

MediaPlayer::MediaPlayer(QWidget* parent): QWidget(parent)
{
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

    const QIcon playIcon = QIcon(":/resources/icons/play.svg");
    const QIcon pauseIcon = QIcon(":/resources/icons/pause.svg");
    const QIcon skipIcon= QIcon(":/resources/icons/skip.svg");
    const QIcon backIcon = QIcon(":/resources/icons/rewind.svg");
    const QIcon eqIcon = QIcon(":/resources/icons/equalizer.svg");
    const QIcon loopIcon = QIcon(":/resources/icons/loop.svg");

    playbutton->setIcon(playIcon);
    pausebutton->setIcon(pauseIcon);
    skipbutton->setIcon(skipIcon);
    backbutton->setIcon(backIcon);
    eqbutton->setIcon(eqIcon);
    loopbutton->setIcon(loopIcon);

    buttonLayout->addWidget(loopbutton);
    buttonLayout->addWidget(backbutton);
    buttonLayout->addWidget(playbutton);
    buttonLayout->addWidget(pausebutton);
    buttonLayout->addWidget(skipbutton);
    buttonLayout->addWidget(eqbutton);

    connect(skipbutton, &QPushButton::clicked, this, &MediaPlayer::skip);
    connect(backbutton, &QPushButton::clicked, this, &MediaPlayer::rewind);

    buttons->setLayout(buttonLayout);

    playerLayout->addWidget(dataDisplay);
    playerLayout->addWidget(buttons);
    setLayout(playerLayout);

    setMouseTracking(true);
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
    this->currentPlaylist->setSelectedSong(this->currentPlaylist->getSelectedSong() + 1, false);
}

void MediaPlayer::rewind() {
    this->currentPlaylist->setSelectedSong(this->currentPlaylist->getSelectedSong() - 1, false);
}


void MediaPlayer::updateCurrentSong(QString songPath, QString title, QString artist, QString album, QImage* songImage) {
    this->currentSongPath = songPath;
    this->currentSongTitle->setText(title);
    this->currentSongArtist->setText(artist);
    this->currentSongAlbum->setText(album);
    this->currentSongArt = songImage;
    generateImage(currentSongArt);
}

