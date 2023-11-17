#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QScrollArea>
#include <picturebox.h>
#include <textmetadata.h>
#include <editingbuttons.h>
#include <customsongwidget.h>

class Song;

class Playlist: public QObject
{
public:
    Playlist(QString* filename=nullptr);
    void savePlaylist(QString* filename);
    void addSong(Song* newSong);
    void removeSong(qint64 position);
    Song* getSelectedSong();

    void setImage(QString imagePath);
    void setUserName(QString name);
    void setPlaylistName(QString name);

    void setSelectedSong(qint64 pos);

    QImage* getImage();
    QString getUserName();
    QString getPlaylistName();
    qint64 getDuration();

    PictureBox* getPictureBox();
    TextMetadata* getTextMetadata();
    EditingButtons* getEditingButtons();
    QWidget* getListGUI();

protected:
    void createPlaylistOutput();
    void processPlaylist(QString* filename);
    void calcLength();


private:
    //Playlist path
    QString* openedPlaylist;

    //Name, username, and duration
    QString playlistName;
    QString userName;
    qint64 duration = 0;

    //Image data
    QString imagePath;
    QImage playlistImage;

    //Vector of all the songs
    QVector<Song*> allSongs;

    //Pointer in vector to current song, set to -1 as the first value is
    qint64 selectedSong = -1;


    //Items for the picture box item
    PictureBox* playlistImageBox;

    //Items for the text metadata item
    TextMetadata* textData;

    //Items for the application buttons
    EditingButtons* editButtons;

    //Items for the scrollable Song list item.
    QWidget* songsListGUI;
    QVBoxLayout* songsListLayout;

    QWidget* fullWidget;
};

#endif // PLAYLIST_H
