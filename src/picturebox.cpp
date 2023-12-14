#include <QPushButton>
#include <QLabel>
#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QVBoxLayout>

#include "picturebox.h"

//Constructor that inherits from QWidget
PictureBox::PictureBox(QString playlistArtPath): QWidget()
{
    //Create the playlist art
    this->art = new QLabel();
    generateImage(playlistArtPath);

    //Photo search button
    QPushButton* photoSearch = new QPushButton("Browse Photos");
    connect(photoSearch, &QPushButton::clicked, this, &PictureBox::select_image);

    //Add both widgets
    QVBoxLayout* finalLayout = new QVBoxLayout();
    finalLayout->addWidget(this->art);
    finalLayout->addWidget(photoSearch);
    this->setLayout(finalLayout);
}


//This function is a modified version of Bryan Piedra's function he created. All credit goes to him.
void PictureBox::select_image()
{
    //Create a new window, and set it to be a file dialog
    QMainWindow* newWindow = new QMainWindow();
    QFileDialog dialog(newWindow);
    dialog.setNameFilter("PNG Files (*.png);;JPG Files (*.jpg);;JPEG Files (*.jpeg)");

    // Set the dialog to select a single file
    dialog.setFileMode(QFileDialog::ExistingFile);

    // Open the dialog and get the selected file path
    QString selectedFilePath = dialog.getOpenFileName(newWindow, "Select an image file", QString(), "PNG Files (*.png);;JPG Files (*.jpg);;JPEG Files (*.jpeg)");

    //If the path in the dialog is empty ignore it, and keep the current image.
    if (selectedFilePath.isEmpty()) {
        return;
    }

    //Emit this signal to update the playlist's reference to the image
    emit newImagePath(selectedFilePath);


    //Create the new image
    generateImage(selectedFilePath);
}

void PictureBox::generateImage(QString imagePath)
{
    QImage* playlistArt = new QImage(imagePath);
    QSize changedSize(150,150);
    QPixmap map;

    //If the playlistArt is not NULL, create the map, else use a generic image
    if (!playlistArt->isNull()) {
        map = QPixmap::fromImage(*playlistArt).scaled(changedSize, Qt::KeepAspectRatio);
    } else {
        QPixmap defaultImage(":/resources/images/NoPlaylistImage.svg");
        map = defaultImage.scaled(changedSize, Qt::KeepAspectRatio);
    }

    //Set the label to have the PixMap
    this->art->setPixmap(map);
}
