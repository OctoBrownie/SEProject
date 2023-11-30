#include<QLabel>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QMouseEvent>

#include "customsongwidget.h"
#include "song.h"

CustomSongWidget::CustomSongWidget(Song* song): QWidget() {
	QHBoxLayout* outerLayout = new QHBoxLayout();
	QVBoxLayout* innerLayout = new QVBoxLayout();
	QLabel* titleText = new QLabel(song->getSongTitle());
	QLabel* artistText = new QLabel(song->getArtist());
	QLabel* albumTest = new QLabel(song->getAlbum());

	this->fromSong = song;
	this->selected = false;

	innerLayout->addWidget(titleText);
	innerLayout->addWidget(artistText);
	innerLayout->addWidget(albumTest);

	QLabel* art = new QLabel();
	QSize changedSize(100,100);
	QPixmap map;

	if (!song->getArt()->isNull()) {
		map = QPixmap::fromImage(*song->getArt()).scaled(changedSize, Qt::KeepAspectRatio);
	} else {
		QPixmap defaultImage(":/images/noAlbumArt.svg");
		map = defaultImage.scaled(changedSize, Qt::KeepAspectRatio);
	}
	art->setPixmap(map);

	QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

	outerLayout->addWidget(art);
	outerLayout->addSpacerItem(spacer);
	outerLayout->addLayout(innerLayout);
	outerLayout->setAlignment(Qt::AlignLeft);
	setLayout(outerLayout);
}

void CustomSongWidget::mousePressEvent(QMouseEvent *event) {
	fromSong->setSelected(!this->selected);
}


void CustomSongWidget::setBackground(int value) {
	if (value == 1) setStyleSheet("background-color:#73b2e4");
	else setStyleSheet("background-color: #F0F0F0");

	this->selected = value;
}