#include<QWidget>
#include<QVBoxLayout>
#include<QVector>
#include<QFile>
#include<QTextStream>
#include<QScrollArea>
#include<QLineEdit>

#include "playlist.h"
#include "song.h"
#include "musicplayer.h"

#include<iostream>

//Playlist constructor
Playlist::Playlist(MusicPlayer* player, QObject* parent)
	: QObject{parent}, musicPlayer{player} {
	songsListGUI = nullptr;
	allSongs = new QVector<Song*>();
	if (player == nullptr) std::cout << "null in playlist" << std::endl;
}

Playlist* Playlist::createPlaylist(QString filename, MusicPlayer* player, QObject* parent) {
	Playlist* playlist = new Playlist(player, parent);

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

	//Go through the file
	qint64 position = 0;
	while (!input.atEnd()) {
		//Read the line,
		QString songPath = input.readLine();

		//Init the song
		Song* newSong = Song::createSong(songPath);
		if (newSong == nullptr) {
			// invalid song, skip it
			std::cerr << "Couldn't initialize \"" << songPath.toStdString() << '"' << std::endl;
			continue;
		}

		//Set the position of the song in the playlist
		newSong->setPosition(position);

		//Add to the vector and increase position
		playlist->allSongs->append(newSong);
		position++;

		// Connect the selectedSong signal from Song, so that when the signal is sent, the playlist will
		// set the selected song to the value in the signal (the value of the song in the playlist)
		connect(newSong, &Song::selectedSong, playlist, &Playlist::setSelectedSong);

		//Connect to the button clicked signal, to determine when the up or down button is clicked.
		connect(newSong, &Song::buttonClicked, playlist, &Playlist::moveSong);

		if (player != nullptr) connect(newSong, SIGNAL(playSong(Song*)), player, SLOT(setSong(Song*)));
	}

	file->close();

	//Create the GUI elements
	playlist->createPlaylistOutput();
	return playlist;
}

Playlist::~Playlist() {
	// will automatically delete songsListLayout too
	if (songsListGUI != nullptr) delete songsListGUI;
	if (allSongs != nullptr) delete allSongs;
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
		for(int i = 0; i < this->allSongs->length(); i++) {
			output << (*allSongs)[i]->getSongPath() << "\n";
		}
    }
}


//Add a song to a playlist
void Playlist::addSong(Song* newSong) {
	if (newSong == nullptr) return;

	//Set position to the last element.
	newSong->setPosition(this->allSongs->length());
	//Add to the vector
	this->allSongs->append(newSong);

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

	if (musicPlayer != nullptr) connect(newSong, SIGNAL(playSong(Song*)), musicPlayer, SLOT(setSong(Song*)));
}

//Remove a song
void Playlist::removeSong(qint64 position) {
	//Invalid range, prevents removal
	if (position < 0 || position >= allSongs->length()) {
        return;
    }

    //Update the positions of all of the songs in the array.
	for (int i = position + 1; i < allSongs->length(); i++) {
		(*allSongs)[i]->setPosition(i - 1);
    }

	//Choose the song at the specified position.
	Song* chosenSong = (*allSongs)[position];

    //Update the duration, and the label in the metadata text.
    this->duration -= chosenSong->getDuration();
    this->textData->updateDurationLabel(this->duration);

    //Remove the song widget from the GUI
    this->songsListLayout->removeWidget(chosenSong);

	//Remove widget from the vector
	this->allSongs->remove(position);

    //Free widget
    delete chosenSong;
}


//Get the selected song; incredibly important for MP3Player interactions
qint64 Playlist::getSelectedSong() {
	return this->selectedSong;
}

/*

    Setters

*/

//Get the position of a song that needs to be selected.
void Playlist::setSelectedSong(qint64 pos, bool move) {
	if (pos >= allSongs->length() || pos < 0) {
		return;
	}

	//If the new selected song is the same as the old selected song, deselect
	if (this->selectedSong == pos) {
		this->selectedSong = -1;
	}
	else {
		//If another song is selected, deselect that song
		if (this->selectedSong != -1) {
			(*allSongs)[this->selectedSong]->setSelected(false);
		}

		if((*allSongs)[pos]->getSelected() == false) {
			(*allSongs)[pos]->setSelected(true);
		}
		//Select the new song
		this->selectedSong = pos;
	}

	if (!move) {
		if (this->selectedSong == -1) {
			emit newSelectedSong("", "Title", "Artist", "Album", new QImage());
		} else {
			//Proof of concept and testing
			Song* sendSong = (*allSongs)[this->selectedSong];
			emit newSelectedSong(sendSong->getSongPath(), sendSong->getSongTitle(),
								 sendSong->getArtist(), sendSong->getAlbum(), sendSong->getArt());
		}
	}
}


