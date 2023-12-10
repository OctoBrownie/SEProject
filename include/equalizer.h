#ifndef EQUALIZER_H
#define EQUALIZER_H

// number of taps in the FIR filter
#define EQUALIZER_NUM_TAPS 10

/**
 * Handles equalizing audio data for playback on the fly.
 */
class Equalizer {
private:
	// multiplier for each frequency band
	float lowMult, midMult, highMult;

	// coefficients (taps) for each frequency band
	float *lowTaps, *midTaps, *highTaps;

	// shift registers (one for each channel)
	float** data;

	float TEMPVAR;

	int numChannels;

public:
	Equalizer(int channels);
	Equalizer(int channels, float low, float mid, float high);

	~Equalizer();

	float getLowMult() const { return lowMult; }
	void setLowMult(float f);

	float getMidMult() const { return midMult; }
	void setMidMult(float f);

	float getHighMult() const { return highMult; }
	void setHighMult(float f);

	float getSample(int channel, float f);
	void flush();
};

#endif // EQUALIZER_H
