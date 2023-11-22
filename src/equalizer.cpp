#include<fftw3.h>

#include "equalizer.h"

Equalizer::Equalizer() {
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
