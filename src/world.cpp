#include "world.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>
using std::cerr;
using std::endl;
using std::cout;
using std::ofstream;
using std::ifstream;

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
	for (int i = 0; i < MAX_WEAPONS; i++) {
		weapons[i] = NULL;
	}
	this->shader = shader;
	this->player = (Player*) new Entity(0, 0, 0, Matrix44());
	this->ground = NULL;
	this->sky = NULL;
	for (int i = 0; i < CUADRANTS; i++) this->maps[i] = new uint8[100 * 100]; //Mapwidth * Mapheight
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

void World::addWeapon(Weapon* weapon) {
	for (int i = 0; i < MAX_WEAPONS; i++) {
		if (weapons[i] == NULL) {
			weapons[i] = weapon;
			break;
		}
	}
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
	copia->m.rotate(entity->yaw * DEG2RAD, Vector3(0,1,0));
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
void World::initMap() {
	int Mapwidth = 100;
	int Mapheight = 100;

	//for (int i = 0; i < CUADRANTS; i++) {
	//	uint8* currentMap = maps[i];
	//	for (int j = 0; j < Mapwidth * Mapheight; j++) {//Recorrer mapa
	//		for (int k = 0; k < MAX_ENTITIES; k++) {	//Recorrer entidades
	//			if (staticEntities[k] != NULL) {
	//				Entity* current = staticEntities[k];
	//				Vector3 coll;
	//				Vector3 collNormal;
	//				Vector3 pos;
	//				if (i == 0) Vector3 pos = Vector3(j % Mapwidth, 0.1, j / Mapwidth);		//AllPositive
	//				else if (i == 1) Vector3 pos = Vector3(j % Mapwidth, 0.1, -(j / Mapwidth));	//xPositive
	//				else if (i == 2) Vector3 pos = Vector3(-(j % Mapwidth), 0.1, j / Mapwidth);	//zPositive
	//				else if (i == 3) Vector3 pos = Vector3(-(j % Mapwidth), 0.1, -(j / Mapwidth));//AllNegative

	//				if (current->mesh != NULL) {
	//					if (current->pos.x <= pos.x && current->pos.x >= pos.x - 1 && current->pos.z <= pos.z && current->pos.z >= pos.z - 1) 
	//						currentMap[j] = 0;
	//					//if (current->mesh->testSphereCollision(current->m, pos, 0.5, coll, collNormal)) currentMap[j] = 0;
	//					else currentMap[j] = 1;
	//				}
	//			}
	//			else break;
	//		}
	//	}
	//	maps[i] = currentMap;
	//}
	for (int j = 0; j < Mapwidth * Mapheight; j++) {
		maps[0][j] = 1;
		maps[1][j] = 1;
		maps[2][j] = 1;
		maps[3][j] = 1;
	}
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* current = staticEntities[i];
		if (current == NULL) break;
		int index = (floor(abs(current->pos.z)) * Mapwidth) + floor(abs(current->pos.x));
		if (current->pos.x >= 0 && current->pos.z >= 0) 
			maps[0][index] = 0;
		else if (current->pos.x >= 0 && current->pos.z <= 0) 
			maps[1][index] = 0;
		else if (current->pos.x <= 0 && current->pos.z >= 0) 
			maps[2][index] = 0;
		else if (current->pos.x <= 0 && current->pos.z <= 0) 
			maps[3][index] = 0;
	}
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void World::loadDecoration() {
	//Load filenames from save
	std::string path = "data/Assets/Decoration/";
	std::vector<std::string> deco;
	std::string mbin = ".mbin";
	int numElements = 0;
	for (const auto& object : std::filesystem::directory_iterator(path)) {
		std::string name = object.path().u8string();

		std::string ext = name.substr((name.size() - 5), 5);
		//std::cerr << ext << std::endl;;
		if (ext == mbin) continue;
		deco.push_back(name);
		numElements++;
		//std::cerr << name << std::endl;;
	}
	Matrix44 m;
	for (int i = 0; i < numElements - 1; i += 2) {
		Entity* entity = new Entity(0, 0, 0, m);
		const char* meshObject = deco[i].c_str();
		entity->loadMesh(meshObject);

		const char* textureObject = deco[i + 1].c_str();
		entity->loadTexture(textureObject);

		std::string tipo = deco[i].substr(23, deco[i].size() - 27);
		entity->type = tipo;
		//std::cerr << tipo << std::endl;;
		addDecoration(entity);
	}

	maxDecoration = (deco.size() / 2) - 1;

}
void World::loadStructure() {
	//Load filenames from save
	std::string path = "data/Assets/Structure/";
	std::string mbin = ".mbin";
	std::vector<std::string> stru;
	int numElements = 0;
	for (const auto& object : std::filesystem::directory_iterator(path)) {
		std::string name = object.path().u8string();
		std::string ext = name.substr((name.size() - 5), 5);
		//std::cerr << ext << std::endl;;
		if (ext == mbin) continue;
		stru.push_back(name);
		numElements++;
		//std::cerr << name << std::endl;;
	}
	Matrix44 m;
	for (int i = 0; i < numElements - 1; i += 2) {
		Entity* entity = new Entity(0, 0, 0, m);
		const char* meshObject = stru[i].c_str();
		entity->loadMesh(meshObject);

		const char* textureObject = stru[i + 1].c_str();
		entity->loadTexture(textureObject);

		std::string tipo = stru[i].substr(26, stru[i].size() - 30);
		entity->type = tipo;
		//std::cerr << tipo << std::endl;;
		addStructure(entity);
	}
	maxStructure = (stru.size() / 2) - 1;
}


//
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
void World::deleteEntity(Entity* entity) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (staticEntities[i] == entity) staticEntities[i] = NULL; break;
		if (dynamicEntities[i] == entity) dynamicEntities[i] = NULL; break;
	}
}
void World::moveZombies() {
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		Zombie* zombie = zombies[i];
		if (zombie == NULL) continue;
		Vector3 target;
		target = zombie->AStarPath(player->pos, maps);
		zombie->move(target);
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
		if (entity->mesh != NULL) {

		BoundingBox currentBox = transformBoundingBox(entity->m, entity->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		entity->render(shader);
		}

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

void World::RenderZombies(Camera* camera, float time) {
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		Zombie* zombie = zombies[i];
		if (zombie == NULL) {
			break;
		}
		BoundingBox currentBox = transformBoundingBox(zombie->m, zombie->mesh->box);
		//if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		zombie->renderAnimation(time);
	}
}

void World::RenderPlayer(Camera* camera) {
	player->render(shader);
	weapons[0]->renderWeapon(player, shader); //CAMBIAR EL [0] AL ARMA QUE TENGA SELECCIONADA
}

void World::RenderBoundingStatic(Camera* camera)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* entity = staticEntities[i];
		if (entity == NULL || entity->mesh == NULL) break;
		BoundingBox currentBox = transformBoundingBox(entity->m, entity->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		if (entity->bounding)entity->mesh->renderBounding(entity->m);
	}
}

