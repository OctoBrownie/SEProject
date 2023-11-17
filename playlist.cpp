#include "playlist.h"

Playlist::Playlist(QString* filename)
{
    this->processPlaylist(filename);
}

void Playlist::processPlaylist(QString* filename) {
    QFile* file = new QFile(*filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        //Error handling. Will probably raise an alert, and exit.
        return;
    }

    QTextStream input(file);
    qint64 position = 0;

    this->openedPlaylist = filename;
    this->playlistName = input.readLine();
    this->userName = input.readLine();
    this->duration = input.readLine().toInt();
    this->imagePath = input.readLine();

    while (!input.atEnd()) {
        QString line = input.readLine();
        QByteArray byteArray = line.toUtf8();
        const char* songPath = byteArray.constData();
        Song* newSong = new Song(songPath);
        newSong->setPosition(position);
        this->allSongs.append(newSong);
        position++;
    }

    file->close();

    createPlaylistOutput();
}


void Playlist::savePlaylist(QString* filename) {
    QFile* file = new QFile(*filename);

    if (file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {

        QTextStream output(file);

        output << this->playlistName << "\n";
        output << this->userName << "\n";
        output << this->duration << "\n";
        output << this->imagePath << "\n";

        for(int i = 0; i < this->allSongs.length(); i++) {
            output << *allSongs[i]->getSongPath() << "\n";
        }
    }
}


void Playlist::createPlaylistOutput() {
    // The layout of the editor portion of the layout application
    QVBoxLayout* playlistEditor = new QVBoxLayout;
    playlistEditor->setAlignment(Qt::AlignTop);

    // The picture data and the text data in the UI
    this->playlistImageBox = new PictureBox(this);
    this->textData = new TextMetadata(this);

    // These are the buttons that allow playlist editing
    this->editButtons = new EditingButtons;

    //Songs list layout and widget
    QWidget* songsListWidget = new QWidget();
    QVBoxLayout* songsListLayout = new QVBoxLayout();

    for(int i =0; i < allSongs.length(); i++) {
        Song* songToPrint = allSongs[i];
        CustomSongWidget* songBox = songToPrint->getBox();
        songsListLayout->addWidget(songBox);
    }

    songsListWidget->setLayout(songsListLayout);

    this->songsListLayout = songsListLayout;

    this->songsListGUI = songsListWidget;
}

void Playlist::calcLength() {
    int newDuration = 0;
    for (int x = 0; x < (this->allSongs).length(); x ++) {
        newDuration += allSongs[x]->getDuration();
    }

    this->duration = newDuration;
}


void Playlist::addSong(Song* newSong) {
    newSong->setPosition(this->allSongs.length());
    this->allSongs.append(newSong);
    this->duration += newSong->getDuration();
    this->songsListLayout->addWidget(newSong->getBox());

    this->textData->updateDurationLabel(this->duration);
}

QWidget* Playlist::getListGUI() {
    return this->songsListGUI;
}


void Playlist::removeSong(qint64 position) {
    if (position >= allSongs.length()) {
        return;
    }

    for (int i = position + 1; i < allSongs.length(); i++) {
        allSongs[i]->setPosition(i - 1);
    }

    Song* chosenSong = this->allSongs[position];

    this->duration -= chosenSong->getDuration();
    this->textData->updateDurationLabel(this->duration);

    CustomSongWidget* songGUI = chosenSong->getBox();

    this->songsListLayout->removeWidget(songGUI);

    this->allSongs.remove(position);

    delete songGUI;
    delete chosenSong;
}



Song* Playlist::getSelectedSong() {
    return allSongs[selectedSong];
}


void Playlist::setImage(QString imagePath) {
    this->playlistImage = QImage(imagePath);
    this->imagePath = imagePath;
}

void Playlist::setUserName(QString name) {
    this->userName = name;
}

void Playlist::setPlaylistName(QString name) {
    this->playlistName = name;
}

QImage* Playlist::getImage() {
    return &this->playlistImage;
}

QString Playlist::getUserName() {
    return this->userName;
}

QString Playlist::getPlaylistName() {
    return this->playlistName;
}


PictureBox* Playlist::getPictureBox() {
    return this->playlistImageBox;
}

TextMetadata* Playlist::getTextMetadata() {
    return this->textData;
}

EditingButtons* Playlist::getEditingButtons() {
    return this->editButtons;
}

qint64 Playlist::getDuration() {
    return this->duration;
}
