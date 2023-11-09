#ifndef SONG_H
#define SONG_H

#include <QString>
#include <QImage>
#include <QMediaPlayer>
#include <QObject>
#include <QMediaMetaData>

class Song: public QObject
{
public:
    Song(QString* path);
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
