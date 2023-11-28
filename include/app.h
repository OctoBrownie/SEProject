#ifndef APP_H
#define APP_H

#include<QWidget>

class QPushButton;
class QVBoxLayout;
template <typename T> class QStack;

class App : public QWidget {
private:
    Q_OBJECT

	// bottom bar with the current song in it
	QWidget* playBar;

	// left container to show all playlists
	QWidget* playlistContainer;

	// top bar for back button, search bar, and settings button
	QWidget* topBar;

	QVBoxLayout* mainLayout;

	// current main widget
	QWidget* currMainWidget;

	// stack of all main widgets accessible via the back button
	QStack<QWidget*>* mainWidgetStack;

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
	 * left of the main screen.
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
	// virtual void keyPressEvent(QKeyEvent* event);

	bool addMainWidget(QWidget* mainWidget, bool keepStack);

public slots:
	void goBack();
signals:
};

#endif // APP_H
