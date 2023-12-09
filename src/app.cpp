#include<QWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QScrollArea>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include<QStack>

#include "app.h"
#include "library.h"
#include "song.h"
#include "playlist.h"
#include "playlistcontainer.h"
#include "playerwidget.h"
#include "musicplayer.h"
#include "equalizer.h"

App::App(QWidget *parent) : QWidget{parent} {
	setWindowState(Qt::WindowMaximized);
	setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	this->mainWidgetStack = new QStack<QWidget*>();
	this->currMainWidget = nullptr;
	this->equalizer = new Equalizer(2);
	this->musicPlayer = new MusicPlayer(equalizer, this);
	this->musicLibrary = new Library(this, musicPlayer);

	// top level layout with everything in it
	QVBoxLayout *topLayout = new QVBoxLayout(this);
	topLayout->setContentsMargins(0, 0, 0, 0);
	topLayout->setSpacing(0);

	// horizontal widget/layout for the playlists and search/main widget
	QWidget* hWidget = new QWidget(this);
	QHBoxLayout* hLayout = new QHBoxLayout(hWidget);
	hLayout->setSpacing(0);
	hLayout->setContentsMargins(0, 0, 0, 0);

	QWidget* vWidget = createMainContainer(hWidget);	// search/main widget container
	this->playlistContainer = new PlaylistContainer(this, musicLibrary, hWidget);	// just playlists
	this->playBar = new PlayerWidget(musicPlayer, this);	// active song (+ play/pause)

	// TODO: create blank main widget?
	// TODO: load last opened playlist from previous session
	QWidget* mainWidget = new QLabel("Open a playlist from the pane on the left or search for a song using the search bar above!");
	addMainWidget(mainWidget, false);

	hLayout->addWidget(playlistContainer);
	hLayout->addWidget(vWidget);

	topLayout->addWidget(hWidget);
	topLayout->addWidget(this->playBar);
//	topLayout->addWidget(new MusicPlayer());
}

App::~App() {
	delete this->mainWidgetStack;
	delete this->equalizer;
}

QWidget* App::createTopBar(QWidget* parent) {
	QWidget* bar = new QWidget(parent);

	QSizePolicy policy = QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	policy.setVerticalStretch(1);
	bar->setSizePolicy(policy);

	QHBoxLayout* layout = new QHBoxLayout(bar);
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	// size policy for buttons
	policy = QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	policy.setVerticalStretch(1);

	// back button
	QPushButton* b = new QPushButton(bar);
	b->setIcon(QIcon(":/icons/back.svg"));
	b->setSizePolicy(policy);
	layout->addWidget(b);
	connect(b, SIGNAL (clicked()), this, SLOT (goBack()));
	// TODO: make button keep aspect ratio and scale based on available space
	// see https://stackoverflow.com/questions/42833511/qt-how-to-create-image-that-scale-with-window-and-keeps-aspect-ratio
	// or https://stackoverflow.com/questions/8211982/qt-resizing-a-qlabel-containing-a-qpixmap-while-keeping-its-aspect-ratio

	// TODO: make button not pressable (or hidden) if there's no previous to go back to

	this->searchLineEdit = new QLineEdit(bar);
	searchLineEdit->setPlaceholderText("Search library");
	searchLineEdit->setGeometry(10, 10, 80, 30);
	layout->addWidget(searchLineEdit);
	connect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT(searchLibrary()));

	// settings button
	b = new QPushButton(bar);
	b->setIcon(QIcon(":/icons/settings.svg"));
	b->setSizePolicy(policy);
	layout->addWidget(b);
	// TODO: open settings from this button

	connect(b, SIGNAL(clicked()), this, SLOT(makeEqualizer()));

	return bar;
}

void App::saveEqualizerSettings() {
	this->equalizer->setLowMult(this->low->text().toFloat());
	this->equalizer->setMidMult(this->mid->text().toFloat());
	this->equalizer->setHighMult(this->high->text().toFloat());
}

void App::makeEqualizer() {
	QWidget* w = new QWidget(this);
	QVBoxLayout* l = new QVBoxLayout(w);

	this->low = new QLineEdit(w);
	this->low->setPlaceholderText("low");
	connect(this->low, SIGNAL(returnPressed()), this, SLOT(saveEqualizerSettings()));
	l->addWidget(this->low);

	this->mid = new QLineEdit(w);
	this->mid->setPlaceholderText("mid");
	connect(this->mid, SIGNAL(returnPressed()), this, SLOT(saveEqualizerSettings()));
	l->addWidget(this->mid);

	this->high = new QLineEdit(w);
	this->high->setPlaceholderText("high");
	connect(this->high, SIGNAL(returnPressed()), this, SLOT(saveEqualizerSettings()));
	l->addWidget(this->high);
	l->addStretch();

	w->setLayout(l);
	addMainWidget(w);

}

