#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "animation.h"
#include <string>


#pragma once
class Entity {
public:
	
	Vector3 pos; //Mirar si es util por culpa del setTranslation ese que dice el feo del Adam
	Matrix44 m;
	Mesh* mesh;
	Texture* texture;
	float yaw;
	float pitch;
	bool bounding = false;
	std::string type;
	
	Entity(Vector3 pos, Matrix44 m, float yaw = 0, Mesh* mesh = NULL, Texture* texture = NULL, float pitch = 0) {
		this->pos = pos;
		this->m = m;
		this->mesh = mesh;
		this->texture = texture;
		this->yaw = yaw;
		this->pitch = pitch;
	};
	Entity(float x, float y, float z, Matrix44 m, float yaw = 0, Mesh* mesh = NULL, Texture* texture = NULL, float pitch = 0) {
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
		this->m = m;
		this->mesh = mesh;
		this->texture = texture;
		this->yaw = yaw;
		this->pitch = pitch;
	};
	

	void setPos(Vector3 pos) {
		this->pos = pos;
	};
	void setPos(int x, int y, int z) {
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
	};

	Vector3 getPos() {
		return m.getTranslation();
	};

	void loadMesh(const char* filename) {
		mesh = Mesh::Get(filename);
	};
	void loadTexture(const char* filename) {
		texture = Texture::Get(filename);
	};

	void DeleteEntity();

	void render(Shader* shader, float tiling = 1.0f);
	void copy(Entity* entity);
};

class Zombie : public Entity {
public:
	Vector3 dir;
	float vel;
	int vida;

	Vector3 AStarPath(Vector3 target, uint8* map);
	void move(Vector3 target);
	void setVel(float v);
	void renderAnimation(float time, float tiling = 1.0f);
};

class ZombieSpawner : public Entity {
	float ultimoSpawn;
	void spawnZombie(Zombie* zombie, int vida);
};
class Player : public Entity {
public:
	Vector3 dir;
	float vel;
	int vida;
	void setVel(float v);
	void CamPlayer(Camera* camera);
};

class Weapon : public Entity {
public:
	float cadencia;
	bool isEmpty;
	int tamañoCargador;
	bool recargar;
	float retroceso;
	//bool apuntar;
	void renderWeapon(Player* player, Shader* shader, float tiling = 1.0f);

};

class Spawn : public Entity {
public:
	void spawnAZombie(Zombie* zombie, int round);

};