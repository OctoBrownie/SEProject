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

protected:
    void processPlaylist(QString* filename);
    void recalcLength();


private:
    QString* openedPlaylist = nullptr;
    QString playlistName = nullptr;
    QString userName = nullptr;
    qint64 length = 0;
    QVector<Song*> allSongs;

    int playlistPlacement = -1;
};

#endif // PLAYLIST_H
