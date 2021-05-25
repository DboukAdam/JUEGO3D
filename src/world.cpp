#include "world.h"

World::World(Shader* shader) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		entities[i] = NULL;
	}
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		zombies[i] = NULL;
	}
	this->shader = shader;
	this->player = NULL;
}

void World::addEntity(Entity* entity) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (entities[i] == NULL){
			entities[i] = entity;
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

void World::createZombies() {
	Matrix44 m;
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		float randX = random(20, -10);
		float randZ = random(20, -10);
		m.setTranslation(randX, 0, randZ);
		Zombie* zombie = (Zombie*) new Entity(randX, 0, randZ, m);
		zombie->loadMesh("data/ZombieScale.obj");
		zombie->loadTexture("data/Zombie/Zed_1.png");
		zombie->setVel(2.0f);
		addZombie(zombie);
	}
}

void World::disparar() {
	Camera* camera = Camera::current;
	Vector3 colPoint;
	Vector3 colNormal;
	float maxDistance = 10;
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		Zombie* zombie = zombies[i];
		if (zombie == NULL)	break;
		if (zombie->mesh->testRayCollision(zombie->m, camera->eye, camera->center, colPoint, colNormal, maxDistance, false)) {
			//zombie->~Zombie();
			std::cerr << "Buena punteria!" << std::endl;;
		}
	}
}

void World::addObjectEditor(Mesh* mesh, Texture* texture, Vector3 dir) {
	Camera* camera = Camera::current;
	Vector3 origin = camera->eye;
	Vector3 pos = RayPlaneCollision(Vector3(0, -4.5, 0), Vector3(0, 1, 0), origin, dir);
	Matrix44 entityModel;
	Entity* entity = new Entity(0, 0, 0, entityModel);
	entity->m.setTranslation(pos.x, pos.y, pos.z);
	entity->mesh = mesh;
	entity->texture = texture;
	addEntity(entity);
}

void World::selectEntityEditor(Vector3 dir){
	Camera* camera = Camera::current;
	Vector3 origin = camera->eye;
	for (int i = 1; i < MAX_ENTITIES; i++)
	{
		Entity* current = entities[i];
		Vector3 col;
		Vector3 normal;
		if (current == NULL) break;
		if (!current->mesh->testRayCollision(current->m, origin, dir, col, normal, 10000)) continue;
		if (selectedEntity != NULL) selectedEntity->bounding = false;
		current->bounding = true;
		selectedEntity = current;
		break;
	}
}