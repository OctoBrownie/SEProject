#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "picturebox.h"
#include "textmetadata.h"
#include "song.h"


//Playlist class
class Playlist: public QObject {
public:
	// constructor for outside classes, processes the given file
	// will return a blank playlist if the file is null, will return nullptr if
	// the file is invalid
	static Playlist* createPlaylist(QString filename=nullptr, QObject* parent=nullptr);
	virtual ~Playlist();

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


public slots:
	// save and overwrite the playlist in the current file
	void save() { savePlaylist(&openedPlaylist); }

	// removes the currently selected song
	void removeSong() { removeSong(selectedSong); }


protected:
	Q_OBJECT

	// constructor for this class
	Playlist(QObject* parent = nullptr);

    //Turn the playlist data into a GUI output
	void createPlaylistOutput();

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
