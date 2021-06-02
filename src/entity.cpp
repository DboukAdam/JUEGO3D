#include "entity.h"


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

void Player::CamPlayer(Camera* camera)
{
	camera->enable();

	m.setTranslation(pos.x, pos.y, pos.z);
	m.rotate(yaw * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	
		Matrix44 pitchM;
		pitchM.rotate(pitch * DEG2RAD, Vector3(1.0f, 0.0f, 0.0f));
		Vector3 forward = pitchM.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
		forward = m.rotateVector(forward);

		Vector3 eye = m * Vector3(0.0f, 2.0f, -0.5f);
		Vector3 center = eye + forward;

		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		camera->lookAt(eye, center, up);
	
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