#include<fftw3.h>
#include<QAudioDecoder>

#include "mp3_player.h"

Mp3_Player::Mp3_Player() {
	fftw_complex *in, *out;
	// fftw_plan p;
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*10);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*10);

	fftw_free(in);
	fftw_free(out);
}
