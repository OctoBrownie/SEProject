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

    // The layout of the metadata section of the application
    QHBoxLayout* metadataHolder = new QHBoxLayout;

    // The picture data and the text data in the UI
    PictureBox* photoData = new PictureBox;
    TextMetadata* textData = new TextMetadata;

    // Where the songs will eventually be listed
    QScrollArea* songsList = new QScrollArea;

    QString playPath = QString("C:/Users/astro/Desktop/MyPlaylist.pa");
    Playlist myPlaylist = Playlist(&playPath);
    QWidget* scrollData = myPlaylist.createPlaylistOutput();
    songsList->setWidget(scrollData);


    // Buttons that allow playlist editing
    EditingButtons* playlistEditorButtons = new EditingButtons;

    QVBoxLayout* MP3ViewerLayout = new QVBoxLayout();
    // Buttons that provide central functionality to the application
    ApplicationButtons* applicationButtons = new ApplicationButtons;

    MediaPlayer* player = new MediaPlayer();
    QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    MP3ViewerLayout->addLayout(applicationButtons);
    MP3ViewerLayout->addSpacerItem(spacer);
    MP3ViewerLayout->addWidget(player);

    // Add buttons to viewer
    MP3Viewer->addLayout(MP3ViewerLayout);

    // Add data to the holder component
    metadataHolder->addLayout(photoData);
    metadataHolder->addLayout(textData);

    // Add data to the editor component
    playlistEditor->addLayout(metadataHolder);
    playlistEditor->addLayout(playlistEditorButtons);
    playlistEditor->addWidget(songsList);

    // Make widgets that are designed based on editor and MP3 player
    QWidget* sideWidget = new QWidget;
    sideWidget->setLayout(playlistEditor);

    QWidget *window = new QWidget;
    window->setLayout(MP3Viewer);



    // Set the editor pane to the be the playlist editor
    QDockWidget* playlistEditPane = new QDockWidget;
    playlistEditPane->setWidget(sideWidget);
    playlistEditPane->setMinimumWidth(500);

    // Compile the interface
    Interface* mainWindow = new Interface(window, playlistEditPane);

    mainWindow->show();



    return app.exec();
}
