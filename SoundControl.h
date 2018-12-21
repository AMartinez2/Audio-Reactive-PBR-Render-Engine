#ifndef SOUND_CONTROL_H
#define SOUND_CONTROL_H


#include <BASS/bass.h>

#include <iostream>
#include <string>
#include <limits>


class SoundControl
{
public:
	// Constructor with sound file location as argument
	SoundControl(const char* soundFile);
	// Deconstructor to clean up our buffers, bins, etc...
	~SoundControl();
	// Play the audio stream
	void playAudio();
	// Stop the audio stream
	void stopAudio();
	// Return all audio frequency bins
	float* processBins();
	// Getters and setters
	int getFFTRange();
	void setVolume(float volume);

private:
	// Number of bins, basically the resolution/granularity of the frequency reference
	int _numBins;
	// Forward Fourier Transformation resolution/granularity
	int _fftRange;
	// Stream to hold the audio data
	HSTREAM _audioStream;
	// _fftSampleRange = _fftRange / _numBins; 
	int _fftSampleRange;
	// Arrays to hold stream information
	float _bins[10];
	float* _prevBins;
	float* _fft;
};

#endif