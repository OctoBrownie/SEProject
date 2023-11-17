#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QLabel>

class Playlist;

class PictureBox: public QVBoxLayout
{
public:
    PictureBox(Playlist* playlist);
};

#endif // PICTUREBOX_H
