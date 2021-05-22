#include "world.h"

World::World() {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		entities[i] = NULL;
	}
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		zombies[i] = NULL;
	}
}

void World::addEntity(Entity* entity) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (entities[i] == NULL){
			entities[i] = entity;
			break;
		}
	}
}
void World::addZombie(Zombie* zombie) {
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		if (zombies[i] == NULL) {
			zombies[i] = zombie;
			break;
		}
	}
}

void World::addPlayer(Player* player) {
	this->player = player;
}
