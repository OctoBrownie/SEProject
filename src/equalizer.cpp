#include<fftw3.h>

#include "equalizer.h"


// max muliplier for any equalizer setting
#define EQUALIZER_MAX_MULT 5

// window and buffer size for the FFTs
#define EQUALIZER_WINDOW_SIZE 4096

// hop size between each chunk for the FFT
#define EQUALIZER_HOP_SIZE 512


Equalizer::Equalizer() : Equalizer(0, 0, 0) {}

Equalizer::Equalizer(float low, float mid, float high) :
	lowMult(low), midMult(mid), highMult(high) {
	// checking FFTW works

	fftw_complex *in, *out;
	fftw_plan p;
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*10);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*10);

	fftw_free(in);
	fftw_free(out);
}

void Equalizer::setLowMult(float f) {
	if (f < 0 || f > EQUALIZER_MAX_MULT) return;
	lowMult = f;
}

void Equalizer::setMidMult(float f) {
	if (f < 0 || f > EQUALIZER_MAX_MULT) return;
	midMult = f;
}

void Equalizer::setHighMult(float f) {
	if (f < 0 || f > EQUALIZER_MAX_MULT) return;
	highMult = f;
}
