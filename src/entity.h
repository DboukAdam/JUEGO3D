#include "utils.h"
#include "mesh.h"
#include "texture.h"

class Entity {
public:
	Vector3 pos;
	Matrix44 m;
	Mesh* mesh;
	Texture* texture;
	Entity();
	Entity(Vector3 pos, Matrix44 m, Mesh* mesh, Texture* texture);
	Entity(int x, int y, int z, Matrix44 m, Mesh* mesh, Texture* texture);

	void setPos(Vector3 pos);
	void setPos(int x, int y, int z);
	void loadMesh(const char* filename);
	void loadTexture(const char* filename);
};

/*class Zombie : public Entity {
public:
	Zombie();
};*/