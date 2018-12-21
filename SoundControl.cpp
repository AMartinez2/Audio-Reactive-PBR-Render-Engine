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
	int count = 0;
	int index = 0;
	float temp = 0;
	// We need 10 freq data samples. So every 15 bins, we stash our current accumulation and start a new one.
	float min = 999;
	float max = -999;
	for (int i = 24; i < _fftRange; i++) {
		temp += sqrt(_fft[i]) * (10.0f);
		if (count == 100) {
			_bins[index] = temp / 100;
			if (_bins[index] < min) {
				min = _bins[index];
			}
			if (_bins[index] > max) {
				max = _bins[index];
			}
			index += 1;
			count = 0;
			temp = 0;
		}
		count += 1;
	}
	
	// Normalize: normalized = (x-min(x))/(max(x)-min(x))
	/*
	for (int i = 0; i < 10; i++) {
		_bins[i] = (_bins[i] - min) / (max - min);
		std::cout << _bins[i] << " ";
	}std::cout << "\n";
	*/
	return _bins;
}


// Getters and setters 
int SoundControl::getFFTRange() {
	return _fftRange;
}


void SoundControl::setVolume(float volume) {
	BASS_ChannelSetAttribute(_audioStream, BASS_ATTRIB_VOL, volume);
}