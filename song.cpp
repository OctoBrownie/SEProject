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
        this->albumArt = data[QMediaMetaData::ThumbnailImage].value<QImage>();
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


QWidget* Song::createSongBox() {
    QWidget* box = new QWidget();
    QHBoxLayout* outerLayout = new QHBoxLayout();
    QVBoxLayout* innerLayout = new QVBoxLayout();
    QLabel* titleText = new QLabel(this->title);
    QString mergedString;

    if (this->artists.length() == 1) {
        mergedString = this->artists[0];
    }
    else if (this->artists.length() > 0) {
        mergedString = this->artists[0] + ", ";
        for (int i = 1; i < this->artists.length() - 1; i++) {
            mergedString += this->artists[i];
            mergedString += ", ";
        }
        mergedString += this->artists[this->artists.length() - 1];
    } else {
        mergedString = "No artist found!\n";
    }

    QLabel* artistText = new QLabel(mergedString);
    QLabel* albumTest = new QLabel(this->album);

    innerLayout->addWidget(titleText);
    innerLayout->addWidget(artistText);
    innerLayout->addWidget(albumTest);

    QLabel* art = new QLabel();
    QImage image(100, 100, QImage::Format_RGB888);
    image.fill(Qt::white);

    art->setPixmap(QPixmap::fromImage(image));
    outerLayout->addWidget(art);
    outerLayout->addLayout(innerLayout);
    box->setLayout(outerLayout);
    return box;
}
