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


void SoundControl::stopAudio() {
	BASS_ChannelStop(_audioStream);
}


float* SoundControl::processBins() {
	BASS_ChannelGetData(_audioStream, _fft, BASS_DATA_FFT2048);
	// Scale FFT values
	float min = 999;
	float max = -999;
	for (int i = 0; i < _fftRange; i++) {
		_fft[i] = sqrt(_fft[i]) * (5.0f);
		if (_fft[i] < min) {
			min = _fft[i];
		}
		if (_fft[i] > max) {
			max = _fft[i];
		}
	}
	// Normalize: normalized = (x-min(x))/(max(x)-min(x))
	for (int i = 0; i < _fftRange; i++) {
		_fft[i] = (_fft[i] - min) / (max - min);
	}

	return _fft;
}


// Getters and setters 
int SoundControl::getFFTRange() {
	return _fftRange;
}
