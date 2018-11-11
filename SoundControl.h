#ifndef SOUND_CONTROL_H
#define SOUND_CONTROL_H


#include <BASS/bass.h>

#include <iostream>
#include <string>


class SoundControl
{
public:
	// Constructor with sound file location as argument
	SoundControl(const char* soundFile);
	// Deconstructor to clean up our buffers, bins, etc...
	~SoundControl();
	// Play the audio stream
	void playAudio();
	// Return all audio frequency bins
	float* processBins();

	// Getters and setters
	int getNumBins();
	//void setNumBins(int num); // If implemented, will need functionality to update all associated variables

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
	float* _bins;
	float* _prevBins;
	float* _fft;

	// Function to encapsulate all the necessary math applied to stream data prior the FFT
	void freqPreprocess();
};

#endif