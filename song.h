#ifndef SONG_H
#define SONG_H

#include <QString>
#include <QImage>
#include <QMediaPlayer>
#include <QObject>
#include <QMediaMetaData>
#include <QEventLoop>

class Song: public QObject
{
    Q_OBJECT

public:
    Song(QString* path);
    void printSong();

signals:
    void metadataLoaded();

public slots:
    void getMetaData(QMediaPlayer::MediaStatus status);

private:
    QString title;
    QStringList artists;
    QString album;
    QImage albumArt;
    qint64 duration;
    QMediaPlayer* connector = new QMediaPlayer();
};

#endif // SONG_H
