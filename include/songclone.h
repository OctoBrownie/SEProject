#ifndef SONGCLONE_H
#define SONGCLONE_H

#include "song.h"
#include <QBoxLayout>
#include <QLabel>

class SongClone: public QWidget
{
    Q_OBJECT

public:
    SongClone(Song* song);

protected:
    void selectedSong(int pos);

    void setSelected(bool selected);

    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked();

private:

    QWidget* widget;

    Song* song;

    bool selected;
};

#endif // SONGCLONE_H
