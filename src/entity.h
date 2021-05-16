#include "utils.h"
#include "mesh.h"
#include "texture.h"

class Entity {
public:
	Vector3 pos;
	Matrix44 m;
	Mesh* mesh;
	Texture* texture;
	
	Entity(Vector3 pos, Matrix44 m, Mesh* mesh = NULL, Texture* texture = NULL) {
		this->pos = pos;
		this->m = m;
		this->mesh = mesh;
		this->texture = texture;
	}
	Entity(int x, int y, int z, Matrix44 m, Mesh* mesh = NULL, Texture* texture = NULL) {
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
		this->m = m;
		this->mesh = mesh;
		this->texture = texture;
	}

	void setPos(Vector3 pos) {
		this->pos = pos;
	}
	void setPos(int x, int y, int z) {
		this->pos.x = x;
		this->pos.y = y;
		this->pos.z = z;
	}
	void loadMesh(const char* filename) {
		mesh = Mesh::Get(filename);
	}
	void loadTexture(const char* filename) {
		texture = Texture::Get(filename);
	}
};