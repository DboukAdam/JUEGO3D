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

void World::addObjectEditor(Mesh* mesh, Texture* texture, Vector3 dir) {
	Camera* camera = Camera::current;
	Vector3 origin = camera->eye;
	Vector3 pos = RayPlaneCollision(Vector3(0, 0, 0), Vector3(0, 1, 0), origin, dir);
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

void World::RenderEntities(Camera* camera)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* entity = entities[i];
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

void World::RenderBoundingEntities(Camera* camera)
{
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* entity = entities[i];
		if (entity == NULL) break;
		BoundingBox currentBox = transformBoundingBox(entity->m, entity->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		entity->mesh->renderBounding(entity->m);
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


