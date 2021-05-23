#include "utils.h"
#include "shader.h"
#include "entity.h"
#include "camera.h"

#define MAX_ENTITIES 155
#define MAX_ZOMBIES 30

class World {
public:
	Entity* entities[MAX_ENTITIES];
	Zombie* zombies[MAX_ZOMBIES];
	Player* player;

	World();
	void addEntity(Entity* entity);
	void addZombie(Zombie* zombie);
	void addPlayer(Player* player);
};
