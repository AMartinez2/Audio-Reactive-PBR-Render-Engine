#ifndef SOUND_CONTROL_H
#define SOUND_CONTROL_H


#include <BASS/bass.h>

#include <iostream>
#include <string>


class SoundControl
{
public:
	SoundControl(const char* soundFile);
	~SoundControl();

	void playAudio();
	float* processBins();
	//--- TODO: This function will return as many combined bin ranges as the given `binLevel` argumnet
	//float* getControledBins(int binLevel);
public:
	const int _numBins;
	const int _fftRange;
	HSTREAM _audioStream;
	int _fftSampleRange;
	float* _bins;
	float* _prevBins;
	float* _fft;
};

#endif