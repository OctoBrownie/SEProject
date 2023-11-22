#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QFrame>
#include<QPushButton>
#include<QLabel>
#include<QString>
#include<QVariant>

#include "mediaplayer.h"

MediaPlayer::MediaPlayer(QWidget* parent): QWidget(parent)
{
    QVBoxLayout* playerLayout = new QVBoxLayout();
    playerLayout->setAlignment(Qt::AlignHCenter);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QVBoxLayout* songDataLayout = new QVBoxLayout();

    QFrame* songImage = new QFrame();
    songImage->setFixedSize(300, 300);
    songImage->setFrameStyle(QFrame::Box);
    songImage->setStyleSheet("background-color: white");
    QLabel* songName = new QLabel("Song");
    QLabel* albumName = new QLabel("Album");
    QLabel* artistName = new QLabel("Artist");

    songDataLayout->addWidget(songImage);
    songDataLayout->addWidget(songName);
    songDataLayout->addWidget(albumName);
    songDataLayout->addWidget(artistName);
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

    const QIcon* playIcon = new QIcon(":/resources/icons/Play.png");
    const QIcon* pauseIcon = new QIcon(":/resources/icons/Pause.png");
    const QIcon* skipIcon= new QIcon(":/resources/icons/Skip.png");
    const QIcon* backIcon = new QIcon(":/resources/icons/Back.png");
    const QIcon* eqIcon = new QIcon(":/resources/icons/EQ.png");
    const QIcon* loopIcon = new QIcon(":/resources/icons/Loop.png");

    playbutton->setIcon(*playIcon);
    pausebutton->setIcon(*pauseIcon);
    skipbutton->setIcon(*skipIcon);
    backbutton->setIcon(*backIcon);
    eqbutton->setIcon(*eqIcon);
    loopbutton->setIcon(*loopIcon);

    buttonLayout->addWidget(loopbutton);
    buttonLayout->addWidget(backbutton);
    buttonLayout->addWidget(playbutton);
    buttonLayout->addWidget(pausebutton);
    buttonLayout->addWidget(skipbutton);
    buttonLayout->addWidget(eqbutton);


    buttons->setLayout(buttonLayout);

    playerLayout->addWidget(dataDisplay);
    playerLayout->addWidget(buttons);
    setLayout(playerLayout);

    setMouseTracking(true);
}

void MediaPlayer::swapLoop() {
    if (this->isLooped == 0) {
        this->isLooped = 1;
    } else {
        this-> isLooped = 0;
    }
}

