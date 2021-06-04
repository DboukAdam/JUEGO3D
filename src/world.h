#include "utils.h"
#include "shader.h"
#include "entity.h"
#include "camera.h"
#include "input.h"


#define MAX_ENTITIES 155
#define MAX_ZOMBIES 100

#pragma once
class World {
public:

	Entity* entities[MAX_ENTITIES];
	Entity* editorEntities[MAX_ENTITIES];
	int numEntity = 0;
	Zombie* zombies[MAX_ZOMBIES];
	Player* player;
	Shader* shader;
	Entity* sky;
	Entity* selectedEntity = NULL;
	uint8* map;

	World(Shader* shader);
	void addEntity(Entity* entity);
	void addZombie(Zombie* zombie);
	void addEditorEntity(Entity* entity);
	void addPlayer(Player* player);
	void disparar();

	void addObjectEditor(Entity* entity, Vector3 dir);
	void selectEntityEditor(Vector3 dir);

	void RenderEntities(Camera* camera);
	void RenderZombies(Camera* camera);

	void RenderBoundingEntities(Camera* camera);
	void RenderBoundingZombies(Camera* camera);
	void saveWorldInfo();
	bool loadWorldInfo();
};
