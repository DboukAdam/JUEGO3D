#include "entity.h"

Entity::Entity()
{
	this->mesh = NULL;
	this->texture = NULL;
}

Entity::Entity(Vector3 pos, Matrix44 m, Mesh* mesh = NULL, Texture* texture = NULL) {
	this->pos = pos;
	this->m = m;
	this->mesh = mesh;
	this->texture = texture;
}

Entity::Entity(int x, int y, int z, Matrix44 m, Mesh* mesh = NULL, Texture* texture = NULL) {
	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;
	this->m = m;
	this->mesh = mesh;
	this->texture = texture;
}

void Entity::setPos(Vector3 pos) {
	this->pos = pos;
}

void Entity::setPos(int x, int y, int z) {
	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;
}

void Entity::loadMesh(const char* filename) {
	mesh = Mesh::Get(filename);
}

void Entity::loadTexture(const char* filename) {
	texture = Texture::Get(filename);
}

/*Zombie::Zombie()
{
}*/

