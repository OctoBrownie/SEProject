#include "picturebox.h"

PictureBox::PictureBox(QWidget* parent): QVBoxLayout(parent)
{
    QFrame* pictureBox = new QFrame;
    pictureBox->setFixedSize(150, 150);
    pictureBox->setFrameStyle(QFrame::Box);
    pictureBox->setStyleSheet("background-color: white");

    QPushButton* photoSearch = new QPushButton("Browse Photos");
    addWidget(pictureBox);
    addWidget(photoSearch);
}
