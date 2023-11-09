#include "song.h"

Song::Song(QString* path)
{
    this->connector->setSource(QUrl::fromLocalFile(*path));

    connect(this->connector, &QMediaPlayer::mediaStatusChanged, this, &Song::getMetaData);
}

void Song::getMetaData(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::LoadedMedia) {
        QMediaMetaData data = this->connector->metaData();
        this->title = data[QMediaMetaData::Title].toString();
        this->artists = data[QMediaMetaData::Author].toStringList();
        this->album = data[QMediaMetaData::AlbumTitle].toString();
        this->albumArt = data[QMediaMetaData::CoverArtImage].value<QImage>();
        this->duration = data[QMediaMetaData::Duration].toInt();
    }
}
