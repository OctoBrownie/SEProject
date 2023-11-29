#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include<QVBoxLayout>
#include <QLabel>

class QImage;

class PictureBox: public QVBoxLayout
{
    Q_OBJECT

public:
    //Constructor
    PictureBox(QString playlistArtPath);

signals:
    void newImagePath(QString path);

protected:
    void select_image();

    void generateImage(QString imagePath);

    QLabel* art;
};

#endif // PICTUREBOX_H