QWidget* App::createPlayBar(QWidget* parent) {
	QWidget* bar = new QWidget(parent);

	QSizePolicy policy = QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
	policy.setRetainSizeWhenHidden(false);
	bar->setSizePolicy(policy);

	QHBoxLayout* layout = new QHBoxLayout(bar);
	layout->setContentsMargins(0, 0, 0, 0);
//	layout->setSpacing(0);

	// TODO: connect this stuff to a signal from music player
	QLabel* art = new QLabel(bar);
	QSize changedSize(100,100);
	QPixmap defaultImage(":/images/noAlbumArt.svg");
	QPixmap map = defaultImage.scaled(changedSize, Qt::KeepAspectRatio);
	art->setPixmap(map);
	layout->addWidget(art);

	QLabel* songName = new QLabel("artist - song", bar);
	layout->addWidget(songName);
	layout->addStretch();

	// size policy for buttons
	policy = QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	policy.setVerticalStretch(1);

	// rewind button
	QPushButton* b = new QPushButton(bar);
	b->setIcon(QIcon(":/icons/rewind.svg"));
	b->setSizePolicy(policy);
	layout->addWidget(b);

	// play/pause button
	// TODO: swap play/pause icons
	b = new QPushButton(bar);
	b->setIcon(QIcon(":/icons/play.svg"));
	b->setSizePolicy(policy);
	layout->addWidget(b);

	// skip button
	b = new QPushButton(bar);
	b->setIcon(QIcon(":/icons/skip.svg"));
	b->setSizePolicy(policy);
	layout->addWidget(b);

	return bar;
}

QWidget* App::createMainContainer(QWidget* parent) {
	QWidget* container = new QWidget(parent);
	container->setStyleSheet(QString("background:#eee;"));

	QSizePolicy policy = QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	policy.setHorizontalStretch(3);
	container->setSizePolicy(policy);

	this->mainLayout = new QVBoxLayout(container);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	this->topBar = createTopBar(container);
	this->mainLayout->addWidget(this->topBar);

	return container;
}

bool App::addMainWidget(QWidget* mainWidget, bool keepStack, bool hidePlayBar) {
	if (mainWidget == nullptr || mainWidgetStack->contains(mainWidget) || mainWidget == currMainWidget)
		return false;

	QSizePolicy policy = mainWidget->sizePolicy();
	policy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
	policy.setVerticalPolicy(QSizePolicy::MinimumExpanding);
	policy.setVerticalStretch(10);
	mainWidget->setSizePolicy(policy);

	if (keepStack && this->currMainWidget != nullptr) {
		// push the previous main widget onto the stack
		mainWidgetStack->push(this->currMainWidget);
		this->currMainWidget->hide();
	}
	else {
		// clear the stack and free all memory within
		QWidget* temp;
		while (!mainWidgetStack->isEmpty()) {
			temp = mainWidgetStack->pop();
			this->mainLayout->removeWidget(temp);
			delete temp;
		}
		if (this->currMainWidget != nullptr) {
			this->mainLayout->removeWidget(this->currMainWidget);
			delete this->currMainWidget;
		}
	}

	this->mainLayout->addWidget(mainWidget);
	this->currMainWidget = mainWidget;

	if (hidePlayBar) this->playBar->hide();
	else this->playBar->show();

	return true;
}

void App::goBack() {
	if (mainWidgetStack->isEmpty()) return;

	mainLayout->removeWidget(this->currMainWidget);
	delete this->currMainWidget;

	this->currMainWidget = mainWidgetStack->pop();
	this->currMainWidget->show();
}

void App::openPlaylist(Playlist* p) {
	if (p == nullptr) return;
	addMainWidget(p->getListGUI());
}

void App::searchLibrary() {
	QVector<Song*>* results = this->musicLibrary->search(searchLineEdit->text());

	QScrollArea* w = new QScrollArea(this);
	w->setFrameShape(QFrame::NoFrame);
	QWidget* innerWidget = new QWidget(w);

	QVBoxLayout* layout = new QVBoxLayout(innerWidget);
	for (Song* s : *results) {
		if (s != nullptr) layout->addWidget(s);
	}
	layout->addStretch();
	w->setWidget(innerWidget);

	addMainWidget(w);
}


