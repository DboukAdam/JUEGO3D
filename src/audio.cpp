#include "audio.h"

Audio::Audio() {
	if (BASS_Init == false) {
		std::cout << "AUDIO ERROR: tarjeta de sonido" << std::endl;
	}
	
	sample = BASS_SampleLoad(false, "data/Audio/musicbox-silent-night.wav", 0, 2, 3, 0);
	if (sample == 0) {
		std::cout << "AUDIO ERROR: sample not found" << std::endl;
	}
	
}

Audio::~Audio()
{
}

HCHANNEL Audio::play(float volume) {
	HCHANNEL channel;
	channel = BASS_SetVolume(volume);
	channel = BASS_ChannelPlay(sample, true);
	return channel;
}

void Audio::Stop(HCHANNEL channel) {
	BASS_ChannelStop(channel);
}

Audio* Audio::Get(const char* filename) {

	return nullptr;
}

HCHANNEL* Audio::Play(const char* filename) {
	return nullptr;
}
