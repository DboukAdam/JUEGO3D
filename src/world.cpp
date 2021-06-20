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
	this->shader = shader;
	this->player = (Player*) new Entity(0, 0, 0, Matrix44());
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
void World::loadDecoration() {
	//Load filenames from save
	std::string path = "data/classic/1.Decoration/";
	std::vector<std::string> decoration;
	int numElements = 0;
	for (const auto& object : std::filesystem::directory_iterator(path)) {
		std::string name = object.path().u8string();
		decoration.push_back(name);
		numElements++;
		std::cerr << name << std::endl;;
	}

	Matrix44 m;
	for (int i = 0; i < numElements - 1; i += 2) {
		Entity* entity = new Entity(0, 0, 0, m);
		const char* meshObject = decoration[i].c_str();
		entity->loadMesh(meshObject);

		const char* textureObject = decoration[i + 1].c_str();
		entity->loadTexture(textureObject);

		std::string tipo = decoration[i].substr(26, decoration[i].size() - 30);
		//entity->type = tipo;
		std::cerr << tipo << std::endl;;
		addDecoration(entity);
	}

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
		Entity* newEntity = new Entity(pos, m);
		indata >> meshName;
		newEntity->loadMesh(meshName.c_str());
		indata >> textureName;
		newEntity->loadTexture(textureName.c_str());
		
		indata >> entityType;
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