#ifndef MP3_PLAYER_H
#define MP3_PLAYER_H

#include<fftw3.h>

class Mp3_Player {
private:
	// stuff for FFTs
	fftw_plan *fft, *ifft;
	fftw_complex *inFft, *outFft, *inIfft, *outIfft;
public:
	Mp3_Player();
};

#endif // MP3_PLAYER_H
