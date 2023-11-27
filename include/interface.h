#ifndef INTERFACE_H
#define INTERFACE_H

// Qt
#include<QMainWindow>
class QHBoxLayout;

// project
class Playlist;

class Interface : public QMainWindow {
public:
    Interface(QWidget* centralWidget, QDockWidget* dockedWidget, QWidget* parent=nullptr);

	static QHBoxLayout* createMainToolbar(Playlist* p, QWidget* parent=nullptr);
	static QHBoxLayout* createPlaylistToolbar(Playlist* p, QWidget* parent=nullptr);
};

#endif // INTERFACE_H
