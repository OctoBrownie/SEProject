#include "picturebox.h"
#include "playlist.h"

PictureBox::PictureBox(Playlist* playlist): QVBoxLayout()
{

    QLabel* art = new QLabel();
    QSize changedSize(150,150);
    QPixmap map;
    QImage* playlistArt = playlist->getImage();

    if (!playlistArt->isNull()) {
        map = QPixmap::fromImage(*playlistArt).scaled(changedSize, Qt::KeepAspectRatio);
    } else {
        QPixmap defaultImage(":/resources/images/NoPlaylistImage.png");
        map = defaultImage.scaled(changedSize, Qt::KeepAspectRatio);
    }
    art->setPixmap(map);

    QPushButton* photoSearch = new QPushButton("Browse Photos");
    addWidget(art);
    addWidget(photoSearch);
}
