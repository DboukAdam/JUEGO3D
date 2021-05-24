#include "utils.h"
#include "shader.h"
#include "entity.h"
#include "camera.h"

#define MAX_ENTITIES 155
#define MAX_ZOMBIES 10

class World {
public:
	Entity* entities[MAX_ENTITIES];
	Zombie* zombies[MAX_ZOMBIES];
	Player* player;
	Shader* shader;

	World(Shader* shader);
	void addEntity(Entity* entity);
	void addZombie(Zombie* zombie);
	void addPlayer(Player* player);
	void createZombies();
	void disparar();
};
