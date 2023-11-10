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
    this->imagePath = input.readLine();

    while (!input.atEnd()) {
        QString line = input.readLine();
        QByteArray byteArray = line.toUtf8();
        const char* songPath = byteArray.constData();
        Song* newSong = new Song(songPath);
        this->allSongs.append(newSong);
    }

    file->close();
}


void Playlist::savePlaylist(QString* filename) {
    QFile* file = new QFile(*filename);

    if (file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {

        QTextStream output(file);

        output << this->playlistName << "\n";
        output << this->userName << "\n";
        output << this->length << "\n";
        output << this->imagePath << "\n";

        for(int i = 0; i < this->allSongs.length(); i++) {
            output << *allSongs[i]->getSongPath() << "\n";
        }
    }
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
