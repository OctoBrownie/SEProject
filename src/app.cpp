#include<QWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QScrollArea>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include<QStack>

#include "app.h"

App::App(QWidget *parent) : QWidget{parent} {
	setWindowState(Qt::WindowMaximized);
	setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	this->mainWidgetStack = new QStack<QWidget*>();
	this->currMainWidget = nullptr;

	// top level layout with everything in it
	QVBoxLayout *topLayout = new QVBoxLayout(this);
	topLayout->setContentsMargins(0, 0, 0, 0);
	topLayout->setSpacing(0);

	// horizontal widget/layout for the playlists and search/main widget
	QWidget* hWidget = new QWidget(this);
	QHBoxLayout* hLayout = new QHBoxLayout(hWidget);
	hLayout->setSpacing(0);
	hLayout->setContentsMargins(0, 0, 0, 0);

	// vertical widget/layout for the search and main widget
	QWidget* vWidget = createMainContainer(hWidget);

	this->playlistContainer = createPlaylistContainer(hWidget);
	this->playBar = createPlayBar(this);

	// TODO: create blank main widget?
	// TODO: load last opened playlist from previous session
	QWidget* mainWidget = new QLabel("\"main widget\"");
	mainWidget->setStyleSheet(QString("background:#caf;"));
	addMainWidget(mainWidget, false);

	hLayout->addWidget(this->playlistContainer);
	hLayout->addWidget(vWidget);

	topLayout->addWidget(hWidget);
	topLayout->addWidget(this->playBar);
//	topLayout->addWidget(new MusicPlayer());
}

App::~App() {
	delete this->mainWidgetStack;
}

QWidget* App::createTopBar(QWidget* parent) {
	QWidget* bar = new QWidget(parent);
	bar->setStyleSheet(QString("background:#fcc;"));

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

	QLineEdit* search = new QLineEdit(bar);
	search->setPlaceholderText("Search library");
	search->setGeometry(10, 10, 80, 30);
	layout->addWidget(search);
	// TODO: register with the returnPressed signal

	// settings button
	b = new QPushButton(bar);
	b->setIcon(QIcon(":/icons/settings.svg"));
	b->setSizePolicy(policy);
	layout->addWidget(b);
	// TODO: open settings from this button

	return bar;
}

QWidget* App::createPlayBar(QWidget* parent) {
	QWidget* bar = new QWidget(parent);
	bar->setStyleSheet(QString("background:#ccf;"));

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

QWidget* App::createPlaylistContainer(QWidget* parent) {
	QWidget* container = new QWidget(parent);
	container->setStyleSheet(QString("background:#cfe;"));

	QSizePolicy policy = QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	policy.setHorizontalStretch(1);
	container->setSizePolicy(policy);

	QVBoxLayout* layout = new QVBoxLayout(container);
	layout->setSpacing(0);

	layout->addWidget(new QLabel("Playlists"));

	// TODO: Give scrollLayout to Library to populate with clickable playlists
	QScrollArea* scrollArea = new QScrollArea();
	scrollArea->setFrameShape(QFrame::NoFrame);

	QVBoxLayout* scrollLayout = new QVBoxLayout(scrollArea);
	policy = QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	scrollArea->setSizePolicy(policy);

	scrollLayout->addWidget(new QLabel("p1"));
	scrollLayout->addWidget(new QLabel("p1"));
	scrollLayout->addWidget(new QLabel("p1"));
	scrollLayout->addWidget(new QLabel("p1"));
	scrollLayout->addStretch();

	layout->addWidget(scrollArea);

	return container;
}

QWidget* App::createMainContainer(QWidget* parent) {
	QWidget* container = new QWidget(parent);
	container->setStyleSheet(QString("background:#eee;"));

	QSizePolicy policy = QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	policy.setHorizontalStretch(3);
	container->setSizePolicy(policy);

	this->mainLayout = new QVBoxLayout(container);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	this->topBar = createTopBar(container);
	this->mainLayout->addWidget(this->topBar);

	return container;
}

bool App::addMainWidget(QWidget* mainWidget, bool keepStack) {
	if (mainWidget == nullptr) return false;

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

	return true;
}

void App::goBack() {
	if (mainWidgetStack->isEmpty()) return;

	mainLayout->removeWidget(this->currMainWidget);
	delete this->currMainWidget;

	this->currMainWidget = mainWidgetStack->pop();
	this->currMainWidget->show();
}


