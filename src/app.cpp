#include "app.h"
#include "mp3player.h"

#include<QPushButton>
#include<QWidget>
#include<QLineEdit>
#include<QVBoxLayout>
#include<QMediaPlayer>
#include<QAudioOutput>

App::App(QWidget *parent) : QWidget{parent} {
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

	layout->addWidget(new Mp3Player());

	this->mediaPlayer = new QMediaPlayer;
	this->audioOutput = new QAudioOutput;
	this->mediaPlayer->setAudioOutput(this->audioOutput);
}

App::~App() {
	delete this->mediaPlayer;
	delete this->audioOutput;
}

// slot for button
void App::startMusic() {
	mediaPlayer->setSource(QUrl::fromLocalFile(this->editor->text()));
	audioOutput->setVolume(0.5);
	mediaPlayer->play();
}

void App::startStop() {
	if (mediaPlayer->isPlaying()) mediaPlayer->stop();
	else mediaPlayer->play();
}
