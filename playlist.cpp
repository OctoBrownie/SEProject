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
        Song* newSong = new Song(&line);
        this->allSongs.append(newSong);
    }

    file->close();
}

void Playlist::displayPlaylist() {
    qDebug() << "Playlist Name: " << this->playlistName << "\n";
    qDebug() << "User Name: " << this->userName << "\n";
    qDebug() << "Length: " << this->length << "\n";

    for(int i =0; i < allSongs.length(); i++) {
        Song* songToPrint = allSongs[i];
        songToPrint->printSong();
    }
}

QWidget* Playlist::createPlaylistOutput() {
    QWidget* output = new QWidget();
    QVBoxLayout* outLayout = new QVBoxLayout();

    for(int i =0; i < allSongs.length(); i++) {
        Song* songToPrint = allSongs[i];
        QWidget* songBox = songToPrint->createSongBox();
        outLayout->addWidget(songBox);
    }

    output->setLayout(outLayout);
    return output;
}
