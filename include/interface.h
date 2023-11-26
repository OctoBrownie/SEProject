#ifndef INTERFACE_H
#define INTERFACE_H

#include<QMainWindow>

class QHBoxLayout;

class Interface : public QMainWindow {
public:
    Interface(QWidget* centralWidget, QDockWidget* dockedWidget, QWidget* parent=nullptr);

	static QHBoxLayout* createMainToolbar(QWidget* parent=nullptr);
	static QHBoxLayout* createPlaylistToolbar(QWidget* parent=nullptr);
};

#endif // INTERFACE_H
