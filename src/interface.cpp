#include<QWidget>
#include<QDockWidget>
#include<QHBoxLayout>
#include<QPushButton>

#include "playlist.h"
#include "interface.h"

Interface::Interface(QWidget* centralWidget, QDockWidget* dockedWidget, QWidget* parent)
	: QMainWindow(parent) {

	setCentralWidget(centralWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockedWidget);
    resize(1500, 1000);
}

QHBoxLayout* Interface::createMainToolbar(Playlist* p, QWidget* parent) {
	QHBoxLayout* toolbar = new QHBoxLayout(parent);

	QPushButton* b = new QPushButton("Import Playlist");
	toolbar->addWidget(b);

	b = new QPushButton("Save Playlist");
	connect(b, SIGNAL (clicked()), p, SLOT (save()));
	toolbar->addWidget(b);

	b = new QPushButton("Export Playlist");
	toolbar->addWidget(b);

	b = new QPushButton("Settings");
	toolbar->addWidget(b);

	return toolbar;
}

QHBoxLayout* Interface::createPlaylistToolbar(Playlist* p, QWidget* parent) {
	QHBoxLayout* toolbar = new QHBoxLayout(parent);

	QPushButton* b = new QPushButton("Add to Playlist");
	toolbar->addWidget(b);

	b = new QPushButton("Delete Selected");
	connect(b, SIGNAL (clicked()), p, SLOT (removeSong()));
	toolbar->addWidget(b);

	b = new QPushButton("Search");
	toolbar->addWidget(b);

	return toolbar;
}

