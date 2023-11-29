#ifndef SONG_H
#define SONG_H

#include<QWidget>

class QString;
class QImage;
class QPushButton;

//The Song class, inherits from the QWidget class
class Song: public QWidget
{
    //Need this macro to send signals
    Q_OBJECT

public:
    //Constructor
    Song(QString path);

    //Getters
    QString getSongPath();
    QString getSongTitle();
    QString getArtist();
    QString getAlbum();
    QImage* getArt();
    qint64 getDuration();
    qint64 getPosition();
    bool getSelected();

    QPushButton* getUpButton();
    QPushButton* getDownButton();

    //Setters
    void setPosition(qint64 newPosition);
    void setSelected(bool selected);

protected:
    //Determines what happens when a mouse is pressed on the widget
    void mousePressEvent(QMouseEvent *event) override;

    //Method to create the widget, stored in this widget. The output is stored in the this->widget. This is only done to allow accurate highlighting when selected by a user.
    void makeLayout();

signals:
    //A signal that is sent out after this widget is clicked on. This tells the playlist what the new selected song position is.
    void selectedSong(int pos, bool move);

    //A signal that is sent out when one of the buttons (up (1) or down (0)) is clicked, sending the song position and button type.
    void buttonClicked(int pos, int status);

private:
    //Path to the song, used by the MP3 player to play the song later
    QString songPath;

    //Title of the song
    QString title;

    //The artists of the song (can be more than 1, but stored in a string)
    QString artists;

    //Album from the song
    QString album;

    //Album art
    QImage albumArt;

    //How long the song is, in seconds.
    qint64 duration;

    //The position of the song in the playlist
    qint64 pPosition;

    //Determines if a song is selected or not; not needed in future implementations
    bool selected = false;

    //This stores the widget that represents the song, that is stored inside the song Widget. This is only done to allow proper highlighting when clicked. It's a necessary evil.
    QWidget* widget;

    //The buttons to move the songs up and down in the list
    QPushButton* upButton;
    QPushButton* downButton;
};

#endif // SONG_H