//Set the image path and image itself, based on the path
void Playlist::setImagePath(QString imagePath) {
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
QString Playlist::getImagePath() {
	return this->imagePath;
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

	// TODO: PICTURE BOX?!
	// The picture data and the text data in the UI.
//	this->playlistImageBox = new PictureBox(this->getImagePath());
//	connect(this->playlistImageBox, &PictureBox::newImagePath, this, &Playlist::setImagePath);

	this->textData = new TextMetadata(this->playlistName, this->userName, this->duration);

	//When the playlist title and username are edited in the TextMetadata GUI, they are also updated in the playlist.
	connect(this->textData->getPlaylistTitle(), &QLineEdit::textChanged, this, &Playlist::setPlaylistName);
	connect(this->textData->getUsername(), &QLineEdit::textChanged, this, &Playlist::setUserName);

	//Songs list layout and widget
	QScrollArea* scrollArea = new QScrollArea();
	QWidget* songsListWidget = new QWidget(scrollArea);
	songsListWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	QVBoxLayout* songsListLayout = new QVBoxLayout();
	songsListLayout->setAlignment(Qt::AlignTop);
	songsListLayout->addWidget(this->textData);

	//For all songs in the playlist, add the widget to the layout
	for (int i = 0; i < allSongs->length(); i++) {
		songsListLayout->addWidget((*allSongs)[i]);
	}
	songsListLayout->addStretch();

	//set the widget to have the layout, and store the layout and the widget.
	songsListWidget->setLayout(songsListLayout);
	scrollArea->setWidget(songsListWidget);

	this->songsListLayout = songsListLayout;

	this->songsListGUI = scrollArea;
}


//Find the playlist duration, by adding the duration of all of the songs together.
void Playlist::calculatePlaylistDuration() {
	int newDuration = 0;
	for (int x = 0; x < (this->allSongs)->length(); x ++) {
		newDuration += (*allSongs)[x]->getDuration();
    }

    this->duration = newDuration;
}




//Move a song up or down in the playlist, based on the status
void Playlist::moveSong(int pos, bool up) {
	if (up) {
		//Ensure the elements do not ascend above 0, which is impossible
		if (pos - 1 >= 0) {
			//Save the widget temporarily
			Song* tempSong1 = (*allSongs)[pos];

			//Swap the allSongs[pos] and allSongs[pos - 1] elements. Up and down switch
			(*allSongs)[pos] = (*allSongs)[pos - 1];
			(*allSongs)[pos - 1] = tempSong1;

			//Reset the positions of the elements, so there is no confusion.
			(*allSongs)[pos]->setPosition(pos);
			(*allSongs)[pos - 1]->setPosition(pos - 1);

			//Maintain the proper selected song; check if any of the swapped songs were the selected song, and if any of them were, swap the selectedSong
			if (this->selectedSong == pos) {
				this->setSelectedSong(pos - 1, true);
			} else if (this->selectedSong == pos - 1) {
				this->setSelectedSong(pos, true);
			}

			//Remove both widgets from the layout
			this->songsListLayout->removeWidget((*allSongs)[pos - 1]);
			this->songsListLayout->removeWidget((*allSongs)[pos]);

			//Insert them in the reverse position; add the item to be pushed towards the back first
			this->songsListLayout->insertWidget(pos, (*allSongs)[pos]);
			this->songsListLayout->insertWidget(pos, (*allSongs)[pos - 1]);
		}
	} else {
		//Ensure the elements do not descend below the maximum size, which is impossible
		if (pos + 1< this->allSongs->length()) {
			//Save widget temporarily
			Song* tempSong1 = (*allSongs)[pos];

			//Swap
			(*allSongs)[pos] = (*allSongs)[pos + 1];
			(*allSongs)[pos + 1] = tempSong1;

			//Reset positions
			(*allSongs)[pos]->setPosition(pos);
			(*allSongs)[pos + 1]->setPosition(pos +1);

			//Maintain the proper selected song; check if any of the swapped songs were the selected song, and if any of them were, swap the selectedSong
			if (this->selectedSong == pos) {
				this->setSelectedSong(pos + 1, true);
			} else if (this->selectedSong == pos + 1) {
				this->setSelectedSong(pos, true);
			}

			//Remove both widgets from the layout
			this->songsListLayout->removeWidget((*allSongs)[pos + 1]);
			this->songsListLayout->removeWidget((*allSongs)[pos]);

			//Insert them in the reverse position; add the item to be pushed back first
			this->songsListLayout->insertWidget(pos + 1, (*allSongs)[pos + 1]);
			this->songsListLayout->insertWidget(pos + 1, (*allSongs)[pos]);
		}
	}
}
