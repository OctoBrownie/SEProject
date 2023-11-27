#include<QWidget>
#include<QVBoxLayout>
#include<QVector>
#include<QFile>
#include<QTextStream>
#include<QScrollArea>
#include<QLineEdit>

#include "playlist.h"

//Playlist constructor
Playlist::Playlist(QObject* parent) : QObject(parent) {}

Playlist* Playlist::createPlaylist(QString filename, QObject* parent) {
	Playlist* playlist = new Playlist(parent);

	if (filename == nullptr) return playlist;

	//Open file
	QFile* file = new QFile(filename);
	if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
		// TODO: error handling. Raise an alert and exit?
		delete playlist;
		return nullptr;
	}

	//Read through the file
	QTextStream input(file);

	//Set the playlist data from the file
	playlist->openedPlaylist = filename;
	playlist->playlistName = input.readLine();
	playlist->userName = input.readLine();
	playlist->duration = input.readLine().toInt();
	playlist->imagePath = input.readLine();
	playlist->playlistImage = QImage(playlist->imagePath);

	//Go through the file
	qint64 position = 0;
	while (!input.atEnd()) {
		//Read the line,
		QString songPath = input.readLine();

		//Init the song
		Song* newSong = Song::createSong(songPath);
		if (newSong == nullptr) {
			// invalid song, skip it
			continue;
		}

		//Set the position of the song in the playlist
		newSong->setPosition(position);

		//Add to the vector and increase position
		playlist->allSongs.append(newSong);
		position++;

		//Connect the selectedSong signal from Song, so that when the signal is sent, the playlist will set the selected song to the value in the signal (the value of the song in the playlist)
		connect(newSong, &Song::selectedSong, playlist, &Playlist::setSelectedSong);

		//Connect to the button clicked signal, to determine when the up or down button is clicked.
		connect(newSong, &Song::buttonClicked, playlist, &Playlist::moveSong);
	}

	file->close();

	//Create the GUI elements
	playlist->createPlaylistOutput();
	return playlist;
}

Playlist::~Playlist() {
	// will automatically delete songsListLayout too
	delete songsListGUI;
}


