
#include "songclone.h"

#include "song.h"
#include <QBoxLayout>
#include <QLabel>

//Constructor
SongClone::SongClone(Song* song)
{
    //Create the outer layout (has two halves; image on the left, text on the right)
    QHBoxLayout* outerLayout = new QHBoxLayout();

    //Create the inner layout (has three levels, title, artist, album, text, going from top to bottom)
    QVBoxLayout* innerLayout = new QVBoxLayout();

    QLabel* titleText = new QLabel(song->getSongTitle());
    QLabel* artistText = new QLabel(song->getArtist());
    QLabel* albumTest = new QLabel(song->getAlbum());

    //Add all of the widgets to the vertical, internal layout
    innerLayout->addWidget(titleText);
    innerLayout->addWidget(artistText);
    innerLayout->addWidget(albumTest);

    //Create the label and set size
    QLabel* art = new QLabel();
    QSize changedSize(100,100);
    QPixmap map;

    //If the this->albumArt exists, the MP3 stores an image, then get the PixMap from the Image
    if (!song->getArt()->isNull()) {
        map = QPixmap::fromImage(*song->getArt()).scaled(changedSize, Qt::KeepAspectRatio);
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

    //Add horizontal items and align to the left
    outerLayout->addWidget(art);
    outerLayout->addSpacerItem(spacer1);
    outerLayout->addLayout(innerLayout);
    outerLayout->addSpacerItem(spacer2);
    outerLayout->setAlignment(Qt::AlignLeft);

    //Set this->widget to be a widget with the created layout.
    this->widget = new QWidget();

    this->widget->setLayout(outerLayout);

    //Set the layout and add a widget to this layout.
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(this->widget);

    //Set the layout of the song Widget to this new layout, so it stores it's GUI element.
    this->setLayout(layout);
    this->setFixedHeight(150);

    this->setSelected(song->getSelected());

    //When the clone is clicked, essentially cause a "click" to happen in two places
    connect(this, &SongClone::clicked, song, &Song::triggerMousePressEvent);

    //When the song is clicked, change the clone's selected status
    connect(song, &Song::cloneHelper, this, &SongClone::setSelected);
}


//Set the value of the selected variable of the song, based on whether it is clicked or not.
void SongClone::setSelected(bool selected) {
    this->selected = selected;

    // Set background color for the entire widget and its child widgets
    QString backgroundColor = (this->selected == true) ? "#73b2e4" : "#F0F0F0";
    this->widget->setStyleSheet(QString("background-color: %1").arg(backgroundColor));
}


void SongClone::mousePressEvent(QMouseEvent *event) {
    //If the Song is already selected, unselect it, and emit a signal to tell the playlist that it no longer has a selected song. Send the position of the song, so the playlist knows what to do.
    if (this->selected == true) {
        this->setSelected(false);
        emit clicked();

        //If the Song is not already selected, select it, and emit a signal to tell the playlist to select this song instead. Send the position of the song, so the playlist knows what to do.
    } else {
        this->setSelected(true);
        emit clicked();
    }
}
