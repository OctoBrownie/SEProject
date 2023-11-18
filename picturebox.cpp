#include "picturebox.h"

//Constructor
PictureBox::PictureBox(QImage* playlistArt): QVBoxLayout()
{
    //Create a label, set the size, and create a PixMap
    QLabel* art = new QLabel();
    QSize changedSize(150,150);
    QPixmap map;

    //If the playlistArt is not NULL, create the map, else use a generic image
    if (!playlistArt->isNull()) {
        map = QPixmap::fromImage(*playlistArt).scaled(changedSize, Qt::KeepAspectRatio);
    } else {
        QPixmap defaultImage(":/resources/images/NoPlaylistImage.png");
        map = defaultImage.scaled(changedSize, Qt::KeepAspectRatio);
    }

    //Set the label to have the PixMap
    art->setPixmap(map);

    //Photo search button
    QPushButton* photoSearch = new QPushButton("Browse Photos");

    //Add both widgets
    addWidget(art);
    addWidget(photoSearch);
}
