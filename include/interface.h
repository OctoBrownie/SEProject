#ifndef INTERFACE_H
#define INTERFACE_H

//Used for inheritance purposes
#include <QMainWindow>

//Forward Class Declarations
class QHBoxLayout;
class QVBoxLayout;
class Playlist;
class QScrollArea;
class PictureBox;
class TextMetadata;
class MediaPlayer;
class SearchWindow;
class QWidget;
class SettingsWindow;
class EqualizerWindow;

/*
   --------------------------------------------
    Interface Class, inherits from QMainWindow
   --------------------------------------------
*/

class Interface : public QMainWindow {
public:
    //Interface Constructor
    Interface();

    //Interface Destructor
    ~Interface();

    //Create a series of buttons to save and import playlists.
    QHBoxLayout* createMainToolbar();

    //Create a series of buttons to edit playlists by adding, removing, and searching a playlist
    QHBoxLayout* createPlaylistToolbar();


protected:
    //Import an MP3 file to add to the playlist.
    void importSong();

    //Import a folder containing MP3 files to add to the playlist.
    void importFolder();

    //Import a playlist to be loaded into the system.
    void importPlaylist();

    //Save a Playlist to the currently selected playlist file.
    void savePlaylist();

    //Export a Playlist to an already existing or specified file.
    void exportPlaylist();

    //Delete a playlist, which removes it from the application and deletes the original playlist.
    void deletePlaylist();

    //Open Equalizer Window when user clicks on Equalizer Button
    void openEqualizerWindow();


private:
    //The currently loaded playlist. Can change when new playlists are imported.
    Playlist* currentPlaylist;

    //The layout that contains the PictureBox (where playlists can import and display playlist images)
    QHBoxLayout* metadataHolder;

    //The layout that contains all of the playlist editing materials. Used to display it in the interface
    QVBoxLayout* playlistEditor;

    //The scrollable area where all of the songs are displayed.
    QScrollArea* songsList;

    //The picture box object that contains an image and button to select a new image. Needs to be removed when a new playlist is imported.
    PictureBox* photoData;

    //The text metadata box that contain QLabels and has to be removed in when a new playlist is imported.
    TextMetadata* textData;

    //The media player element that communicates with the playlist. Needs a reference to run some functions.
    MediaPlayer* player;

    //The search window to find a song in a playlist. Has to be changed when importing a new playlist.
    SearchWindow* searchWindow;

    //The settings window toset and find settings.
    SettingsWindow* settingsWindow;

    EqualizerWindow* equalizerWindow;

};

#endif // INTERFACE_H
