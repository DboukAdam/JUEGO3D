#include "utils.h"
#include "shader.h"
#include "entity.h"
#include "camera.h"
#include "input.h"


#define MAX_ENTITIES 1000
#define MAX_ZOMBIES 100
#define MAX_WEAPONS 10
#define CUADRANTS 4

#pragma once
class World {
public:

	Entity* staticEntities[MAX_ENTITIES];
	Entity* dynamicEntities[MAX_ENTITIES];
	Entity* structures[MAX_ENTITIES];
	Entity* decoration[MAX_ENTITIES];
	int numEntity = 0;
	int maxStructure = 0;
	int numStructure = 0;

	int maxDecoration = 0;
	int numDecoration = 0;
	Zombie* zombies[MAX_ZOMBIES];
	Player* player;
	Weapon* weapons[MAX_WEAPONS];
	Shader* shader;
	Entity* sky;
	Entity* selectedEntity = NULL;
	uint8* maps[CUADRANTS];
	Entity* ground;
	bool isStaticObject = false;
	Spawn* spawn1;
	Spawn* spawn2;
	Spawn* spawn3;
	int round;
	int numSpawns = 3;

	World(Shader* shader);
	void addStaticEntity(Entity* entity);
	void addDynamicEntity(Entity* entity);
	void addZombie(Zombie* zombie);
	void addPlayer(Player* player);
	void addWeapon(Weapon* weapon);
	void addStructure(Entity* entity);
	void addDecoration(Entity* entity);
	void addObjectEditor(Entity* entity, Vector3 dir);
	//Inits
	void initCamera(Camera* camera);
	void initPlayer(Vector3 pos, Mesh* mesh, Texture* text);
	void initSky(Mesh* mesh, Texture* texture);
	void initGround(Texture* texture);
	void initWeapon(Weapon* weapon); //NO HACE NADA
	void initMap();
	//Loads
	void loadDecoration();
	void loadStructure();
	//Otro
	void disparar();
	void selectEntityEditor(Vector3 dir);
	void deleteEntity(Entity* entity);
	void moveZombies();
	//Renders
	void RenderStatic(Camera* camera);
	void RenderDynamic(Camera* camera);
	void RenderZombies(Camera* camera, float time);
	void RenderPlayer(Camera* camera);
	void RenderBoundingStatic(Camera* camera);
	void RenderBoundingDynamic(Camera* camera);
	void RenderBoundingZombies(Camera* camera);
	//WorldInfos
	void saveWorldInfo(std::string filename);
	bool loadWorldInfo(std::string filename);
	void initSpawn();
	void StartRound(float time);
};
