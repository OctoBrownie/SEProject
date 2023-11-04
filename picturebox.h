#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>


class PictureBox: public QVBoxLayout
{
public:
    PictureBox(QWidget* parent=nullptr);
};

#endif // PICTUREBOX_H
