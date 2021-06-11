#include "audio.h"
#include <cassert>

std::map<std::string, Audio*> Audio::sLoadedAudios;

Audio::Audio() {
	
	sample = 0;
}

Audio::~Audio() {
}

HCHANNEL Audio::play(float volume) {
	if (this == nullptr) {
		std::cout << "AUDIO ERROR: could not play, audio is NULL" << std::endl;
		return NULL;
	}
	HCHANNEL channel;
	channel = BASS_SetVolume(volume);
	channel = BASS_ChannelPlay(sample, true);
	return channel;
	
}

bool Audio::load(const char* filename) {
	if (BASS_Init == false) {
		std::cout << "AUDIO ERROR: tarjeta de sonido" << std::endl;
	}
	sample = BASS_SampleLoad(false, filename, 0, 0, 3, 0);
	if (sample == 0) {
		std::cout << "AUDIO ERROR: file not found" << BASS_ErrorGetCode() << std::endl;
		return false;
	}
	sLoadedAudios[filename] = this;
	return true;
}

void Audio::Stop(HCHANNEL channel) {
	BASS_ChannelStop(channel);
}

Audio* Audio::Get(const char* filename) {
	assert(filename);

	//check if loaded
	auto it = sLoadedAudios.find(filename);
	if (it != sLoadedAudios.end())
		return it->second;

	//load it
	Audio* audio = new Audio();
	if (!audio->load(filename))
	{
		delete audio;
		return NULL;
	}
	return audio;
}

HCHANNEL* Audio::Play(const char* filename) {
	assert(filename);

	Audio* audio = Audio::Get(filename);
	if (audio == NULL) {
		return NULL;
	}
	HCHANNEL channel = audio->play(1);
	return &channel;
}
