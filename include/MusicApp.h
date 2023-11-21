#ifndef MUSICAPP_H
#define MUSICAPP_H

#include<QWidget>
class QMediaPlayer;
class QAudioOutput;
class QLineEdit;

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

#endif // MUSICAPP_H
