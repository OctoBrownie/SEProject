#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QLabel>

class MediaPlayer: public QWidget
{
public:
    MediaPlayer(QWidget* parent=nullptr);
};

#endif // MEDIAPLAYER_H
