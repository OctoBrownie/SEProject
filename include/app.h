#ifndef APP_H
#define APP_H

// Qt
#include<QWidget>
class QPushButton;
class QVBoxLayout;
template <typename T> class QStack;

// project
class Library;
class Playlist;

/**
 * Is the top-level widget that holds all other widgets of the application.
 * Manages the GUI of the application and interfaces with the other
 * components of the application to make stuff happen.
 *
 * Some parts of the GUI are static (the playlist list is always there) but
 * some are dynamic (the play bar should only be shown when a song is actively
 * being played and the main widget isn't displaying the current song).
 */
class App : public QWidget {
protected:
    Q_OBJECT

	// bottom bar with the current song in it
	QWidget* playBar;

	// scrolling area for clickable playlists
	QWidget* playlistContainer;
	QVBoxLayout* playlistLayout;

	// top bar for back button, search bar, and settings button
	QWidget* topBar;

	// layout to add main widgets to
	QVBoxLayout* mainLayout;

	// current main widget
	QWidget* currMainWidget;

	// stack of all main widgets accessible via the back button
	QStack<QWidget*>* mainWidgetStack;

	// contains the entire searchable music library
	Library* musicLibrary;



	/**
	 * Creates the top navigation bar (which includes the back button,
	 * search bar, and settings). Does not add it to the layout.
	 * @param parent	the parent widget of this new widget
	 * @return a new navigation bar widget with the back button, search bar, and
	 * settings button
	 */
	QWidget* createTopBar(QWidget* parent=nullptr);

	/**
	 * Creates the bottom bar for music playback (includes album art, artist name,
	 * song name, and playback controls). Does not add it to the layout.
	 * @param parent	the parent widget of this new widget
	 * @return a new play bar widget with placeholders for song data
	 */
	QWidget* createPlayBar(QWidget* parent=nullptr);

	/**
	 * Creates the playlist container, which displays clickable playlists to the
	 * left of the main screen. Also initializes this->playlistLayout.
	 * @param parent	the parent widget of this new widget
	 * @return a new playlist container with title and scroll area initialized
	 */
	QWidget* createPlaylistContainer(QWidget* parent=nullptr);

	/**
	 * Creates the main container of the GUI, which contains the search bar layout
	 * (the top bar) and the swappable main widget. Also initializes this->topBar and
	 * this->mainLayout (doesn't check if either have been initialized yet).
	 * @param parent	the parent widget of this new widget
	 * @return a new main container with search bar initialized but no main widget
	 */
	QWidget* createMainContainer(QWidget* parent=nullptr);
public:
	explicit App(QWidget *parent = nullptr);
	virtual ~App();

	bool addMainWidget(QWidget* mainWidget, bool keepStack=true, bool hidePlayBar=false);
	Library* getLibrary() { return musicLibrary; }

public slots:
	void goBack();
	void openPlaylist(Playlist* p);
signals:
};

#endif // APP_H
