#ifndef CUSTOMSONGWIDGET_H
#define CUSTOMSONGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

class Song;

class CustomSongWidget: public QWidget
{
public:
    CustomSongWidget(Song* song);
    void setBackground(int value);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    int selected = 0;
    Song* fromSong;
};

#endif // CUSTOMSONGWIDGET_H
