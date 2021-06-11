#include "world.h"

World::World(Shader* shader) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		staticEntities[i] = NULL;
	}
	for (int i = 0; i < MAX_ENTITIES; i++) {
		dynamicEntities[i] = NULL;
	}
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		zombies[i] = NULL;
	}
	for (int i = 0; i < MAX_ENTITIES; i++) {
		structures[i] = NULL;
	}
	for (int i = 0; i < MAX_ENTITIES; i++) {
		decoration[i] = NULL;
	}
	this->shader = shader;
	this->player = NULL;
	this->ground = NULL;
	this->sky = NULL;
	this->map = NULL;
}

//ADDITION OF ENTITIES IN THEIR CORRESPONDING LIST
void World::addStaticEntity(Entity* entity) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (staticEntities[i] == NULL){
			staticEntities[i] = entity;
			break;
		}
	}
}
void World::addDynamicEntity(Entity* entity) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (dynamicEntities[i] == NULL) {
			dynamicEntities[i] = entity;
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

void World::addStructure(Entity* entity) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (structures[i] == NULL) {
			structures[i] = entity;
			break;
		}
	}
}

void World::addDecoration(Entity* entity)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (decoration[i] == NULL) {
			decoration[i] = entity;
			break;
		}
	}
}

void World::addObjectEditor(Entity* entity, Vector3 dir) {
	Camera* camera = Camera::current;
	Vector3 origin = camera->eye;
	Vector3 pos = RayPlaneCollision(Vector3(0, 0, 0), Vector3(0, 1, 0), origin, dir);
	Entity* copia = new Entity(0, 0, 0, Matrix44());
	copia->copy(entity);
	copia->m.setTranslation(pos.x, pos.y, pos.z);
	if (isStaticObject) {
		addStaticEntity(copia);
	}
	else {
		addDynamicEntity(copia);
	}
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//INIT BASIC THINGS OF THE WORLD
void World::initCamera(Camera* camera) {
	Camera::current = camera;
	int window_width = 800;
	int window_height = 600;
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
	Vector3 eye = player->m * Vector3(0.f, 1.f, 0.f);
	Vector3 center = player->m * Vector3(0.f, 5.f, -1.f);
	Vector3 up = player->m.rotateVector(Vector3(0.f, 1.f, 0.f));
	camera->lookAt(eye, center, up);
}

void World::initPlayer(Vector3 pos, Mesh* mesh, Texture* texture) {
	Matrix44 m;
	m.setTranslation(pos.x, pos.y, pos.z);
	Player* player = (Player*) new Entity(pos, m);
	player->mesh = mesh;
	player->texture = texture;
	player->setVel(2.0f);
	addPlayer(player);
}

void World::initSky(Mesh* mesh, Texture* texture) {
	Matrix44 m;
	Vector3 playerPos = player->getPos();
	m.setTranslation(playerPos.x, playerPos.y, playerPos.z);
	sky = new Entity(playerPos, m);
	sky->mesh = mesh;
	sky->texture = texture;
}

void World::initGround(Texture* texture) {
	Matrix44 m;
	m.setTranslation(0, 0, 0);
	ground = new Entity(0, 0, 0, m);
	ground->mesh = new Mesh();
	ground->mesh->createPlane(2000);
	ground->texture = texture;

}

void World::initWeapon(Weapon* weapon) {

}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

//BASIC FUNCTIONALITIES OF EACH WORLD, FOR PLAYSTAGE AND ALSO EDITOR STAGE
void World::disparar() {
	Camera* camera = Camera::current;
	Vector3 colPoint;
	Vector3 colNormal;
	Vector3 origin = camera->eye;
	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, 800, 600);
	float maxDistance = 100;
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		Zombie* zombie = zombies[i];
		if (zombie == NULL)	break;
		if (zombie->mesh->testRayCollision(zombie->m, origin, dir, colPoint, colNormal, maxDistance, false)) {
			//zombie->~Zombie();
			std::cerr << "Buena punteria!" << std::endl;;
		}
	}
}

void World::selectEntityEditor(Vector3 dir){
	Camera* camera = Camera::current;
	Vector3 origin = camera->eye;
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		Vector3 col;
		Vector3 normal;

		if (isStaticObject) {
			Entity* current = staticEntities[i];
			if (current == NULL) break;
			if (!current->mesh->testRayCollision(current->m, origin, dir, col, normal, 10000)) continue;
			if (selectedEntity != NULL) selectedEntity->bounding = false;
			current->bounding = true;
			selectedEntity = current;
			break;
		}
		else {
			Entity* current = dynamicEntities[i];
			if (current == NULL) break;
			if (!current->mesh->testRayCollision(current->m, origin, dir, col, normal, 10000)) continue;
			if (selectedEntity != NULL) selectedEntity->bounding = false;
			current->bounding = true;
			selectedEntity = current;
			break;
		}
	}
}
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::...

