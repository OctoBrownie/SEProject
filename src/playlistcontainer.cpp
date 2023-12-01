// Qt
#include<QScrollArea>
#include<QVBoxLayout>
#include<QLabel>

// project
#include "playlistcontainer.h"
#include "playlistwidget.h"
#include "app.h"
#include "library.h"

PlaylistContainer::PlaylistContainer(App *a, Library* lib, QWidget* parent)
	: QWidget{parent}, app{a}, library{lib} {
	this->widgets = new QVector<PlaylistWidget*>();
	this->selected = -1;

	QSizePolicy policy = QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	policy.setHorizontalStretch(1);
	this->setSizePolicy(policy);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setSpacing(0);

	layout->addWidget(new QLabel("Playlists"));

	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setFrameShape(QFrame::NoFrame);

	this->layout = new QVBoxLayout(scrollArea);
	policy = QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	scrollArea->setSizePolicy(policy);
	layout->addWidget(scrollArea);

	refreshPlaylists();
}

void PlaylistContainer::refreshPlaylists() {
	// TODO: optimize with PlaylistWidget::setPlaylist()

	// delete all playlists currently in the layout
	QLayoutItem* i = this->layout->takeAt(0);
	while(i != nullptr) {
		delete i->widget();
		delete i;
		i = this->layout->takeAt(0);
	}
	widgets->clear();

	// remake all playlists
	const QVector<Playlist*>* playlists = library->getPlaylists();
	PlaylistWidget* pWidget;
	for (Playlist* p : *playlists) {
		pWidget = new PlaylistWidget(p);
		widgets->append(pWidget);
		connect(pWidget, SIGNAL(leftClicked(PlaylistWidget*)), this, SLOT(itemClicked(PlaylistWidget*)));
		connect(pWidget, SIGNAL(openPlaylist(Playlist*)), app, SLOT(openPlaylist(Playlist*)));
		this->layout->addWidget(pWidget);
	}
	this->layout->addStretch();
}

void PlaylistContainer::itemClicked(PlaylistWidget* w) {
	qint64 wIndex = widgets->indexOf(w);
	if (selected >= 0) {
		// something was selected before
		(*widgets)[selected]->setSelected(false);
	}

	if (selected == wIndex) {
		// now there's nothing selected
		selected = -1;
	}
	else {
		(*widgets)[wIndex]->setSelected(true);
		selected = wIndex;
	}
}
