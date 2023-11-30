#include<QWidget>
#include<QDockWidget>
#include<QHBoxLayout>
#include<QPushButton>

#include "interface.h"

Interface::Interface(QWidget* centralWidget, QDockWidget* dockedWidget, QWidget* parent)
	: QMainWindow(parent) {

	setCentralWidget(centralWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockedWidget);
    resize(1500, 1000);
}

QHBoxLayout* Interface::createMainToolbar(Playlist* playlist, SettingsWindow* settings)
{
    QHBoxLayout* toolbar = new QHBoxLayout();

    QPushButton* settingsButton = new QPushButton("Settings");
    connect(settingsButton, &QPushButton::clicked, settings, &QMainWindow::show);

    toolbar->addWidget(new QPushButton("Import Playlist"));
    QPushButton* saveButton = new QPushButton("Save Playlist");

    connect(saveButton, &QPushButton::clicked, [=] {
        this->savePlaylist(playlist);
    });

    toolbar->addWidget(saveButton);

    QPushButton* exportButton = new QPushButton("Export Playlist");
    connect(exportButton, &QPushButton::clicked, [=] {
        this->exportPlaylist(playlist);
    });
    toolbar->addWidget(exportButton);
    toolbar->addWidget(settingsButton);

	return toolbar;
}

QHBoxLayout* Interface::createPlaylistToolbar(Playlist* playlist, QWidget* parent) {
	QHBoxLayout* toolbar = new QHBoxLayout(parent);

    QPushButton* addButton = new QPushButton("Add to Playlist");
    connect(addButton, &QPushButton::clicked, playlist, [=]() {
        Song* newSong = new Song("C:/Users/astro/Downloads/Fluffing-a-Duck(chosic.com).mp3");
        playlist->addSong(newSong);
    });
    toolbar->addWidget(addButton);

    QPushButton* deleteButton = new QPushButton("Delete Selected");
    connect(deleteButton, &QPushButton::clicked, [=] {
        playlist->removeSong(playlist->getSelectedSong());
        playlist->setSelectedSong(-1, false);
    });
    toolbar->addWidget(deleteButton);

    SearchWindow* searchWindow = new SearchWindow(playlist, parent);

    QPushButton* searchButton = new QPushButton("Search");
    connect(searchButton, &QPushButton::clicked, searchWindow, [=] {
        searchWindow->show();
        searchWindow->query();
    });

    toolbar->addWidget(searchButton);

	return toolbar;
}


void Interface::savePlaylist(Playlist* playlist) {
    if (*playlist->getPlaylistPath() == "") {
        QMessageBox::critical(nullptr, "Error", "Please Export Before Saving! This will set the spot where the playlist will be saved!");
    } else {
        playlist->savePlaylist(playlist->getPlaylistPath());
    }
}

void Interface::exportPlaylist(Playlist* playlist)
{
    QMainWindow* newWindow = new QMainWindow();
    QFileDialog dialog(newWindow);
    dialog.setNameFilter("Playlist Files (*.pa)");

    // Open the dialog and get the selected file path
    QString selectedFilePath = dialog.getSaveFileName(newWindow, "Select an image file", QString(), "Playlist Files (*.pa)");

    if(!selectedFilePath.isEmpty()) {
        if(!selectedFilePath.endsWith(".pa", Qt::CaseInsensitive)) {
            selectedFilePath += ".pa";
        }
    }

    playlist->setPlaylistPath(selectedFilePath);
    playlist->savePlaylist(playlist->getPlaylistPath());
}
