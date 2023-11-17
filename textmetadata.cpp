#include "textmetadata.h"
#include "playlist.h"

#include <QLineEdit>
#include <QLabel>

TextMetadata::TextMetadata(Playlist* playlist): QVBoxLayout()
{
    QString pName = playlist->getPlaylistName();
    QString uName = playlist->getUserName();
    qint64 duration = playlist->getDuration();
    QLineEdit* playlistTitle = new QLineEdit;
    playlistTitle->setPlaceholderText("Title");

    if (!pName.isNull())
        playlistTitle->insert(pName);

    connect(playlistTitle, &QLineEdit::textChanged, playlist, &Playlist::setPlaylistName);

    QLineEdit* username = new QLineEdit;
    username->setPlaceholderText("Username");

    if (!uName.isNull())
        username->insert(uName);

    connect(username, &QLineEdit::textChanged, playlist, &Playlist::setPlaylistName);

    QLabel* length = new QLabel("Length: " + QString::number(duration/60) + " minutes");

    addWidget(playlistTitle);
    addWidget(username);
    addWidget(length);

    this->duration = length;
}

void TextMetadata::updateDurationLabel(qint64 duration) {
    QLabel* length = this->duration;

    QLabel* newLength = new QLabel("Length: " + QString::number(duration/60) + " minutes");

    removeWidget(length);

    addWidget(newLength);

    delete length;
}
