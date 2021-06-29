#include "manager.h"
#include "game.h"

void Manager::update() {
	Game* game = Game::instance;
	World* world = game->currentWorld;
	int zombiesAlive = world->zombiesAlive();
	if (zombiesAlive == 0) initRound();
	int zombiesPerRound = 10 + round * 2;
	int zombieLive = 10 + round * 2;

	world->spawnZombies(zombiesPerRound - zombiesAlive, zombieLive, game->time);
	world->moveZombies();
}

void Manager::initRound() {
	round++;
}
