//Library Header Includes

#include <QMessageBox>
#include <QFileDialog>
#include <QScrollArea>
#include<QWidget>
#include<QDockWidget>
#include<QHBoxLayout>
#include<QPushButton>

//Application Header Includes

#include "interface.h"
#include "picturebox.h"
#include "textmetadata.h"
#include "mediaplayer.h"
#include "playlist.h"
#include "settingswindow.h"
#include "searchwindow.h"


//Interface Constructor
Interface::Interface(): QMainWindow() {


    //Create and set the blank, empty playlist
    this->currentPlaylist = new Playlist("");

    //Create settings window
    this->settingsWindow = new SettingsWindow();

    //Media player element
    this->player = new MediaPlayer();
    //Set the player to be assigned to a set playlist
    this->player->setPlaylist(this->currentPlaylist);


/* ----------------------------
 *
 *          Playlist Display and Editing Window
 *
 * ---------------------------- */

    // The layout of the playlist editor portion of the application
    this->playlistEditor = new QVBoxLayout;
    playlistEditor->setAlignment(Qt::AlignTop);


    //This box holds the image and the text (title, user, and duration)
    this->metadataHolder = new QHBoxLayout();
    // The picture data and the text data in the UI
    this->photoData = this->currentPlaylist->getPictureBox();
    this->textData = this->currentPlaylist->getTextMetadata();
    // Add data to the holder component
    this->metadataHolder->addWidget(photoData);
    this->metadataHolder->addWidget(textData);



    //The playlist editor buttons in the editing window
    QHBoxLayout* playlistEditorButtons = this->createPlaylistToolbar();


    //This is the scrollable list of the songs in the playlist.
    this->songsList = new QScrollArea;
    songsList->setWidgetResizable(true);
    songsList->setWidget(this->currentPlaylist->getListGUI());




    // Add data to the playlist editing widget
    this->playlistEditor->addLayout(this->metadataHolder);
    this->playlistEditor->addLayout(playlistEditorButtons);
    this->playlistEditor->addWidget(this->songsList);


    //Assign the created layout to a widget
    QWidget* playlistEditWidget = new QWidget();
    playlistEditWidget->setLayout(this->playlistEditor);

    // Create a widget to be added to the main window that displays playlist data and allows editing
    QDockWidget* playlistEditPane = new QDockWidget;
    playlistEditPane->setWidget(playlistEditWidget);
    playlistEditPane->setMinimumWidth(500);


/* ----------------------------
 *
 * Main Window Element
 *
 * ---------------------------- */


    // The layout of the main portion of the application
    QVBoxLayout* mainAppLayout = new QVBoxLayout;
    mainAppLayout->setAlignment(Qt::AlignTop);


    //The buttons to import, export, and use the settings of the application
    QHBoxLayout* applicationButtons = this->createMainToolbar();

    //Spacer to keep the MP3 player element at the bottom of the screen, and the application buttons at the top
    QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    mainAppLayout->addLayout(applicationButtons);
    mainAppLayout->addSpacerItem(spacer);
    mainAppLayout->addWidget(this->player);


    //The main section of the window. Not the playlist editing submenu on the left.
    QWidget* window = new QWidget;
    window->setLayout(mainAppLayout);


/* ----------------------------
 *
 *  Interface Element
 *
 * ---------------------------- */

    //Set the size, central widget, and the extra dock widget
    this->setCentralWidget(window);
    this->addDockWidget(Qt::LeftDockWidgetArea, playlistEditPane);
    this->resize(1500, 1000);
}


Interface::~Interface() {
    delete this->currentPlaylist;
}


QHBoxLayout* Interface::createMainToolbar()
{
    QHBoxLayout* toolbar = new QHBoxLayout();

    QPushButton* importButton = new QPushButton("Import Playlist");
    connect(importButton, &QPushButton::clicked, this, &Interface::importPlaylist);
    toolbar->addWidget(importButton);


    QPushButton* saveButton = new QPushButton("Save Playlist");
    connect(saveButton, &QPushButton::clicked, this, &Interface::savePlaylist);
    toolbar->addWidget(saveButton);

    QPushButton* exportButton = new QPushButton("Export Playlist");
    connect(exportButton, &QPushButton::clicked, this, &Interface::exportPlaylist);
    toolbar->addWidget(exportButton);

    QPushButton* removeButton = new QPushButton("Remove Playlist");
    connect(removeButton, &QPushButton::clicked, this, &Interface::deletePlaylist);

    toolbar->addWidget(removeButton);

    QPushButton* settingsButton = new QPushButton("Settings");
    connect(settingsButton, &QPushButton::clicked, this->settingsWindow, &QMainWindow::show);
    toolbar->addWidget(settingsButton);

	return toolbar;
}

QHBoxLayout* Interface::createPlaylistToolbar() {
    QHBoxLayout* toolbar = new QHBoxLayout();

    QPushButton* addButton = new QPushButton("Add to Playlist");
    connect(addButton, &QPushButton::clicked, this, &Interface::importSong);
    toolbar->addWidget(addButton);

    QPushButton* addFolderButton = new QPushButton("Add Folder to Playlist");
    connect(addFolderButton, &QPushButton::clicked, this, &Interface::importFolder);
    toolbar->addWidget(addFolderButton);

    QPushButton* deleteButton = new QPushButton("Delete Selected");
    connect(deleteButton, &QPushButton::clicked, [=] {
        this->currentPlaylist->removeSong(this->currentPlaylist->getSelectedSong());
        this->currentPlaylist->setSelectedSong(-5, false);
    });
    toolbar->addWidget(deleteButton);

    this->searchWindow = new SearchWindow(this->currentPlaylist, nullptr);

    QPushButton* searchButton = new QPushButton("Search");
    connect(searchButton, &QPushButton::clicked, searchWindow, [=] {
        searchWindow->show();
        searchWindow->query();
    });

    toolbar->addWidget(searchButton);

	return toolbar;
}


