#include "utils.h"
#include "shader.h"
#include "entity.h"

#define MAX_ENTITIES 155
#define MAX_ZOMBIES 30

class World {
public:
	Entity* entities[MAX_ENTITIES];
	Zombie* zombies[MAX_ZOMBIES];

	World(Entity* entities[], Zombie* zombies[]) {
		
	}

	Shader* shader;
};
