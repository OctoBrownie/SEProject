#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

// Qt
#include<QWidget>
class QLabel;

// project
class Playlist;

class PlaylistWidget : public QWidget {
protected:
	Q_OBJECT

	// currently displayed playlist (DOESN'T manage this memory at all)
	Playlist* currPlaylist;

	// display labels for the playlist being shown
	QLabel *nameLabel, *durationLabel;

	/**
	 * Overrides the mouse click event for QWidget to register whether this
	 * widget was clicked or not.
	 */
	void mousePressEvent(QMouseEvent *event) override;

	/**
	 * Overrides the mouse double click event for QWidget to register whether this
	 * widget was clicked or not.
	 */
	void mouseDoubleClickEvent(QMouseEvent *event) override;
public:
	/**
	 * Creates a new widget to show this playlist.
	 * @param p			the playlist to display in this widget, can be null
	 * @param parent	the parent of this widget
	 */
	PlaylistWidget(Playlist* p=nullptr, QWidget* parent=nullptr);

	/**
	 * Sets the playlist to be shown in this widget.
	 * @param p	the new playlist to show in this widget
	 */
	void setPlaylist(Playlist* p);

	/**
	 * Gets the playlist currently being displayed.
	 * @param p	the currently displayed playlist, or null if none is being
	 * displayed
	 */
	Playlist* getPlaylist() { return currPlaylist; }

	/**
	 * Changes the widget to make it look selected or deselected. Doesn't store this
	 * information at all.
	 * @param s	whether the widget should be selected or not
	 */
	void setSelected(bool s);
public slots:
signals:
	void leftClicked(PlaylistWidget* widgetClicked);
	void openPlaylist(Playlist* p);
};

#endif // PLAYLISTWIDGET_H
