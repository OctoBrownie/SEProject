#include "song.h"

Song::Song(QString* path)
{
    QEventLoop loop;
    this->connector->setSource(QUrl::fromLocalFile(*path));


    connect(this->connector, &QMediaPlayer::mediaStatusChanged, this, &Song::getMetaData);
    connect(this, &Song::metadataLoaded, &loop, &QEventLoop::quit);
    loop.exec();
}

void Song::getMetaData(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::LoadedMedia) {
        QMediaMetaData data = this->connector->metaData();
        this->title = data[QMediaMetaData::Title].toString();

        this->artists = data[QMediaMetaData::Author].toStringList();
        if (this->artists.length() == 0) {
            this->artists = data[QMediaMetaData::ContributingArtist].toStringList();
        }

        this->album = data[QMediaMetaData::AlbumTitle].toString();
        this->albumArt = data[QMediaMetaData::CoverArtImage].value<QImage>();
        this->duration = data[QMediaMetaData::Duration].toInt();


        emit metadataLoaded();
    }
}

void Song::printSong() {
    qDebug() << "Title: " << this->title << "\n";
    qDebug() << "Album: " << this->album << "\n";
    qDebug() << "Duration: " << this->duration << "\n";

    if (artists.length() > 0){
        QString mergedString = artists[0];
        for (int i = 1; i < artists.length(); i++) {
            mergedString += artists[i];
        }
        qDebug() << "Artists: " << mergedString << "\n";
    } else {
        qDebug() << "No artists" << "\n";
    }

    qDebug() << "Album: " << this->album << "\n";
    qDebug() << "Duration: " << this->duration << "\n";
}
