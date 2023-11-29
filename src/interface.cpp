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
	toolbar->addWidget(new QPushButton("Save Playlist"));
	toolbar->addWidget(new QPushButton("Export Playlist"));
    toolbar->addWidget(settingsButton);

	return toolbar;
}

QHBoxLayout* Interface::createPlaylistToolbar(QWidget* parent, Playlist* playlist) {
	QHBoxLayout* toolbar = new QHBoxLayout(parent);

    QPushButton* addButton = new QPushButton("Add to Playlist");
    connect(addButton, &QPushButton::clicked, playlist, [=]() {
        Song* newSong = new Song("C:/Users/astro/Downloads/Fluffing-a-Duck(chosic.com).mp3");
        playlist->addSong(newSong);
    });
    toolbar->addWidget(addButton);
	toolbar->addWidget(new QPushButton("Delete Selected"));
	toolbar->addWidget(new QPushButton("Search"));

	return toolbar;
}

