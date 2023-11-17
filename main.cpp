#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QFrame>
#include <QMainWindow>
#include <QLayout>
#include <QLineEdit>
#include <QDockWidget>
#include <QScrollArea>
#include <QLabel>
#include <interface.h>
#include <picturebox.h>
#include <textmetadata.h>
#include <applicationbuttons.h>
#include <editingbuttons.h>
#include <mediaplayer.h>
#include <song.h>
#include <playlist.h>



int main(int argc, char **argv) {
    QApplication app (argc, argv);

    // The layout of the editor portion of the layout application
    QVBoxLayout* playlistEditor = new QVBoxLayout;
    playlistEditor->setAlignment(Qt::AlignTop);

    // The layout of the MP3 viewer portion of the layout application
    QVBoxLayout* MP3Viewer = new QVBoxLayout;
    MP3Viewer->setAlignment(Qt::AlignTop);

    QVBoxLayout* MP3ViewerLayout = new QVBoxLayout();
    // Buttons that provide central functionality to the application
    ApplicationButtons* applicationButtons = new ApplicationButtons;

    MediaPlayer* player = new MediaPlayer();
    QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    MP3ViewerLayout->addLayout(applicationButtons);
    MP3ViewerLayout->addSpacerItem(spacer);
    MP3ViewerLayout->addWidget(player);
    MP3Viewer->addLayout(MP3ViewerLayout);

    QWidget *window = new QWidget;
    window->setLayout(MP3Viewer);


    QString playPath = QString("C:/Users/astro/Desktop/MyPlaylist.pa");
    Playlist myPlaylist = Playlist(&playPath);



    //This box holds the image and the text (title, user, and duration)
    QHBoxLayout* metadataHolder = new QHBoxLayout;
    // The picture data and the text data in the UI
    PictureBox* photoData = myPlaylist.getPictureBox();
    TextMetadata* textData = myPlaylist.getTextMetadata();
    // Add data to the holder component
    metadataHolder->addLayout(photoData);
    metadataHolder->addLayout(textData);

    // These are the buttons that allow playlist editing
    EditingButtons* playlistEditorButtons = myPlaylist.getEditingButtons();

    //This is the scrollable list of the songs in the playlist.
    QScrollArea* songsList = new QScrollArea;
    songsList->setWidget(myPlaylist.getListGUI());


    // Add data to the editor component
    playlistEditor->addLayout(metadataHolder);
    playlistEditor->addLayout(playlistEditorButtons);
    playlistEditor->addWidget(songsList);

    QWidget* playlistEditWidget = new QWidget();
    playlistEditWidget->setLayout(playlistEditor);

    // Set the editor pane to the be the playlist editor
    QDockWidget* playlistEditPane = new QDockWidget;
    playlistEditPane->setWidget(playlistEditWidget);
    playlistEditPane->setMinimumWidth(500);
    Interface* mainWindow = new Interface(window, playlistEditPane);


    mainWindow->show();

    Song newSong("C:/Users/astro/Downloads/Fluffing-a-Duck(chosic.com).mp3");
    myPlaylist.addSong(&newSong);
    QString* saveFile = new QString("C:/Users/astro/Desktop/SecondPlaylist.pa");
    myPlaylist.savePlaylist(saveFile);

    return app.exec();
}

