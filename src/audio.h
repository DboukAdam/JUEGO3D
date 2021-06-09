#include "extra/bass.h"
#include "includes.h"

class Audio {
public:
	//static std::map<std::string, Audio*> sLoadedAudios;
	HSAMPLE sample;

	Audio();
	~Audio();

	HCHANNEL play(float volume);

	static void Stop(HCHANNEL channel);
	static Audio* Get(const char* filename);
	static HCHANNEL* Play(const char* filename);
};