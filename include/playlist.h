#ifndef PLAYLIST_H
#define PLAYLIST_H


#include<QVector>
class QLabel;
class QVBoxLayout;

#include "textmetadata.h"
class MusicPlayer;
class Song;


//Playlist class
class Playlist: public QObject {
public:
	// constructor for outside classes, processes the given file
	// will return a blank playlist if the file is null, will return nullptr if
	// the file is invalid
	static Playlist* createPlaylist(QString filename=nullptr, MusicPlayer* player=nullptr, QObject* parent=nullptr);
	virtual ~Playlist();

	//Save the playlist to a specified filename
	void savePlaylist(QString* filename);

    //Add a song to a playlist
	void addSong(Song* newSong);

	//Remove a song from a playlist, based on a specific position
	void removeSong(qint64 position);

    //Return the currently selected song; technically a getter but really important
	qint64 getSelectedSong();

    //Setters
	void setImagePath(QString imagePath);
    void setUserName(QString name);
	void setPlaylistName(QString name);
	void setSelectedSong(qint64 pos, bool move);


    //Getters
	QString getImagePath();
    QString getUserName();
    QString getPlaylistName();
    qint64 getDuration();

	TextMetadata* getTextMetadata();
    QWidget* getListGUI();
	QVBoxLayout* getLayout() { return this->songsListLayout;};


public slots:
	// save and overwrite the playlist in the current file
	void save() { savePlaylist(&openedPlaylist); }

	// removes the currently selected song
	void removeSong() { removeSong(selectedSong); }

signals:
	void newSelectedSong(QString songPath, QString title, QString artist, QString album, QImage* songImage);

protected:
	Q_OBJECT

	// constructor for this class
	Playlist(MusicPlayer* p = nullptr, QObject* parent = nullptr);

    //Turn the playlist data into a GUI output
	void createPlaylistOutput();

    //Calculate the playlist duration.
    void calculatePlaylistDuration();

	//Move a song at a given position (make it go up or down one)
	void moveSong(int pos, bool up);


	MusicPlayer* musicPlayer;

	//Playlist path
    QString openedPlaylist;

    //Name, username, and duration
    QString playlistName;
    QString userName;
    qint64 duration = 0;

    //Image data
	QString imagePath;

    //Vector of all the songs
	QVector<Song*>* allSongs;

    //Pointer in vector to current song, set to -1 as the first value is 0. This tells the system when to not look for a value.
	qint64 selectedSong = -1;

    //Items for the text metadata item
    TextMetadata* textData;

    //Items for the scrollable Song list item.
    //Want the reference to the GUI, so it can be accessed and manipulated into one object later in the application.
    QWidget* songsListGUI;

    //Reference to layout is to allow adding and removing objects from the list GUI.
    QVBoxLayout* songsListLayout;
};

#endif // PLAYLIST_H
