#include "world.h"

World::World()
{
	
}

Game* game = Game::instance;

void World::initWorld()
{
	current = intro;
	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	texture = new Texture();
	//texture->load("data/texture.tga");
	texture->load("data/modelos/ambulance.png");
	// example of loading Mesh from Mesh Manager
	//mesh = Mesh::Get("data/box.ASE");
	mesh = Mesh::Get("data/modelos/ambulance.obj");
	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	Matrix44 m;
	Matrix44 modelZ; modelZ.setTranslation(0, 0, 0);
	m.rotate(game->angle * DEG2RAD, Vector3(0, 1, 0));
	suelo = new Entity(0, 0, 0, m);
	suelo->loadMesh("data/Shop/Shop-4-GroundTile.obj");
	suelo->loadTexture("data/Shop/Shop-4-GroundTile.png");

	//zombie = new Entity(0, 0, 0, modelZ);
	//zombie->loadMesh("data/Zombie/Zed_1.obj");
	//zombie->loadTexture("data/Zombie/Zed_1.png");

	shop = new Entity(0, 5, 0, m);
	m.translate(0.0f, 3.5f, 0.0f);
	shop->loadMesh("data/Shop/Shop-1-ShopBuilding_2.obj");
	shop->loadTexture("data/Shop/Shop-1-ShopBuilding_2.png");

	zzz = new Zombie();
	zzz->m = modelZ;
	zzz->pos.set(0.0f, 3.5f, 5.0f);
	zzz->loadMesh("data/Zombie/Zed_1.obj");
	zzz->loadTexture("data/Zombie/Zed_1.png");
}