//RENDERS OF EVERYTHING NECESSARY
void World::RenderStatic(Camera* camera)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* entity = staticEntities[i];
		if (entity == NULL) {
			break;
		}
		BoundingBox currentBox = transformBoundingBox(entity->m, entity->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		entity->render(shader);

	}
}
void World::RenderDynamic(Camera* camera)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* entity = dynamicEntities[i];
		if (entity == NULL) {
			break;
		}
		BoundingBox currentBox = transformBoundingBox(entity->m, entity->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		entity->render(shader);

	}
}

void World::RenderZombies(Camera* camera)
{

	for (int i = 0; i < MAX_ZOMBIES; i++) {
		Zombie* zombie = zombies[i];
		if (zombie == NULL) {
			break;
		}
		BoundingBox currentBox = transformBoundingBox(zombie->m, zombie->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		zombie->render(shader);

	}
}

void World::RenderBoundingStatic(Camera* camera)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* entity = staticEntities[i];
		if (entity == NULL) break;
		BoundingBox currentBox = transformBoundingBox(entity->m, entity->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		if (entity->bounding)entity->mesh->renderBounding(entity->m);
	}
}

void World::RenderBoundingDynamic(Camera* camera)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* entity = dynamicEntities[i];
		if (entity == NULL) break;
		BoundingBox currentBox = transformBoundingBox(entity->m, entity->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		if (entity->bounding)entity->mesh->renderBounding(entity->m);
	}
}

