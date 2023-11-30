#include<QPushButton>
#include<QLabel>
#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

#include "picturebox.h"

//Constructor
PictureBox::PictureBox(QString playlistArtPath): QVBoxLayout()
{
    this->art = new QLabel();
    generateImage(playlistArtPath);

    //Photo search button
    QPushButton* photoSearch = new QPushButton("Browse Photos");
    connect(photoSearch, &QPushButton::clicked, this, &PictureBox::select_image);

    //Add both widgets
    addWidget(this->art);
    addWidget(photoSearch);
}


//This function is a modified version of Bryan Piedra's function he created. All credit goes to him.
void PictureBox::select_image()
{
    QMainWindow* newWindow = new QMainWindow();
    QFileDialog dialog(newWindow);
    dialog.setNameFilter("PNG Files (*.png);;JPG Files (*.jpg);;JPEG Files (*.jpeg)");

    // Set the dialog to select a single file
    dialog.setFileMode(QFileDialog::ExistingFile);

    // Open the dialog and get the selected file path
    QString selectedFilePath = dialog.getOpenFileName(newWindow, "Select an image file", QString(), "PNG Files (*.png);;JPG Files (*.jpg);;JPEG Files (*.jpeg)");

    if (selectedFilePath.isEmpty()) {
        return;
    }

    emit newImagePath(selectedFilePath);

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
        QPixmap defaultImage(":/resources/images/NoPlaylistImage.png");
        map = defaultImage.scaled(changedSize, Qt::KeepAspectRatio);
    }

    //Set the label to have the PixMap
    this->art->setPixmap(map);
}
