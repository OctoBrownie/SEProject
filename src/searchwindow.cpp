#include "searchwindow.h"
#include "ui_searchwindow.h"

SearchWindow::SearchWindow(Playlist* playlist, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SearchWindow)
{
    ui->setupUi(this);

    this->playlist = playlist;

    this->songs = new QVBoxLayout();
    this->songs->setAlignment(Qt::AlignTop);
    this->ui->scrollAreaWidgetContents->setLayout(this->songs);
    this->ui->scrollAreaWidgetContents->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->ui->scrollArea->setWidgetResizable(true);

    connect(this->ui->pushButton, &QPushButton::clicked, this, &SearchWindow::hide);

    connect(this->ui->searchButton, &QPushButton::clicked, this, &SearchWindow::query);
    connect(playlist, &Playlist::newSongAdded, this, &SearchWindow::query);
    connect(playlist, &Playlist::songRemoved, this, &SearchWindow::query);
}

SearchWindow::~SearchWindow()
{
    delete ui;
}

void SearchWindow::query() {
    if (this->isVisible()) {
        while(this->songs->count()) {
            QLayoutItem* item = this->songs->takeAt(0);
            if (item->widget()) {
                item->widget()->deleteLater();
            }
        }

        int count = 0;
        int boxChoice = this->ui->comboBox->currentIndex();
        QVector<Song*>* list = this->playlist->getSongList();
        QString query = ui->lineEdit->text();


        for (int i = 0; i < list->length(); i++) {
            QString searchString;
            if (boxChoice == 0) {
                searchString = (*list)[i]->getArtist().toLower();
            } else if (boxChoice == 1) {
                searchString = (*list)[i]->getSongTitle().toLower();
            } else {
                searchString = (*list)[i]->getAlbum().toLower();
            }

            if (searchString.contains(query.toLower())) {
                SongClone* newSong = new SongClone((*list)[i]);
                this->songs->addWidget(newSong);
                count++;
            }
        }

        if (count == 0){
            this->songs->addWidget(new QLabel("No Songs Found"));
        }
    }
}
