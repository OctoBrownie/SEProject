#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QVariant>

class MediaPlayer: public QWidget
{
public:
    MediaPlayer(QWidget* parent=nullptr);

protected:
    void swapLoop();

private:
    QString* current_song_title;
    QString* current_song_album;
    QString* current_song_artist;
    QVariant* current_song_art;
    bool isLooped = 0;
};

#endif // MEDIAPLAYER_H
