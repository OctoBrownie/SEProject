#include<cmath>
#include<iostream>

#include "equalizer.h"


// max muliplier for any equalizer setting
#define EQUALIZER_MAX_MULT 5

// cutoff freq (kHz) for the low pass filter/band pass (lower)
#define EQUALIZER_LOW_FREQ 0.1

// cutoff freq (kHz) for the high pass filter/band pass (upper)
#define EQUALIZER_HIGH_FREQ 30


Equalizer::Equalizer(int channels) : Equalizer(channels, 1, 1, 1) {}

Equalizer::Equalizer(int channels, float low, float mid, float high) :
	lowMult{low}, midMult{mid}, highMult{high}, numChannels{channels} {

	TEMPVAR = 0;

	data = new float*[numChannels];
	lowTaps = new float[EQUALIZER_NUM_TAPS];
	midTaps = new float[EQUALIZER_NUM_TAPS];
	highTaps = new float[EQUALIZER_NUM_TAPS];

	// init data arrays
	for (int i = 0; i < numChannels; i++) {
		data[i] = new float[EQUALIZER_NUM_TAPS];
		for (int j = 0; j < EQUALIZER_NUM_TAPS; j++) data[i][j] = 0;
	}

	// filter constants
	float m;

	// TODO: responsive sample rate?
	float lambda = 2 * M_PI * EQUALIZER_LOW_FREQ / 44.1;
	float phi = 2 * M_PI * EQUALIZER_HIGH_FREQ / 44.1;

	// init filter coeff arrays
	for (int i = 0; i < EQUALIZER_NUM_TAPS; i++) {

		// calculate coeffs for each filter
		m = i - (EQUALIZER_NUM_TAPS - 1)/2.0;

		// low pass
		lowTaps[i] = m == 0 ? lambda/M_PI : sin(lambda*m)/(M_PI*m);

		// high pass
		highTaps[i] = m == 0 ? (1 - phi)/M_PI : -sin(phi*m)/(M_PI*m);

		// band pass
		midTaps[i] = m == 0 ? (phi - lambda)/M_PI : (sin(m*phi) - sin(m*lambda))/(m * M_PI);

		std::cout << lowTaps[i] << ' ' << midTaps[i] << ' ' << highTaps[i] << std::endl;
	}

}

Equalizer::~Equalizer() {
	delete data;
	delete lowTaps;
	delete midTaps;
	delete highTaps;
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

float Equalizer::getSample(int ch, float f) {
//	if (ch != 0) return 0;

//	float RC = 1.0/(EQUALIZER_LOW_FREQ*2*M_PI);
//	float dt = 1.0/44.1;
//	float alpha = dt/(RC + dt);

//	TEMPVAR = TEMPVAR + alpha*(f - TEMPVAR);

//	return TEMPVAR;

	// shift the shift register
	for (int i = EQUALIZER_NUM_TAPS - 1; i > 0; --i) {
		data[ch][i] = data[ch][i - 1];
	}
	data[ch][0] = f;

	// convolve the filter and data
	float res = 0;
	for (int i = 0; i < EQUALIZER_NUM_TAPS; i++) {
		res += data[ch][i]*lowTaps[i]*1.5;
	}
	return res;
}

void Equalizer::flush() {
	for (int i = 0; i < numChannels; i++)
		for (int j = 0; j < EQUALIZER_NUM_TAPS; j++)
			data[i][j] = 0;
}
