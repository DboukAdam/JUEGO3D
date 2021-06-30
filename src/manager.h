#ifndef Manager_HEADER
#define Manager_HEADER

class Manager {
public:
	int round;
	int zombiesAlive;
	int zombiesPerRound;
	float startedRoundTime;
	int spawnedZombies;

	void update();
	void initRound();
};

#endif