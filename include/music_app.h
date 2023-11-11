#ifndef MUSIC_APP_H
#define MUSIC_APP_H

#include<QWidget>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QLineEdit>

class Music_App : public QWidget {
private:
    Q_OBJECT
	QMediaPlayer* mediaPlayer;
	QAudioOutput* audioOutput;
	QLineEdit* editor;
public:
    explicit Music_App(QWidget *parent = nullptr);
	virtual ~Music_App();
	// virtual void keyPressEvent(QKeyEvent* event);

public slots:
	void startMusic();
	void startStop();
signals:
};

#endif // MUSIC_APP_H
