//QtLibraries
#include<QBoxLayout>
#include<QLabel>
#include<QPushButton>
#include<QMouseEvent>


//TagLib Libraries
#include<taglib/tag.h>
#include<taglib/fileref.h>
#include<taglib/mpegfile.h>
#include<taglib/id3v2tag.h>
#include<taglib/id3v2frame.h>
#include<taglib/attachedpictureframe.h>

#include "song.h"

//Take in the QString path to a song. This input can be changed at anytime.
Song* Song::createSong(QString path, QWidget* parent) {
	Song* song = new Song(parent);
	song->songPath = path;
	song->albumArt = nullptr;

	//Open the file path, by turning the QString into a char*
	TagLib::MPEG::File file(path.toStdString().c_str());

	//If a valid file (MP3 v2, the standard since 2000), continue.
	if (file.isValid() && file.hasID3v2Tag()) {
		//Create a tag (a special object holding all the data)
		TagLib::ID3v2::Tag *tag = file.ID3v2Tag();

		//Get title, artists, album, and duration. TagLib only returns StdStrings, and tag->title() needs to be reformatted into becoming a std::String
		song->title = QString::fromStdString(tag->title().to8Bit(true));
		song->artists = QString::fromStdString(tag->artist().to8Bit(true));
		song->album = QString::fromStdString(tag->album().to8Bit(true));
		song->duration = file.audioProperties()->lengthInSeconds();

		//Attempt to see if the associated image exists.
		TagLib::ID3v2::FrameList frameList = tag->frameList("APIC");

		//If this image is accessible
		if (!frameList.isEmpty()) {
			//Turn into an image.
			TagLib::ID3v2::AttachedPictureFrame *pictureFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frameList.front());

			//If image successful,
			if (pictureFrame) {
				//Save and create the album art
				song->albumArt = new QImage();
				song->albumArt->loadFromData(reinterpret_cast<const uchar*>(pictureFrame->picture().data()), pictureFrame->picture().size());
			}
		}
	}
	else {
		// file doesn't exist or was invalid
		delete song;
		return nullptr;
	}

	//Create the widget that represents the song object, and save to this->widget
	song->makeLayout();

	//Set the layout and add a widget to this layout.
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(song->widget);

	//Set the layout of the song Widget to this new layout, so it stores it's GUI element.
	song->setLayout(layout);
	// song->setFixedHeight(150);

	return song;
}

Song::Song(QWidget* parent): QWidget(parent) {}



/*

    Getters

*/

QString Song::getSongTitle() const {
    return this->title;
}

QString Song::getArtist() const {
    return this->artists;
}

QString Song::getAlbum() const {
    return this->album;
}

QImage* Song::getArt() const {
	return this->albumArt;
}

QString Song::getSongPath() const {
    return this->songPath;
}

qint64 Song::getDuration() const {
    return this->duration;
}

qint64 Song::getPosition() const {
    return this->pPosition;
}

QPushButton* Song::getUpButton() {
    return this->upButton;
}

QPushButton* Song::getDownButton() {
    return this->downButton;
}



/*

    Setters

*/


//Set the position of the song in the playlist it is stored in.
void Song::setPosition(qint64 newPosition) {
    this->pPosition = newPosition;
}

//Set the value of the selected variable of the song, based on whether it is clicked or not.
void Song::setSelected(bool value) {
    this->selected = value;

    // Set background color for the entire widget and its child widgets
    QString backgroundColor = (this->selected == 1) ? "#73b2e4" : "#F0F0F0";
    this->widget->setStyleSheet(QString("background-color: %1").arg(backgroundColor));
}



/*

    Protected Methods

*/


//Make the layout; Used to set the layout of this widget
void Song::makeLayout() {
	//Create the outer layout (has two halves; image on the left, text on the right)
	QHBoxLayout* outerLayout = new QHBoxLayout();

	//Create the inner layout (has three levels, title, artist, album, text, going from top to bottom)
	QVBoxLayout* innerLayout = new QVBoxLayout();

	QLabel* titleText = new QLabel(this->title);
	QLabel* artistText = new QLabel(this->artists);
	QLabel* albumTest = new QLabel(this->album);

	//Add all of the widgets to the vertical, internal layout
	innerLayout->addWidget(titleText);
	innerLayout->addWidget(artistText);
	innerLayout->addWidget(albumTest);

	//Create the label and set size
	QLabel* art = new QLabel();
	QSize changedSize(100,100);
	QPixmap map;

	//If the this->albumArt exists, the MP3 stores an image, then get the PixMap from the Image
	if (this->albumArt != nullptr && !this->albumArt->isNull()) {
		map = QPixmap::fromImage(*this->albumArt).scaled(changedSize, Qt::KeepAspectRatio);
	} else {
		//Use the default image, which is stored in the resources
		QPixmap defaultImage(":/images/noAlbumArt.svg");
		map = defaultImage.scaled(changedSize, Qt::KeepAspectRatio);
	}

	//Set the QLabel to display the chosen Pixmap
	art->setPixmap(map);

	//Make a spacer
	QSpacerItem* spacer1 = new QSpacerItem(40, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	QSpacerItem* spacer2 = new QSpacerItem(40, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

	//Make the two movement button
	QVBoxLayout* buttons = new QVBoxLayout();
	QPushButton* upButton = new QPushButton();
	QPushButton* downButton = new QPushButton();

	//Set the icons
	const QIcon upIcon(":/icons/up button.svg");
	const QIcon downIcon(":/icons/down button.svg");

	//Set the buttons to have the icons and the fixed widths and heights
	upButton->setIcon(upIcon);
//	upButton->setFixedWidth(100);
//	upButton->setFixedHeight(30);

	downButton->setIcon(downIcon);
//	downButton->setFixedWidth(100);
//	downButton->setFixedHeight(30);

	//Assign to the stored class value
	this->upButton = upButton;
	this->downButton = downButton;

	//Add to the buttons widget, so it is one on top of the other
	buttons->addWidget(upButton);
	buttons->addWidget(downButton);

	//If the upButton is pressed, send out the buttonClicked signal with the position, and 1, for up
	connect(upButton, &QPushButton::clicked, this, [=]() {
		emit buttonClicked(this->pPosition, 1);
	});

	//If the downButton is pressed, send out the buttonClicked signal with position and 0, for down
	connect(downButton, &QPushButton::clicked, this, [=]() {
		emit buttonClicked(this->pPosition, 0);
	});

	//Add horizontal items and align to the left
	outerLayout->addWidget(art);
	outerLayout->addSpacerItem(spacer1);
	outerLayout->addLayout(innerLayout);
	outerLayout->addSpacerItem(spacer2);
	outerLayout->addLayout(buttons);
	outerLayout->setAlignment(Qt::AlignLeft);

	//Set this->widget to be a widget with the created layout.
	this->widget = new QWidget();
	this->widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	this->widget->setLayout(outerLayout);
}

//What to do when the mouse is clicked
void Song::mousePressEvent(QMouseEvent *event) {
	this->setSelected(!this->selected);
	emit selectedSong(this->pPosition, this->selected);
}

void Song::mouseDoubleClickEvent(QMouseEvent *event) {
	this->setSelected(false);
	if (event->button() == Qt::LeftButton) {
		// should start playing the song
		emit playSong(this);
	}
}
