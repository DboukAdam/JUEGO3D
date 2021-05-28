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
	float yaw;
	float pitch;
	bool bounding = false;
	
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
	~Entity();

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

	void render(Shader* shader, float tiling = 1.0f);
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