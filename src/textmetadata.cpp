#include<QWidget>
#include<QVBoxLayout>
#include<QLabel>
#include<QLineEdit>

#include "textmetadata.h"


//Constructor
TextMetadata::TextMetadata(QString pName, QString uName, qint64 duration, QWidget* parent)
	: QWidget(parent) {
	this->layout = new QVBoxLayout(this);

	//editable playlist title
	QLineEdit* playlistTitle = new QLineEdit;
	playlistTitle->setPlaceholderText("Title");

	if (!pName.isNull()) playlistTitle->insert(pName);
	this->playlistName = playlistTitle;


	//editable username
    QLineEdit* username = new QLineEdit;
	username->setPlaceholderText("Username");

	if (!uName.isNull()) username->insert(uName);
    this->userName = username;


	// duration label
	this->duration = new QLabel("Length: " + QString::number(duration/60) + " minutes");

	// add all widgets to the QVBoxLayout
	layout->addWidget(this->playlistName);
	layout->addWidget(this->userName);
	layout->addWidget(this->duration);
}


/*

    Getters

*/
QLineEdit* TextMetadata::getPlaylistTitle() {
    return this->playlistName;
}

QLineEdit* TextMetadata::getUsername() {
    return this->userName;
}


//Update the playlist duration label
void TextMetadata::updateDurationLabel(qint64 duration) {
    QLabel* length = this->duration;

    this->duration = new QLabel("Length: " + QString::number(duration/60) + " minutes");

	this->layout->removeWidget(length);
	this->layout->addWidget(this->duration);

    delete length;
}


