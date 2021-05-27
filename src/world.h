#include "utils.h"
#include "shader.h"
#include "entity.h"
#include "camera.h"
#include "input.h"

#define MAX_ENTITIES 155
#define MAX_ZOMBIES 100

class World {
public:
	Entity* entities[MAX_ENTITIES];
	Zombie* zombies[MAX_ZOMBIES];
	Player* player;
	Shader* shader;
	Entity* sky;
	Entity* cesped;
	Entity* crossHair;
	Entity* selectedEntity = NULL;
	uint8* map;

	World(Shader* shader);
	void addEntity(Entity* entity);
	void addZombie(Zombie* zombie);
	void addPlayer(Player* player);
	void disparar();

	void addObjectEditor(Mesh* mesh, Texture* texture, Vector3 dir);
	void selectEntityEditor(Vector3 dir);
};
