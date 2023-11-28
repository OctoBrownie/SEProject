#ifndef CUSTOMSONGWIDGET_H
#define CUSTOMSONGWIDGET_H

#include<QWidget>

class Song;

class CustomSongWidget: public QWidget
{
public:
	CustomSongWidget(Song* song);
	void setBackground(int value);

protected:
	void mousePressEvent(QMouseEvent *event) override;

private:
	bool selected;
	Song* fromSong;
};

#endif // CUSTOMSONGWIDGET_H
