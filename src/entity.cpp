#include "entity.h"
#include "camera.h"

void Zombie::AStarPath()
{

}

void Zombie::setVel(float v){
	this->vel = v;
}

Zombie::~Zombie()
{
	mesh->clear();
	texture->clear();
}

void Player::setVel(float v) {
	this->vel = v;
}

Entity::~Entity()
{
	mesh->clear();
	texture->clear();
	this->bounding = false;
}

void Entity::render(Shader* shader, float tiling) {
	Camera* camera = Camera::current;
	if (shader)
	{
		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_time", time);
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", m);
		shader->setUniform("u_texture_tiling", tiling);
		mesh->render(GL_TRIANGLES);
	}
}