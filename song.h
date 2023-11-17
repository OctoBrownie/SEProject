#ifndef SONG_H
#define SONG_H

#include <QString>
#include <QImage>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSize>

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/attachedpictureframe.h>

#include <customsongwidget.h>

class Song
{
public:
    //Create the song
    Song(const char* path);

    //Create a GUI element
    void createSongBox();
    QString* getSongPath();
    qint64 getDuration();
    qint64 getPosition();
    CustomSongWidget* getBox();
    void setPosition(qint64 newPosition);

private:
    //Path to the song, used by the MP3 player to play the song later
    QString* songPath;

    //Title of the song
    QString title;

    //The artists of the song (can be more than 1, but stored in a string)
    QString artists;

    //Album from the song
    QString album;

    //Album art
    QImage albumArt;

    //How long the song is, in seconds.
    qint64 duration;

    //This is the position in the playlist
    qint64 pPosition;

    //This is the respective GUI Widget
    CustomSongWidget* songBox;
};

#endif // SONG_H