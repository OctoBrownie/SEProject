// std lib
#include<iostream>

// Qt
#include<QLabel>
#include<QHBoxLayout>
#include<QMouseEvent>

// library
#include "playlist.h"
#include "playlistwidget.h"

#define DEFAULT_NAME_TEXT "[playlist name]"

PlaylistWidget::PlaylistWidget(Playlist* p, QWidget* parent) : QWidget{parent} {
	selected = false;

	nameLabel = new QLabel(this);
	durationLabel = new QLabel(this);

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->addWidget(nameLabel);
	layout->addWidget(durationLabel);

	setPlaylist(p);
}

void PlaylistWidget::setPlaylist(Playlist* p) {
	nameLabel->setText(p == nullptr ? DEFAULT_NAME_TEXT : p->getPlaylistName());
	durationLabel->setText(QString::number(p == nullptr ? 0 : p->getDuration()/60) + " min");
	currPlaylist = p;
}

void PlaylistWidget::setSelected(bool s) {
	if (selected == s) return;
	selected = s;

	if (selected) {
		setStyleSheet("background-color:#eeefff;");
	}
	else {
		setStyleSheet("background-color:#00ffffff;");
	}
}

void PlaylistWidget::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		setSelected(!selected);
	}
}

void PlaylistWidget::mouseDoubleClickEvent(QMouseEvent *event) {
	setSelected(false);
	if (event->button() == Qt::LeftButton) {
		// should open up the playlist
		emit openPlaylist(this->currPlaylist);
	}
}


