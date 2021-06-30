#include "manager.h"
#include "game.h"

void Manager::update() {
	Game* game = Game::instance;
	World* world = game->currentWorld;
	int cooldownRound = 5;
	if (world->zombiesAlive() == 0 && startedRoundTime + cooldownRound < game->time) initRound();
	int zombieLive = 10 + round * 2;
	float zombieVel = 0.1 + random(0, 0.01 * round);
	world->spawnZombies(zombiesPerRound - spawnedZombies, zombieLive, zombieVel, game->time);
	world->moveZombies();
	world->collisionPlayerZombie();
	if (world->player->vida <= 0) game->setEndStage();
}

void Manager::initRound() {
	startedRoundTime = Game::instance->time;
	round++;
	zombiesPerRound =  1; // 10 + round * 2
	if (zombiesPerRound >= 100) zombiesPerRound = 100;
	zombiesAlive = zombiesPerRound;
	spawnedZombies = 0;
}
