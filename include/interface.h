#ifndef INTERFACE_H
#define INTERFACE_H

#include<QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include "playlist.h"
#include "settingswindow.h"
#include "searchwindow.h"


class QHBoxLayout;

class Interface : public QMainWindow {
public:
    Interface(QWidget* centralWidget, QDockWidget* dockedWidget, QWidget* parent=nullptr);

    QHBoxLayout* createMainToolbar(Playlist* playlist, SettingsWindow* settings);
    QHBoxLayout* createPlaylistToolbar(Playlist* playlist=nullptr, QWidget* parent=nullptr);

protected:
    void savePlaylist(Playlist* playlist);
    void exportPlaylist(Playlist* playlist);
};

#endif // INTERFACE_H