void Interface::savePlaylist() {
    if (this->currentPlaylist->getPlaylistName() == "") {
        QMessageBox::critical(nullptr, "Error", "Please set a playlist name before exporting!");
    } else if (this->currentPlaylist->getUserName() == "") {
        QMessageBox::critical(nullptr, "Error", "Please set the name of the creator before saving!");
    } else if (*this->currentPlaylist->getPlaylistPath() == "") {
        QMessageBox::critical(nullptr, "Error", "Please Export Before Saving! This will set the spot where the playlist will be saved!");
    } else {
        this->currentPlaylist->savePlaylist(this->currentPlaylist->getPlaylistPath());
    }
}

void Interface::exportPlaylist()
{
    QMainWindow* newWindow = new QMainWindow();
    QFileDialog dialog(newWindow);
    dialog.setNameFilter("Playlist Files (*.pa)");

    // Open the dialog and get the selected file path
    QString selectedFilePath = dialog.getSaveFileName(newWindow, "Select an image file", QString(), "Playlist Files (*.pa)");

    if(!selectedFilePath.isEmpty()) {
        if(!selectedFilePath.endsWith(".pa", Qt::CaseInsensitive)) {
            selectedFilePath += ".pa";
        }

        this->currentPlaylist->setPlaylistPath(selectedFilePath);
        this->savePlaylist();
    }
}


void Interface::importSong() {

    QMainWindow* newWindow = new QMainWindow();
    QFileDialog dialog;
    dialog.setNameFilter("MP3 Files (*.mp3)");
    dialog.setFileMode(QFileDialog::Directory);

    QStringList selectedFilePath = dialog.getOpenFileNames(newWindow, "Selected an MP3 file", QString(), "MP3 Files (*.mp3)");

    if (!selectedFilePath.isEmpty()) {
        foreach (const QString file, selectedFilePath) {
            Song* newSong = new Song(file);
            this->currentPlaylist->addSong(newSong);
        }
    }
}

void Interface::importFolder() {
    QMainWindow* newWindow = new QMainWindow();
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setNameFilter("MP3 Files (*.mp3)");
    dialog.setWindowTitle("Select Folders with  MP3 Files");
    dialog.setOption(QFileDialog::ShowDirsOnly, true);

    QString selectedFolder = dialog.getExistingDirectory(newWindow, "Select Folder", QString(), QFileDialog::ReadOnly);

    QDir dir(selectedFolder);

    if (dir.exists()) {
        QStringList allFiles = dir.entryList(QDir::Files);

        foreach (QString mp3File, allFiles)
        {
            if (mp3File.toLower().endsWith(".mp3")) {
                QString fullpath = selectedFolder + "/" + mp3File;
                Song* newSong = new Song(fullpath);
                this->currentPlaylist->addSong(newSong);
            }
        }
    } else {
        QMessageBox::critical(this, "Error", "The selected folder is invalid. Try again");
        return;
    }
}


void Interface::importPlaylist() {
    QMainWindow* newWindow = new QMainWindow();
    QFileDialog dialog;
    dialog.setNameFilter("Playlist Files (*.pa)");
    dialog.setFileMode(QFileDialog::Directory);

    QString selectedFilePath = dialog.getOpenFileName(newWindow, "Selected a Playlist file", QString(), "Playlist Files (*.pa)");

    if (!selectedFilePath.isEmpty()) {

        Playlist* temp = this->currentPlaylist;
        this->currentPlaylist = new Playlist(selectedFilePath);
        delete temp;


        this->metadataHolder->removeWidget(this->photoData);
        this->metadataHolder->removeWidget(this->textData);


        playlistEditor->removeWidget(songsList);
        delete songsList;

        this->songsList = new QScrollArea;
        songsList->setWidgetResizable(true);


        songsList->setWidget(currentPlaylist->getListGUI());

        playlistEditor->addWidget(songsList);
        metadataHolder->addWidget(this->currentPlaylist->getPictureBox());
        metadataHolder->addWidget(this->currentPlaylist->getTextMetadata());

        player->setPlaylist(this->currentPlaylist);
        player->killStream();
        this->currentPlaylist->setSelectedSong(-5, false);

        this->searchWindow->setPlaylist(this->currentPlaylist);
    }
}

void Interface::deletePlaylist() {
    Playlist* temp = this->currentPlaylist;
    this->currentPlaylist = new Playlist("");
    QString path = *temp->getPlaylistPath();
    delete temp;


    this->metadataHolder->removeWidget(this->photoData);
    this->metadataHolder->removeWidget(this->textData);


    playlistEditor->removeWidget(songsList);
    delete songsList;

    this->songsList = new QScrollArea;
    songsList->setWidgetResizable(true);


    songsList->setWidget(currentPlaylist->getListGUI());

    playlistEditor->addWidget(songsList);
    metadataHolder->addWidget(this->currentPlaylist->getPictureBox());
    metadataHolder->addWidget(this->currentPlaylist->getTextMetadata());

    player->setPlaylist(this->currentPlaylist);
    player->killStream();
    this->currentPlaylist->setSelectedSong(-5, false);

    this->searchWindow->setPlaylist(this->currentPlaylist);

    if (path != "") {
        qDebug() << "This is the new path: " << path;
        QFile file(path);
        file.remove();
    }
}

