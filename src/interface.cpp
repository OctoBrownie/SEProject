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

QHBoxLayout* Interface::createMainToolbar(QWidget* parent) {
	QHBoxLayout* toolbar = new QHBoxLayout(parent);

	toolbar->addWidget(new QPushButton("Import Playlist"));
	toolbar->addWidget(new QPushButton("Save Playlist"));
	toolbar->addWidget(new QPushButton("Export Playlist"));
	toolbar->addWidget(new QPushButton("Settings"));

	return toolbar;
}

QHBoxLayout* Interface::createPlaylistToolbar(QWidget* parent) {
	QHBoxLayout* toolbar = new QHBoxLayout(parent);

	toolbar->addWidget(new QPushButton("Add to Playlist"));
	toolbar->addWidget(new QPushButton("Delete Selected"));
	toolbar->addWidget(new QPushButton("Search"));

	return toolbar;
}

