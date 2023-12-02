#ifndef LIBRARY_H
#define LIBRARY_H

#include<QObject>
#include<QVector>

class Playlist;
class Song;

class Library : public QObject {
protected:
	Q_OBJECT
	QVector<Playlist*>* playlists;
	QVector<QString*>* playlistDirs;

	QVector<Song*>* songs;
	QVector<QString*>* songDirs;


	void loadSongDirs();
	void loadPlaylistDirs();

	void indexSongs();
	void indexPlaylists();

public:
	Library(QObject* parent=nullptr);
	virtual ~Library();

	QVector<Song*>* search(QString query) const;

	const QVector<Playlist*>* getPlaylists() const { return playlists; }

	// TODO: setters for adding/deleting playlist/song dirs or files
public slots:
signals:
	void playlistsChanged();
};

#endif // LIBRARY_H