void World::RenderBoundingDynamic(Camera* camera)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* entity = dynamicEntities[i];
		if (entity == NULL || entity->mesh == NULL) break;
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

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void World::saveWorldInfo(std::string filename) {
	ofstream outdata;
	outdata.open("save/" + filename + ".dat");
	if (!outdata) {
		cerr << "Error: file could not be opened" << endl;
		exit(1);
	}
	cerr << "Saving world" << endl;
	for (size_t i = 0; i < MAX_ENTITIES; i++)
	{
		if (staticEntities[i] != NULL) {
			Matrix44 mat = staticEntities[i]->m;
			for (int j = 0; j < 16; j++) {
				outdata << mat.m[j] << " ";
			}
			outdata << endl;
			outdata << staticEntities[i]->mesh->name << endl;
			outdata << staticEntities[i]->texture->filename << endl;
			outdata << "static" << endl;
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
			outdata << "dynamic" << endl;
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
bool World::loadWorldInfo(std::string filename) {
	ifstream indata;
	std::string strMatrix;
	std::string meshName;
	std::string textureName;
	std::string entityType;

	indata.open("save/" + filename + ".dat");
	if (!indata) {
		cerr << "Error tu prima" << endl;
		return false;
	}
	cerr << "Loading world" << endl;
	
	int staticIndex = 0;
	int dynamicIndex = 0;
	while (indata.good()) {
		Matrix44 m = Matrix44();
		for (int i = 0; i < 16; i++) {
			indata >> strMatrix;
			m.m[i] = ::atof(strMatrix.c_str());
		}
		Vector3 pos = m.getTranslation();
		indata >> meshName;
		indata >> textureName;
		indata >> entityType;
		if (pos.y == 10000000.0) {
			continue;
		}
		Entity* newEntity = new Entity(pos, m);
		newEntity->loadMesh(meshName.c_str());
		newEntity->loadTexture(textureName.c_str());
		if (entityType == "static") {
			staticEntities[staticIndex] = newEntity;
			staticIndex++;
		}
		else if (entityType == "dynamic") {
			dynamicEntities[dynamicIndex] = newEntity;
			dynamicIndex++;
		}
	}
	cerr << "World loaded!" << endl;
	return true;
}

void World::initSpawn() {
	Matrix44 m;
	spawn1 = (Spawn*) new Entity(0,0,0,m);
	spawn2 = (Spawn*) new Entity(0, 0, 0, m); //definirlos en las posiciones que queramos y añadirles de alguna manera los zombies que tendran
	spawn3 = (Spawn*) new Entity(0, 0, 0, m);
	
}


void World::StartRound(float time){  //ni caso a los parametros pilla la idea namas, si te gusta
	int r = this->round;
	int numZombies = 3 * (r + 5); //divisible entre 3 para poder distribuirlos por igual por los spawns
	for (int s = 0; s < numSpawns; s++) {
		for (int i = 0; i < numZombies; i++) {

			Zombie* zombie = (Zombie*) new Entity(0, 0, 0, Matrix44());//pos del spawn, m
			spawn1->spawnAZombie(zombie, round);
		}

	}
}