void World::RenderBoundingZombies(Camera* camera)
{
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		Zombie* zombie = zombies[i];
		if (zombie == NULL) break;
		BoundingBox currentBox = transformBoundingBox(zombie->m, zombie->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		if (zombie->bounding) zombie->mesh->renderBounding(zombie->m);
	}
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/*
//FUNCTIONS IN ORDER TO SAVE AND LOAD WORLDS
void World::saveWorldInfo()
{
	World world_info = World(shader);
	//fill here game_info with all game data
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (staticEntities[i] != NULL) {

		//copia de la existent
		Entity* copia = new Entity(0, 0, 0, Matrix44());
		copia->copy(this->staticEntities[i]);

		//nova entity per la info
		Entity* info = new Entity(0, 0, 0, Matrix44());

		//asignem la copia
		world_info.staticEntities[i] = info;
		world_info.staticEntities[i]->m = copia->m;
		world_info.staticEntities[i]->mesh = copia->mesh;
		world_info.staticEntities[i]->texture = copia->texture;
		world_info.staticEntities[i]->pos = copia->getPos();
		}
		if (staticEntities[i] == NULL) break;
		
	}
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (dynamicEntities[i] != NULL) {

			//copia de la existent
			Entity* copia = new Entity(0, 0, 0, Matrix44());
			copia->copy(this->dynamicEntities[i]);

			//nova entity per la info
			Entity* info = new Entity(0, 0, 0, Matrix44());

			//asignem la copia
			world_info.dynamicEntities[i] = info;
			world_info.dynamicEntities[i]->m = copia->m;
			world_info.dynamicEntities[i]->mesh = copia->mesh;
			world_info.dynamicEntities[i]->texture = copia->texture;
			world_info.dynamicEntities[i]->pos = copia->getPos();
		}
		if (dynamicEntities[i] == NULL) break;

	}
	for (int j = 0; j < MAX_ZOMBIES; j++)
	{
		if (zombies[j] != NULL) {

			//copia de la existent
			Zombie* copia = (Zombie*) new Entity(0, 0, 0, Matrix44());
			copia->copy(this->zombies[j]);

			//nova entity per la info
			Zombie* info = (Zombie*) new Entity(0, 0, 0, Matrix44());

			//asignem la copia
			world_info.zombies[j] = info;
			world_info.zombies[j]->m = copia->m;
			world_info.zombies[j]->mesh = copia->mesh;
			world_info.zombies[j]->texture = copia->texture;
			world_info.zombies[j]->pos = copia->getPos();
		}
		if (zombies[j] == NULL) break;

	}
	
	//world_info.map = this->map;
	//save to file
	FILE* fp = fopen("savegame.bin", "wb");
	fwrite(&world_info, sizeof(World), 1, fp);
	fclose(fp);
}

bool World::loadWorldInfo()
{
	World world_info =  World(shader);

	//load file
	FILE* fp = fopen("savegame.bin", "rb");
	if (fp == NULL) //no savegame found
		return false;

	fread(&world_info, sizeof(World), 1, fp);
	fclose(fp);

	//transfer data from game_info to Game
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (world_info.staticEntities[i] != NULL) {

			//copia de la existent
			Entity* copia = new Entity(0, 0, 0, Matrix44());
			copia->copy(world_info.staticEntities[i]);

			//nova entity per la info
			Entity* info = new Entity(0, 0, 0, Matrix44());

			//asignem la copia
			this->staticEntities[i] = info;
			this->staticEntities[i]->m = copia->m;
			this->staticEntities[i]->mesh = copia->mesh;
			this->staticEntities[i]->texture = copia->texture;
			this->staticEntities[i]->pos = copia->getPos();
		}
		if (world_info.staticEntities[i] == NULL) break;

	}
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (world_info.dynamicEntities[i] != NULL) {

			//copia de la existent
			Entity* copia = new Entity(0, 0, 0, Matrix44());
			copia->copy(world_info.dynamicEntities[i]);

			//nova entity per la info
			Entity* info = new Entity(0, 0, 0, Matrix44());

			//asignem la copia
			this->dynamicEntities[i] = info;
			this->dynamicEntities[i]->m = copia->m;
			this->dynamicEntities[i]->mesh = copia->mesh;
			this->dynamicEntities[i]->texture = copia->texture;
			this->dynamicEntities[i]->pos = copia->getPos();
		}
		if (world_info.dynamicEntities[i] == NULL) break;

	}
	for (int j = 0; j < MAX_ZOMBIES; j++)
	{
		if (world_info.zombies[j] != NULL) {

			//copia de la existent
			Zombie* copia = (Zombie*) new Entity(0, 0, 0, Matrix44());
			copia->copy(world_info.zombies[j]);

			//nova entity per la info
			Zombie* info = (Zombie*) new Entity(0, 0, 0, Matrix44());

			//asignem la copia
			this->zombies[j] = info;
			this->zombies[j]->m = copia->m;
			this->zombies[j]->mesh = copia->mesh;
			this->zombies[j]->texture = copia->texture;
			this->zombies[j]->pos = copia->getPos();
		}
		if (world_info.zombies[j] == NULL) break;

	}
	//this->map = world_info.map;

	return true;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
using std::cerr;
using std::endl;
using std::cout;
using std::ofstream;
using std::ifstream;
void World::saveWorldInfo() {
	ofstream outdata;
	outdata.open("mundo.dat");
	if (!outdata) {
		cerr << "Error: file could not be opened" << endl;
		exit(1);
	}
	cerr << "Saving world" << endl;
	for (size_t i = 0; i < MAX_ENTITIES; i++)
	{
		if (staticEntities[i] != NULL) {
			/*outdata << staticEntities[i]->pos.x << endl;
			outdata << staticEntities[i]->pos.y << endl;
			outdata << staticEntities[i]->pos.y << endl;*/
		}
		else {
			break;
		}
	}
	for (size_t i = 0; i < MAX_ENTITIES; i++)
	{
		if (dynamicEntities[i] != NULL) {
			Matrix44 mat = dynamicEntities[i]->m;
			for (int j = 0; j < 16; j++) {
				outdata << mat.m[j] << " ";
			}
			outdata << endl;
			outdata << dynamicEntities[i]->mesh->name << endl;
			outdata << dynamicEntities[i]->texture->filename << endl;
		}
		else {
			break;
		}
	}
	/*for (size_t i = 0; i < MAX_ZOMBIES; i++)
	{
		if (zombies[i] != NULL) {
			outdata << zombies[i] << endl;
		}
		else {
			break;
		}
	}*/
	cerr << "World saved!" << endl;
	outdata.close();
}
bool World::loadWorldInfo() {
	ifstream indata;
	std::string strMatrix;
	std::string meshName;
	std::string textureName;

	indata.open("mundo.dat");
	if (!indata) {
		cerr << "Error tu prima" << endl;
		return false;
	}
	cerr << "Loading world" << endl;
	
	int j = 0;
	while (indata.good()) {
		Matrix44 m = Matrix44();
		for (int i = 0; i < 16; i++) {
			indata >> strMatrix;
			m.m[i] = ::atof(strMatrix.c_str());
		}
		Vector3 pos = m.getTranslation();
		Entity* newEntity = new Entity(pos, m);
		indata >> meshName;
		newEntity->loadMesh(meshName.c_str());
		indata >> textureName;
		newEntity->loadTexture(textureName.c_str());

		staticEntities[j] = newEntity;
		j++;
	}
	cerr << "World loaded!" << endl;
	return true;
}