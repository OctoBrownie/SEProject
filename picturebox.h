#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

class PictureBox: public QVBoxLayout
{
public:
    //Constructor
    PictureBox(QImage* playlistArt);
};

#endif // PICTUREBOX_H
