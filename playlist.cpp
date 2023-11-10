#include "playlist.h"

Playlist::Playlist(QString* filename)
{
    this->processPlaylist(filename);
}

void Playlist::processPlaylist(QString* filename) {
    QFile* file = new QFile(*filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        //Error handling. Will probably raise an alert, and exit.
        return;
    }

    QTextStream input(file);

    this->openedPlaylist = filename;
    this->playlistName = input.readLine();
    this->userName = input.readLine();
    this->length = input.readLine().toInt();

    while (!input.atEnd()) {
        QString line = input.readLine();
        QByteArray byteArray = line.toUtf8();
        const char* songPath = byteArray.constData();
        Song* newSong = new Song(songPath);
        this->allSongs.append(newSong);
    }

    file->close();
}


QWidget* Playlist::createPlaylistOutput() {
    QWidget* output = new QWidget();
    QVBoxLayout* outLayout = new QVBoxLayout();

    for(int i =0; i < allSongs.length(); i++) {
        Song* songToPrint = allSongs[i];
        CustomSongWidget* songBox = songToPrint->createSongBox();
        outLayout->addWidget(songBox);
    }

    output->setLayout(outLayout);
    return output;
}
