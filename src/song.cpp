//QtLibraries
#include<QBoxLayout>
#include<QLabel>
#include<QPushButton>
#include <QFile>


//TagLib Libraries
#include<taglib/tag.h>
#include<taglib/fileref.h>
#include<taglib/mpegfile.h>
#include<taglib/id3v2tag.h>
#include<taglib/id3v2frame.h>
#include<taglib/attachedpictureframe.h>

#include "song.h"

//Take in the QString path to a song. This input can be changed at anytime.
Song::Song(QString path): QWidget()
{
    this->songPath = path;

    //Open the file path, by turning the QString into a char*
    TagLib::MPEG::File file(path.toStdString().c_str());

    //If a valid file (MP3 v2, the standard since 2000), continue.
    if (file.hasID3v2Tag()) {
        //Create a tag (a special object holding all the data)
        TagLib::ID3v2::Tag *tag = file.ID3v2Tag();

        //Get title, artists, album, and duration. TagLib only returns StdStrings, and tag->title() needs to be reformatted into becoming a std::String
        this->title = QString::fromStdString(tag->title().to8Bit(true));
        qDebug() << "This is the title: " << this->title;
        if (this->title.isNull()) {
            this->title = this->songPath;
        }

        this->artists = QString::fromStdString(tag->artist().to8Bit(true));
        if (this->artists.isNull()) {
            this->artists = "No Artist";
        }

        this->album = QString::fromStdString(tag->album().to8Bit(true));
        if (this->album.isNull()) {
            this->album = "No Album";
        }

        this->duration = file.audioProperties()->lengthInSeconds();

        //Attempt to see if the associated image exists.
        TagLib::ID3v2::FrameList frameList = tag->frameList("APIC");

        //If this image is accessible
        if (!frameList.isEmpty()) {
            //Turn into an image.
            TagLib::ID3v2::AttachedPictureFrame *pictureFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frameList.front());

            //If image successful,
            if (pictureFrame) {
                //Save and create the album art
                QImage albumArt;
                albumArt.loadFromData(reinterpret_cast<const uchar*>(pictureFrame->picture().data()), pictureFrame->picture().size());
                this->albumArt = albumArt;
            }
        }
    } else {
        this->title = this->songPath;
        this->artists = "No Artist";
        this->album = "No Album";
    }

    //Create the widget that represents the song object, and save to this->widget
    makeLayout();

    //Set the layout and add a widget to this layout.
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(this->widget);

    //Set the layout of the song Widget to this new layout, so it stores it's GUI element.
    this->setLayout(layout);
    this->setFixedHeight(150);
}

//Public facing version of the protected mousePressEvent method
void Song::triggerMousePressEvent() {
    this->mousePressEvent(nullptr);
}

/*

    Getters

*/

QString Song::getSongTitle() {
    return this->title;
}

QString Song::getArtist() {
    return this->artists;
}

QString Song::getAlbum() {
    return this->album;
}

QImage* Song::getArt() {
    return &this->albumArt;
}

QString Song::getSongPath() {
    return this->songPath;
}

qint64 Song::getDuration() {
    return this->duration;
}

qint64 Song::getPosition() {
    return this->pPosition;
}

QPushButton* Song::getUpButton() {
    return this->upButton;
}

QPushButton* Song::getDownButton() {
    return this->downButton;
}

bool Song::getSelected() {
    return this->selected;
}



/*

    Setters

*/


//Set the position of the song in the playlist it is stored in.
void Song::setPosition(qint64 newPosition) {
    this->pPosition = newPosition;
}

//Set the value of the selected variable of the song, based on whether it is clicked or not.
void Song::setSelected(bool selected) {
    this->selected = selected;

    // Set background color for the entire widget and its child widgets
    QString backgroundColor = (this->selected == true) ? "#73b2e4" : "#F0F0F0";
    this->widget->setStyleSheet(QString("background-color: %1").arg(backgroundColor));

    emit cloneHelper(selected);
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
    if (!this->albumArt.isNull()) {
        map = QPixmap::fromImage(this->albumArt).scaled(changedSize, Qt::KeepAspectRatio);
    } else {
        //Use the default image, which is stored in the resources
        QPixmap defaultImage(":/resources/images/DefaultMusicImage.svg");
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
    const QIcon upIcon(":/resources/icons/upButton.svg");
    const QIcon downIcon(":/resources/icons/downButton.svg");

    //Set the buttons to have the icons and the fixed widths and heights
    upButton->setIcon(upIcon);
    upButton->setFixedWidth(100);
    upButton->setFixedHeight(30);

    downButton->setIcon(downIcon);
    downButton->setFixedWidth(100);
    downButton->setFixedHeight(30);

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

    this->widget->setLayout(outerLayout);
}

//What to do when the mouse is clicked
void Song::mousePressEvent(QMouseEvent *event) {
    //If the Song is already selected, unselect it, and emit a signal to tell the playlist that it no longer has a selected song. Send the position of the song, so the playlist knows what to do.
    if (this->selected == true) {
        this->setSelected(false);
        emit selectedSong(this->pPosition, false);

    //If the Song is not already selected, select it, and emit a signal to tell the playlist to select this song instead. Send the position of the song, so the playlist knows what to do.
    } else {
        this->setSelected(true);
        emit selectedSong(this->pPosition, false);
    }
}

