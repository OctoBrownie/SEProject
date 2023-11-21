#ifndef SONG_H
#define SONG_H

#include<QWidget>

class QString;
class QImage;

class Playlist;
class CustomSongWidget;

class Song {
public:
	//Create the song
	Song(const char* path);

	//Create a GUI element
	void createSongBox();
	CustomSongWidget* getBox();


	QString* getSongPath();
	qint64 getDuration();
	qint64 getPosition();
	QString getSongTitle();
	QString getArtist();
	QString getAlbum();
	QImage* getArt();
	void setPosition(qint64 newPosition);
	// void setPlaylist(Playlist* playlist);
	void setSelected(int value);
	void deactivate();


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

	//How long the song is (sec)
	qint64 duration;

	//Position in the playlist
	qint64 pPosition;

	// TODO: integrate playlists
	// Playlist* parentPlaylist;

	//Corresponding GUI Widget
	CustomSongWidget* songBox;

	int selected = 0;
};

#endif // SONG_H
