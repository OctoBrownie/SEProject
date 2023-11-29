#include "library.h"
#include "playlist.h"

#define PLAYLIST_DIR_FILE "playlistdirs.txt"
#define SONG_DIR_FILE "songdirs.txt"

Library::Library(QObject* parent) : QObject(parent) {
	// load songs and playlists from file
	this->playlists = new QVector<Playlist*>();
//	playlists->append(Playlist::createPlaylist("[add test here]"));
}

void Library::search(QString query) const {}

void Library::indexSongs() {}

void Library::indexPlaylists() {}
