#ifndef PLAYLISTCONTAINER_H
#define PLAYLISTCONTAINER_H

// Qt
#include<QWidget>
#include<QVector>
class QVBoxLayout;

// project
class App;
class PlaylistWidget;
class Playlist;
class Library;

/**
 * Widget for containing PlaylistWidgets in a list. Also keeps a
 * vertical stretch after all widgets to keep them top aligned. Manages
 * which playlist is selected and updating the GUI to reflect when
 * playlists change.
 */
class PlaylistContainer : public QWidget {
protected:
	Q_OBJECT

	// handle to the application (for connecting signals/slots)
	const App* app;

	// handle to the music library (for getting playlists)
	const Library* library;

	// all playlist widgets
	QVector<PlaylistWidget*>* widgets;

	// layout for inside the scroll area (holds playlist widgets and a stretch)
	QVBoxLayout* scrollLayout;

	// index of which widget is currently selected
	qint64 selected;
public:
	explicit PlaylistContainer(App *a, Library* lib, QWidget* parent=nullptr);

public slots:
	void itemClicked(PlaylistWidget* w);
	void refreshPlaylists();
signals:
};

#endif // PLAYLISTCONTAINER_H
