#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"

class Entity {
public:
	Vector3 pos; //Mirar si es util por culpa del setTranslation ese que dice el feo del Adam
	Matrix44 m;
	Mesh* mesh;
	Texture* texture;
	float angle;
	
	Entity(Vector3 pos, Matrix44 m, float angle = 0, Mesh* mesh = NULL, Texture* texture = NULL) {
		this->pos = pos;
		this->m = m;
		this->mesh = mesh;
		this->texture = texture;
		this->angle = angle;
	}
	Entity(float x, float y, float z, Matrix44 m, float angle = 0, Mesh* mesh = NULL, Texture* texture = NULL) {
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
		this->m = m;
		this->mesh = mesh;
		this->texture = texture;
		this->angle = angle;
	}
	~Entity();

	void setPos(Vector3 pos) {
		this->pos = pos;
	}
	void setPos(int x, int y, int z) {
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
	}

	Vector3 getPos() {
		m.getTranslation();
	}

	void loadMesh(const char* filename) {
		mesh = Mesh::Get(filename);
	}
	void loadTexture(const char* filename) {
		texture = Texture::Get(filename);
	}

	void render(Shader* shader);
};

class Zombie : public Entity {
public:
	Vector3 dir;
	float vel;
	bool triggered;

	~Zombie();
	void AStarPath(); //Cambiar lo que devuelve
	void setVel(float v);
};

class Player : public Entity {
public:
	Vector3 dir;
	float vel;
	int vida;
	void setVel(float v);
};