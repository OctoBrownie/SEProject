#ifndef APP_H
#define APP_H

#include<QWidget>
class QMediaPlayer;
class QAudioOutput;
class QLineEdit;

class App : public QWidget {
private:
    Q_OBJECT
	QMediaPlayer* mediaPlayer;
	QAudioOutput* audioOutput;
	QLineEdit* editor;
public:
	explicit App(QWidget *parent = nullptr);
	virtual ~App();
	// virtual void keyPressEvent(QKeyEvent* event);

public slots:
	void startMusic();
	void startStop();
signals:
};

#endif // APP_H
