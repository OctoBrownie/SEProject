#ifndef SONGCLONE_H
#define SONGCLONE_H


#include <QWidget>
class Song;

//SongClone Class
class SongClone: public QWidget
{
    Q_OBJECT

public:
    //SongClone constructor
    SongClone(Song* song);

protected:
    //Set the selected status of the
    void setSelected(bool selected);

    //What to do when the item is clicked on (turns blue, notifies playlist)
    void mousePressEvent(QMouseEvent *event);

signals:
    //Signal to set the status of the selected song in the playlist based on these fake results
    void clicked();

private:

    //GUI widget
    QWidget* widget;

    //The Song widget that this is a copy of
    Song* song;

    //True/false if it is selected or not
    bool selected;
};

#endif // SONGCLONE_H
