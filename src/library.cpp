// std lib
#include<iostream>

// Qt
#include<QCoreApplication>
#include<QFile>
#include<QStandardPaths>
#include<QDirIterator>

// project
#include "library.h"
#include "playlist.h"

#define PLAYLIST_DIR_FILE "playlistdirs.txt"
#define SONG_DIR_FILE "songdirs.txt"

Library::Library(QObject* parent) : QObject(parent) {
	// init all vectors
	this->playlistDirs = new QVector<QString*>();
	this->playlists = new QVector<Playlist*>();
	this->songDirs = new QVector<QString*>();
	this->songs = new QVector<Song*>();

	playlists->append(Playlist::createPlaylist("C:/Users/Crystal/Downloads/asdf.pa"));

	// load songs and playlists from file
	loadSongDirs();
	loadPlaylistDirs();
}

Library::~Library() {
	if (this->playlistDirs != nullptr) delete this->playlistDirs;
	if (this->playlists != nullptr) delete this->playlists;

	if (this->songDirs != nullptr) delete this->songDirs;
	if (this->songs != nullptr) delete this->songs;
}

void Library::search(QString query) const {}

void Library::loadSongDirs() {
	this->songDirs->clear();

	QFile *dirFile = new QFile(QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, SONG_DIR_FILE));
	if (!dirFile->exists()) {
		delete dirFile;
		dirFile = new QFile(QCoreApplication::applicationDirPath() + '/' + SONG_DIR_FILE);
		if (!dirFile->exists()) {
			delete dirFile;
			std::cerr << "The song directories file doesn't exist." << std::endl;
			return;
		}
	}

	if (!dirFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
		std::cerr << "Couldn't open the song directories file." << std::endl;
		this->songs->clear();
		return;
	}
	QTextStream input(dirFile);
	QString* path;
	while (!input.atEnd()) {
		path = new QString(input.readLine());
		if (path != nullptr && !path->isEmpty()) this->songDirs->append(path);
	}

	dirFile->close();

	indexSongs();
}

void Library::loadPlaylistDirs() {
	this->playlistDirs->clear();

	QFile *dirFile = new QFile(QStandardPaths::locate(QStandardPaths::AppLocalDataLocation, PLAYLIST_DIR_FILE));
	if (!dirFile->exists()) {
		delete dirFile;
		dirFile = new QFile(QCoreApplication::applicationDirPath() + '/' + PLAYLIST_DIR_FILE);
		if (!dirFile->exists()) {
			delete dirFile;
			std::cerr << "The playlist directories file doesn't exist." << std::endl;
			return;
		}
	}

	if (!dirFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
		std::cerr << "Couldn't open the playlist directories file." << std::endl;
		this->songs->clear();
		return;
	}
	QTextStream input(dirFile);
	QString* path;
	while (!input.atEnd()) {
		path = new QString(input.readLine());
		if (path != nullptr && !path->isEmpty()) {
			this->playlistDirs->append(path);
		}
	}

	dirFile->close();

	indexPlaylists();
}

void Library::indexSongs() {
	this->songs->clear();

	Song* temp;
	for (QString* path : *songDirs) {
		if (path == nullptr) continue;
		QDirIterator iter(*path, QStringList() << "*.mp3", QDir::NoFilter, QDirIterator::Subdirectories);
		while (iter.hasNext()) {
			temp = Song::createSong(iter.next());
			if (temp == nullptr) continue;
			this->songs->append(temp);
		}

	}
}

void Library::indexPlaylists() {
	this->playlists->clear();

	Playlist* temp;
	for (QString* path : *playlistDirs) {
		if (path == nullptr) continue;
		QDirIterator iter(*path, QStringList() << "*.pa", QDir::NoFilter, QDirIterator::Subdirectories);
		while (iter.hasNext()) {
			temp = Playlist::createPlaylist(iter.next());
			if (temp == nullptr) continue;
			this->playlists->append(temp);
		}

	}
}
