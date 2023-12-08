#ifndef SONGWIDGET_H
#define SONGWIDGET_H

#include<QWidget>

class Song;

class SongWidget: public QWidget
{
public:
	SongWidget(Song* song);
	void setBackground(int value);
signals:
	void playSong(Song* s);

protected:
	Q_OBJECT
	void mousePressEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
	bool selected;
	Song* fromSong;
};

#endif // SONGWIDGET_H
