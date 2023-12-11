#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "picturebox.h"
#include "textmetadata.h"
#include "song.h"
#include <QLabel>


//Playlist class
class Playlist: public QObject {
    Q_OBJECT

public:
    //Constructor
    Playlist(QString filename=nullptr);

    //Destructor
    ~Playlist();

    //Save the playlist to a specified filename
    void savePlaylist(QString* filename);

    //Add a song to a playlist
    void addSong(Song* newSong);

    //Remove a song from a playlist, based on a specific position
    void removeSong(qint64 position);


    //Setters
    void setImagePath(QString imagePath);
    void setUserName(QString name);
    void setPlaylistName(QString name);
    void setSelectedSong(qint64 pos, bool move);
    void setPlaylistPath(QString playlistPath);

    //Getters
    QString* getPlaylistPath();
    QString getUserName();
    QString getPlaylistName();
    qint64 getDuration();
    QString getImagePath();

    qint64 getSelectedSong();

    PictureBox* getPictureBox();
    TextMetadata* getTextMetadata();
    QWidget* getListGUI();

    QVector<Song*>* getSongList();

    QVBoxLayout* getLayout();

signals:
    //When a new song is selected, this signal is emitted to notify the player of the new song.
    void newSelectedSong(QString songPath, QString title, QString artist, QString album, QImage* songImage);

    //When a song is added, send out this signal. Used to update search results when songs are added.
    void newSongAdded();

    //When a song is removed, send out this signal (to stop the opened stream in the MP3 Player)
    void songRemoved();

protected:
    //Turn the playlist data into a GUI output
    void createPlaylistOutput();

    //Process the playlist from a file. This is the constructor behind the scenes.
    void processPlaylist(QString filename);

    //Calculate the playlist duration.
    void calculatePlaylistDuration();

    //Move a song at a given position either up (1) or down (0)
    void moveSong(int pos, int status);


    //Playlist path
    QString openedPlaylist;

    //Name, username, and duration
    QString playlistName;
    QString userName;
    qint64 duration = 0;

    //Image data
    QString imagePath;

    //Vector of all the songs
    QVector<Song*> allSongs;

    //Pointer in vector to current song, set to -5, as this marks that no song has been selected.
    qint64 selectedSong = -5;

     QVector<qint64> shuffledOrder;

    //Items for the picture box item
    PictureBox* playlistImageBox;

    //Items for the text metadata item
    TextMetadata* textData;

    //Items for the scrollable Song list item.
    //Want the reference to the GUI, so it can be accessed and manipulated into one object later in the application.
    QWidget* songsListGUI;

    //Reference to layout is to allow adding and removing objects from the list GUI.
    QVBoxLayout* songsListLayout;
};

#endif // PLAYLIST_H
