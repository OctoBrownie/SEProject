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

	QSizePolicy policy = QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Ignored);
	policy.setHorizontalStretch(1);
	this->setSizePolicy(policy);

	QVBoxLayout* l = new QVBoxLayout(this);
	l->setSpacing(0);
	l->addWidget(new QLabel("Playlists"));

	QScrollArea* scrollArea = new QScrollArea(this);
//	scrollArea->setFrameShape(QFrame::NoFrame);

	QWidget* scrollWidget = new QWidget(scrollArea);
	scrollWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	this->scrollLayout = new QVBoxLayout(scrollArea);
	scrollLayout->setAlignment(Qt::AlignTop);

	scrollArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Ignored);
	scrollArea->setWidget(scrollWidget);
	l->addWidget(scrollArea);

	refreshPlaylists();
}

void PlaylistContainer::refreshPlaylists() {
	// TODO: optimize with PlaylistWidget::setPlaylist()

	// delete all playlists currently in the layout
	QLayoutItem* i = this->scrollLayout->takeAt(0);
	while(i != nullptr) {
		delete i->widget();
		delete i;
		i = this->scrollLayout->takeAt(0);
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
		this->scrollLayout->addWidget(pWidget);
	}
	this->scrollLayout->addStretch();
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
