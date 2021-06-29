#include "extra/bass.h"
#include "includes.h"
#include <map>
#include <string>

class Audio {
public:
	static std::map<std::string, Audio*> sLoadedAudios;
	HSAMPLE sample;
	HCHANNEL channelSample;

	Audio();
	~Audio();

	HCHANNEL play(float volume);
	bool load(const char* filename);

	static void Stop(HCHANNEL channel);
	static Audio* Get(const char* filename);
	static HCHANNEL* Play(const char* filename);
};