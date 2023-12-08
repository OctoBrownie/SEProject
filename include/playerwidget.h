#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

// Qt
#include <QWidget>
class QLabel;

// project
class MusicPlayer;
class Song;

class PlayerWidget : public QWidget {
protected:
	Q_OBJECT

	MusicPlayer* player;

	QLabel *songName, *albumArt;

	QPixmap defaultImage;
public:
	explicit PlayerWidget(MusicPlayer* p, QWidget *parent = nullptr);

public slots:
	void updateLabels();
signals:

};

#endif // PLAYERWIDGET_H
