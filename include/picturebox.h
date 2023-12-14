#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QWidget>
class QImage;
class QLabel;

//Class that inherits frmo QWidget class
class PictureBox: public QWidget
{
    Q_OBJECT

public:
    //Constructor
    PictureBox(QString playlistArtPath);


signals:
    //Signal that mark a new image was selected
    void newImagePath(QString path);

protected:
    //Used to create a file diaglog, and select a new image file
    void select_image();

    //Create a PixMap based on an image path string
    void generateImage(QString imagePath);

    //The label that contains the image PixMap.
    QLabel* art;
};

#endif // PICTUREBOX_H
