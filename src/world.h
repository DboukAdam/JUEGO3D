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
	Camera* camera;
	Shader* shader;

	World(Camera* camera, Shader* shader);
	void addEntity(Entity* entity);
	void addZombie(Zombie* zombie);
	
};
