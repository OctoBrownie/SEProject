#include "searchwindow.h"
#include "ui_searchwindow.h"
#include <QVBoxLayout>
#include "playlist.h"
#include <songclone.h>

//Constructor
SearchWindow::SearchWindow(Playlist* playlist, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SearchWindow)
{
    ui->setupUi(this);

    //Set the playlist variable
    this->playlist = playlist;

    //Create the layout, and apply it to the widget inside of the qscrollarea in the layout
    this->songs = new QVBoxLayout();
    this->songs->setAlignment(Qt::AlignTop);
    this->ui->scrollAreaWidgetContents->setLayout(this->songs);
    this->ui->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->ui->scrollArea->setWidgetResizable(true);

    //close the window when hitting the exit button
    connect(this->ui->pushButton, &QPushButton::clicked, this, &SearchWindow::hide);

    //Run the query when the button is clicked, a song is added to the playlist, or a song is removed from the playlist
    connect(this->ui->searchButton, &QPushButton::clicked, this, &SearchWindow::query);
    connect(this->playlist, &Playlist::newSongAdded, this, &SearchWindow::query);
    connect(this->playlist, &Playlist::songRemoved, this, &SearchWindow::query);
}

//Destructor
SearchWindow::~SearchWindow()
{
    delete this->playlist;
    delete ui;
}

//Run a query for the playlist.
void SearchWindow::query() {
    //Only run the query when the window is visible, to prevent slowdowns
    if (this->isVisible()) {
        //Remove every item already contained in the QScrollArea widget,
        while(this->songs->count()) {
            QLayoutItem* item = this->songs->takeAt(0);
            if (item->widget()) {
                item->widget()->deleteLater();
            }
        }

        int count = 0;
        //Get the index of the selected value
        int boxChoice = this->ui->comboBox->currentIndex();

        //Get the query and create an array of songs from the playlist
        QVector<Song*>* list = this->playlist->getSongList();
        QString query = ui->lineEdit->text();


        //For every song, based on the combo box option, check each song against the query searchString
        for (int i = 0; i < list->length(); i++) {
            QString searchString;
            if (boxChoice == 0) {
                searchString = (*list)[i]->getArtist().toLower();
            } else if (boxChoice == 1) {
                searchString = (*list)[i]->getSongTitle().toLower();
            } else {
                searchString = (*list)[i]->getAlbum().toLower();
            }

            //If the query string is in the searchString
            if (searchString.contains(query.toLower())) {
                //Create a songClone of the song; a simplified GUI of the output. Add to the widget, and keep a counts
                SongClone* newSong = new SongClone((*list)[i]);
                this->songs->addWidget(newSong);
                count++;
            }
        }

        //If no results, print out the message
        if (count == 0){
            this->songs->addWidget(new QLabel("No Songs Found"));
        }
    }
}

//Set the playlist and use the connect API to monitor for outputs
void SearchWindow::setPlaylist(Playlist* playlist) {
    this->playlist = playlist;
    connect(this->playlist, &Playlist::newSongAdded, this, &SearchWindow::query);
    connect(this->playlist, &Playlist::songRemoved, this, &SearchWindow::query);
}
