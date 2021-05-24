#include "world.h"

World::World(Shader* shader) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		entities[i] = NULL;
	}
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		zombies[i] = NULL;
	}
	this->shader = shader;
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