//Save the playlist to a file
void Playlist::savePlaylist(QString* filename) {
    QFile* file = new QFile(*filename);

    if (file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {

        QTextStream output(file);

        //Write the first 4 lines to be the playlistName, username, duration, and the path to the image.
        output << this->playlistName << "\n";
        output << this->userName << "\n";
        output << this->duration << "\n";
        output << this->imagePath << "\n";

        //For every song in the playlist, get the song path, and write to the file
        for(int i = 0; i < this->allSongs.length(); i++) {
            output << allSongs[i]->getSongPath() << "\n";
        }
    }
}


//Add a song to a playlist
void Playlist::addSong(Song* newSong) {
	if (newSong == nullptr) return;

    //Set position to the last element.
    newSong->setPosition(this->allSongs.length());
    //Add to the vector
    this->allSongs.append(newSong);

    //Update the duration
    this->duration += newSong->getDuration();

    //Add the widget to the end of the layout
    this->songsListLayout->addWidget(newSong);

    //Update the duration label in the textmetadata class
	this->textData->updateDurationLabel(this->duration);

    //Connect with the selected song signal, so when a song is selected, the playlist can determine which one.
    connect(newSong, &Song::selectedSong, this, &Playlist::setSelectedSong);

    //Connect with the button clicked song, so when either the up or down button is clicked, the playlist can respond.
    connect(newSong, &Song::buttonClicked, this, &Playlist::moveSong);
}

//Remove a song
void Playlist::removeSong(qint64 position) {
	//Invalid range, prevents removal
	if (position < 0 || position >= allSongs.length()) {
        return;
    }

    //Update the positions of all of the songs in the array.
    for (int i = position + 1; i < allSongs.length(); i++) {
        allSongs[i]->setPosition(i - 1);
    }

    //Choose the song at the specified position.
    Song* chosenSong = this->allSongs[position];

    //Update the duration, and the label in the metadata text.
    this->duration -= chosenSong->getDuration();
    this->textData->updateDurationLabel(this->duration);

    //Remove the song widget from the GUI
    this->songsListLayout->removeWidget(chosenSong);

    //Remove widget from the vector
    this->allSongs.remove(position);

    //Free widget
    delete chosenSong;
}


//Get the selected song; incredibly important for MP3Player interactions
Song* Playlist::getSelectedSong() {
    return allSongs[selectedSong];
}

/*

    Setters

*/

//Get the position of a song that needs to be selected.
void Playlist::setSelectedSong(qint64 pos) {
    //If the new selected song is the same as the old selected song, deselect
    if (this->selectedSong == pos) {
        this->selectedSong = -1;
    }
    else {
        //If another song is selected, deselect that song
        if (this->selectedSong != -1) {
            allSongs[this->selectedSong]->setSelected(0);
        }

        //Select the new song
        this->selectedSong = pos;
    }

    //Proof of concept and testing
    qDebug() << "The song " << this->selectedSong << " in the list is selected!!!";
}


//Set the image path and image itself, based on the path
void Playlist::setImage(QString imagePath) {
    this->playlistImage = QImage(imagePath);
    this->imagePath = imagePath;
}

//Set username; used in textmetadata
void Playlist::setUserName(QString name) {
    this->userName = name;
}

//Set playlistname; used in textmetadata
void Playlist::setPlaylistName(QString name) {
    this->playlistName = name;
}





/*

    Getters

*/


//Return the playlist image pointer; used in picturebox
QImage* Playlist::getImage() {
    return &this->playlistImage;
}


QString Playlist::getUserName() {
    return this->userName;
}

QString Playlist::getPlaylistName() {
    return this->playlistName;
}


//Get the widget storing the list of songs.
QWidget* Playlist::getListGUI() {
    return this->songsListGUI;
}

PictureBox* Playlist::getPictureBox() {
    return this->playlistImageBox;
}

TextMetadata* Playlist::getTextMetadata() {
    return this->textData;
}

qint64 Playlist::getDuration() {
    return this->duration;
}


/*

    Protected

*/


//Turn the playlist information into a GUI
void Playlist::createPlaylistOutput() {

    // The picture data and the text data in the UI. Messy, need to fix
    this->playlistImageBox = new PictureBox(this->getImage());
    this->textData = new TextMetadata(this->playlistName, this->userName, this->duration);

    //When the playlist title and username are edited in the TextMetadata GUI, they are also updated in the playlist.
    connect(this->textData->getPlaylistTitle(), &QLineEdit::textChanged, this, &Playlist::setPlaylistName);
    connect(this->textData->getUsername(), &QLineEdit::textChanged, this, &Playlist::setUserName);

    //Songs list layout and widget
    QWidget* songsListWidget = new QWidget();
    QVBoxLayout* songsListLayout = new QVBoxLayout();

    //For all songs in the playlist, add the widget to the layout
    for(int i =0; i < allSongs.length(); i++) {
        songsListLayout->addWidget(allSongs[i]);
    }

    //set the widget to have the layout, and store the layout and the widget.
    songsListWidget->setLayout(songsListLayout);

    this->songsListLayout = songsListLayout;
    this->songsListGUI = songsListWidget;
}


//Find the playlist duration, by adding the duration of all of the songs together.
void Playlist::calculatePlaylistDuration() {
    int newDuration = 0;
    for (int x = 0; x < (this->allSongs).length(); x ++) {
        newDuration += allSongs[x]->getDuration();
    }

    this->duration = newDuration;
}




//Move a song up or down in the playlist, based on the status
void Playlist::moveSong(int pos, int status) {
    //If the status is 1 (up button)
    if (status == 1) {
        //Ensure the elements do not ascend above 0, which is impossible
        if (pos - 1 >= 0) {
            //Save the widget temporarily
            Song* tempSong1 = this->allSongs[pos];

            //Swap the allSongs[pos] and allSongs[pos - 1] elements. Up and down switch
            allSongs[pos] = allSongs[pos - 1];
            allSongs[pos - 1] = tempSong1;

            //Reset the positions of the elements, so there is no confusion.
            allSongs[pos]->setPosition(pos);
            allSongs[pos - 1]->setPosition(pos - 1);

            //Maintain the proper selected song; check if any of the swapped songs were the selected song, and if any of them were, swap the selectedSong
            if (this->selectedSong == pos) {
                this->setSelectedSong(pos - 1);
            } else if (this->selectedSong == pos - 1) {
                this->setSelectedSong(pos);
            }

            //Remove both widgets from the layout
            this->songsListLayout->removeWidget(allSongs[pos - 1]);
            this->songsListLayout->removeWidget(allSongs[pos]);

            //Insert them in the reverse position; add the item to be pushed towards the back first
            this->songsListLayout->insertWidget(pos - 1, allSongs[pos]);
            this->songsListLayout->insertWidget(pos - 1, allSongs[pos - 1]);
        }
    } else {
        //Ensure the elements do not descend below the maximum size, which is impossible
        if (pos + 1< this->allSongs.length()) {
            //Save widget temporarily
            Song* tempSong1 = this->allSongs[pos];

            //Swap
            allSongs[pos] = allSongs[pos + 1];
            allSongs[pos + 1] = tempSong1;

            //Reset positions
            allSongs[pos]->setPosition(pos);
            allSongs[pos + 1]->setPosition(pos +1);

            //Maintain the proper selected song; check if any of the swapped songs were the selected song, and if any of them were, swap the selectedSong
            if (this->selectedSong == pos) {
                this->setSelectedSong(pos + 1);
            } else if (this->selectedSong == pos + 1) {
                this->setSelectedSong(pos);
            }

            //Remove both widgets from the layout
            this->songsListLayout->removeWidget(allSongs[pos + 1]);
            this->songsListLayout->removeWidget(allSongs[pos]);

            //Insert them in the reverse position; add the item to be pushed back first
            this->songsListLayout->insertWidget(pos, allSongs[pos + 1]);
            this->songsListLayout->insertWidget(pos, allSongs[pos]);
        }
    }
}
