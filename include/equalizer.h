#ifndef EQUALIZER_H
#define EQUALIZER_H

#include<fftw3.h>

/**
 * Handles equalizing audio data for playback on the fly.
 */
class Equalizer {
private:
	// multiplier for each frequency band
	float lowMult, midMult, highMult;

	const fftw_plan *fft, *ifft;

public:
	// default equalizer constructor
	Equalizer();
	Equalizer(float low, float mid, float high);

	float getLowMult() const { return lowMult; }
	void setLowMult(float f);

	float getMidMult() const { return midMult; }
	void setMidMult(float f);

	float getHighMult() const { return highMult; }
	void setHighMult(float f);

	void equalize();
	void flushBuffer();
};

#endif // EQUALIZER_H
