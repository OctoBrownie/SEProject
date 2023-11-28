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
	bar->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum));
	bar->setStyleSheet(QString("background:#fcc;"));

	QHBoxLayout* layout = new QHBoxLayout(bar);
	layout->setSpacing(0);

	QPushButton* backButton = new QPushButton(bar);
	backButton->setIcon(QIcon(":/icons/back.svg"));
	layout->addWidget(backButton);
	// TODO: make button not pressable (or HIDDEN) if there's no previous to go back to

	QLineEdit* search = new QLineEdit(bar);
	search->setPlaceholderText("Search library");
	search->setGeometry(10, 10, 80, 30);
	layout->addWidget(search);
	// TODO: register with the returnPressed signal

	QPushButton* settingsButton = new QPushButton(bar);
	settingsButton->setIcon(QIcon(":/icons/settings.svg"));
	layout->addWidget(settingsButton);
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
	layout->setSpacing(0);

	layout->addWidget(new QLabel("play bar"));
	layout->addWidget(new QLabel("playing..."));

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
}


