#include "soundControl.h"


SoundControl::SoundControl(const char* soundFile) : _numBins(512), _fftRange(1024) {
	// Initialize BASS
	if (!BASS_Init(-1, 44100, 0, 0, 0)) {
		std::cout << "Bass failed to initialise" << std::endl;
		return;
	}

	// Read in the sound file into an audio stream
	HSTREAM stream;
	stream = BASS_StreamCreateFile(false, soundFile, 0, 0, BASS_STREAM_PRESCAN);
	if (!stream) {
		std::cout << "Bass failed to open audio file" << std::endl;
		return;
	}

	// Assign newly created audio stream to the SoundControl audio stream member variable
	_audioStream = stream;

	// Initialize the rest of the private member variables
	//--- TODO: these sizes may need to be adaptive to the parameters of the given audio file
	_fftSampleRange = _fftRange / (float)_numBins;
	_bins = new float[_numBins];
	_prevBins = new float[_numBins];
	_fft = new float[_fftRange];
	return;
}


SoundControl::~SoundControl() {
	// Make sure to clean up
	delete[] _bins;
	delete[] _prevBins;
	delete[] _fft;
}


void SoundControl::playAudio() {
	BASS_ChannelPlay(_audioStream, false);
}


float* SoundControl::processBins() {
	BASS_ChannelGetData(_audioStream, _fft, BASS_DATA_FFT2048);

	//freqPreprocess();
	
	// Update the old bins
	std::copy(_bins, _bins + _numBins, _prevBins);

	// Update the new bins
	for (int i = 0; i < _numBins; i++) {
		_bins[i] = 0.0f;

		// Get upper and lower values for FFT sampling
		int lower = roundf(_fftSampleRange * (float)(i));
		int upper = roundf(_fftSampleRange * (float)(i + 1));

		// Average of FFT values 
		for (int j = lower; j < upper; j++)
			_bins[i] += _fft[j];
		_bins[i] /= _fftSampleRange;

		// Smooth by averaging old and new bins
		_bins[i] = (_bins[i] + _prevBins[i]) * 0.5f;
	}
	return _bins;
}


void SoundControl::freqPreprocess() {
	//TODO NORMALISE AFTER SQRT
	// Scale FFT values
	for (int i = 0; i < _fftSampleRange; i++)
		_fft[i] = sqrt(_fft[i]) * (5.0f * 800.0f);
}


// Getters and setters 
int SoundControl::getNumBins() {
	return _numBins;
}
