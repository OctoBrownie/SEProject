#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QWidget>
#include <QLabel>
#include "playlist.h"

class QString;

class MediaPlayer: public QWidget
{
public:
    MediaPlayer(QWidget* parent=nullptr);

    void skip();

    void rewind();

    void setPlaylist(Playlist* playlist);



protected:
    void swapLoop();

    void generateImage(QImage* songImage);

    void updateCurrentSong(QString songPath, QString title, QString artist, QString album, QImage* songImage);

private:
    QString currentSongPath;
    QLabel* currentSongTitle;
    QLabel* currentSongAlbum;
    QLabel* currentSongArtist;
    QLabel* currentImage;
    QImage* currentSongArt;
    bool isLooped = false;

    Playlist* currentPlaylist;
};

#endif // MEDIAPLAYER_H
