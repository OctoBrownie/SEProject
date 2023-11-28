#ifndef INTERFACE_H
#define INTERFACE_H

#include<QMainWindow>
#include "playlist.h"
#include "settingswindow.h"


class QHBoxLayout;

class Interface : public QMainWindow {
public:
    Interface(QWidget* centralWidget, QDockWidget* dockedWidget, QWidget* parent=nullptr);

    static QHBoxLayout* createMainToolbar(Playlist* playlist, SettingsWindow* settings);
    static QHBoxLayout* createPlaylistToolbar(QWidget* parent=nullptr, Playlist* playlist=nullptr);
};

#endif // INTERFACE_H
