#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include<fftw3.h>

extern "C" {
	#include<libavformat/avformat.h>
}

#include<QWidget>
#include<QAudioSink>
#include<QLineEdit>

#include<QAudioOutput>
#include<QByteArray>
#include<QBuffer>

class Mp3_Player : public QWidget {
private:
	Q_OBJECT
	// stuff for FFTs
	fftw_plan *fft, *ifft;
	fftw_complex *inFft, *outFft, *inIfft, *outIfft;

	// stuff for playing music
	QAudioSink* aSink;
	QLineEdit* lineEdit;

	QBuffer* musicBuffer;
	QByteArray* musicByteArray;

	// populating buffer from line edit
	int populateBuffer();
public:
	explicit Mp3_Player(QWidget *parent = nullptr);
	virtual ~Mp3_Player();

public slots:
	void startMusic();
	void printSinkError();
signals:
};

#endif // MP3_PLAYER_H
