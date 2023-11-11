#include "music_app.h"
#include "mp3_player.h"

#include<QPushButton>
#include<QApplication>
#include<QWidget>
#include<QLineEdit>
#include<QVBoxLayout>
#include<QMediaPlayer>
#include<QAudioOutput>

Music_App::Music_App(QWidget *parent) : QWidget{parent} {
	Mp3_Player p;
	setBaseSize(100, 50);

	QVBoxLayout* layout = new QVBoxLayout(this);

	this->editor = new QLineEdit("Add a real path", this);
	layout->addWidget(this->editor);

	QPushButton* b = new QPushButton("Play this file!");
	b->setGeometry(10, 10, 80, 30);
	layout->addWidget(b);
	connect(b, SIGNAL (clicked()), this, SLOT (startMusic()));

	b = new QPushButton("Start/stop");
	b->setGeometry(10, 10, 80, 30);
	layout->addWidget(b);
	connect(b, SIGNAL (clicked()), this, SLOT (startStop()));

	this->mediaPlayer = new QMediaPlayer;
	this->audioOutput = new QAudioOutput;
	this->mediaPlayer->setAudioOutput(this->audioOutput);
}

Music_App::~Music_App() {
	free(this->mediaPlayer);
	free(this->audioOutput);
}

// slot for button
void Music_App::startMusic() {
	mediaPlayer->setSource(QUrl::fromLocalFile(this->editor->text()));
	audioOutput->setVolume(0.5);
	mediaPlayer->play();
}

void Music_App::startStop() {
	if (mediaPlayer->isPlaying()) mediaPlayer->stop();
	else mediaPlayer->play();
}
