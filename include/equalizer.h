#ifndef EQUALIZER_H
#define EQUALIZER_H

#include<fftw3.h>

class Equalizer {
private:
	float EQUALIZER_MAX_MULT = 5;
	float lowMult, midMult, highMult;

public:
	Equalizer();
	Equalizer(float low, float mid, float high);

	float getLowMult() { return lowMult; }
	void setLowMult(float f);

	float getMidMult() { return midMult; }
	void setMidMult(float f);

	float getHighMult() { return highMult; }
	void setHighMult(float f);
};

#endif // EQUALIZER_H
