#include<QWidget>
#include<QVBoxLayout>
#include<QLabel>
#include<QLineEdit>

#include "textmetadata.h"


//Constructor
TextMetadata::TextMetadata(QString pName, QString uName, qint64 duration): QVBoxLayout()
{
    //Create a box where the user can edit the playlist title.
    QLineEdit* playlistTitle = new QLineEdit;

    //Create some placeholder text named "Title"
    playlistTitle->setPlaceholderText("Title");

    //If the input is not NULL, insert into the box the inputted string
    if (!pName.isNull())
        playlistTitle->insert(pName);

    //Save to the private variable.
    this->playlistName = playlistTitle;


    //Create a box where the user can edit the username
    QLineEdit* username = new QLineEdit;

    //Create some placeholder text named "Username"
    username->setPlaceholderText("Username");

    //If the input is not NULL, insert into the box the inputted string
    if (!uName.isNull())
        username->insert(uName);

    //Save to the private variable.
    this->userName = username;


    //Create a duration label
    QLabel* length = new QLabel("Length: " + QString::number(duration/60) + " minutes");

    //Set the duration equal to the label.
    this->duration = length;

    //Add all of them to the QVBoxLayout
    addWidget(playlistTitle);
    addWidget(username);
    addWidget(length);
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

    removeWidget(length);

    addWidget(this->duration);

    delete length;
}


