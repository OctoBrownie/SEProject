// Qt
#include<QHBoxLayout>
#include<QLabel>
#include<QPushButton>

#include "playerwidget.h"
#include "musicplayer.h"
#include "song.h"

PlayerWidget::PlayerWidget(MusicPlayer* p, QWidget *parent)
	: QWidget{parent}, player{p} {

	QSizePolicy policy = QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
	policy.setRetainSizeWhenHidden(false);
	this->setSizePolicy(policy);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	//	layout->setSpacing(0);

	this->albumArt = new QLabel(this);
	QSize changedSize(100,100);
	QPixmap img(":/images/noAlbumArt.svg");
	this->defaultImage = img.scaled(changedSize, Qt::KeepAspectRatio);
	albumArt->setPixmap(defaultImage);
	layout->addWidget(albumArt);

	this->songName = new QLabel("artist - song", this);
	layout->addWidget(songName);
	layout->addStretch();

	// size policy for buttons
	policy = QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	policy.setVerticalStretch(1);

	// rewind button
	QPushButton* b = new QPushButton(this);
	b->setIcon(QIcon(":/icons/rewind.svg"));
	b->setSizePolicy(policy);
	layout->addWidget(b);

	// play/pause button
	// TODO: swap play/pause icons
	b = new QPushButton(this);
	b->setIcon(QIcon(":/icons/play.svg"));
	b->setSizePolicy(policy);
	layout->addWidget(b);
	connect(b, SIGNAL(clicked()), p, SLOT(pause()));

	// skip button
	b = new QPushButton(this);
	b->setIcon(QIcon(":/icons/skip.svg"));
	b->setSizePolicy(policy);
	layout->addWidget(b);

	if (p != nullptr) connect(p, SIGNAL(playbackStarted()), this, SLOT(updateLabels()));
}

void PlayerWidget::updateLabels() {
	const Song* s = this->player->getSong();
	songName->setText(s->getSongTitle());

	QImage* img = s->getArt();
	if (img == nullptr) albumArt->setPixmap(this->defaultImage);
	else {
		QSize changedSize(100,100);
		albumArt->setPixmap(QPixmap::fromImage(*s->getArt()).scaled(changedSize, Qt::KeepAspectRatio));
	}
}
