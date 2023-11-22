#ifndef PLAYLIST_H
#define PLAYLIST_H

#include<QWidget>
#include<QVBoxLayout>
#include<QVector>
#include<QFile>
#include<QTextStream>
#include<QScrollArea>

#include "picturebox.h"
#include "textmetadata.h"
#include "song.h"


//Playlist class
class Playlist: public QObject
{
public:
    //Constructor
    Playlist(QString filename=nullptr);

    //Save the playlist to a specified filename
    void savePlaylist(QString* filename);

    //Add a song to a playlist
    void addSong(Song* newSong);

    //Remove a song from a playlist, based on a specific position
    void removeSong(qint64 position);

    //Return the currently selected song; technically a getter but really important
    Song* getSelectedSong();

    //Setters

    void setImage(QString imagePath);

    void setUserName(QString name);

    void setPlaylistName(QString name);

    void setSelectedSong(qint64 pos);


    //Getters
    QImage* getImage();
    QString getUserName();
    QString getPlaylistName();
    qint64 getDuration();

    PictureBox* getPictureBox();
    TextMetadata* getTextMetadata();
    QWidget* getListGUI();


protected:
    //Turn the playlist data into a GUI output
    void createPlaylistOutput();

    //Process the playlist from a file. This is the constructor behind the scenes.
    void processPlaylist(QString filename);

    //Calculate the playlist duration.
    void calculatePlaylistDuration();

    //Move a song at a given position either up (1) or down (0)
    void moveSong(int pos, int status);


private:
    //Playlist path
    QString openedPlaylist;

    //Name, username, and duration
    QString playlistName;
    QString userName;
    qint64 duration = 0;

    //Image data
    QString imagePath;
    QImage playlistImage;

    //Vector of all the songs
    QVector<Song*> allSongs;

    //Pointer in vector to current song, set to -1 as the first value is 0. This tells the system when to not look for a value.
    qint64 selectedSong = -1;


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
