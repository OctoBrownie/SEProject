#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <song.h>
#include <customsongwidget.h>

class Playlist
{
public:
    Playlist(QString* filename=nullptr);
    QWidget* createPlaylistOutput();
    void savePlaylist(QString* filename);


protected:
    void processPlaylist(QString* filename);
    void recalcLength();


private:
    QString* openedPlaylist;
    QString playlistName;
    QString userName;
    qint64 length = 0;
    QString imagePath;
    QVector<Song*> allSongs;

    int playlistPlacement = -1;
};

#endif // PLAYLIST_H
