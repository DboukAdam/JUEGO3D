#include "entity.h"

void Zombie::DeleteZombie()
{
	Entity::DeleteEntity();
}

void Zombie::AStarPath()
{

}

void Zombie::setVel(float v){
	this->vel = v;
}

void Zombie::renderAnimation(Shader* shader, float time, float tiling) {
	Camera* camera = Camera::current;
	Animation* walk = Animation::Get("data/Zombies/Animation/animations_walking.skanim");
	walk->assignTime(time);
	if (shader) {
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", Texture::Get("data/Zombies/WorldWar_zombie.png"));
		shader->setUniform("u_time", time);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_texture_tiling", tiling);
		mesh->renderAnimated(GL_TRIANGLES, &walk->skeleton);
	}
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

	Vector3 eye = m * Vector3(0.0f, 1.0f, -0.5f);
	Vector3 center = eye + forward;

	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	camera->lookAt(eye, center, up);
	
}

void Entity::DeleteEntity()
{
	this->m.setTranslation(0, 10000000, 0);
}

void Entity::render(Shader* shader, float tiling) {
	Camera* camera = Camera::current;
	if (shader)
	{
		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_time", time);
		if (texture != NULL) shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", m);
		shader->setUniform("u_texture_tiling", tiling);
		mesh->render(GL_TRIANGLES);
	}
}

void Entity::copy(Entity* entity) {
	this->mesh = entity->mesh;
	this->texture = entity->texture;
	this->m = entity->m;
}

void Weapon::renderWeapon(Player* player, Shader* shader, float tiling) { //AAAAAAH
	Camera* camera = Camera::current;
	this->pos = player->pos;
	this->pitch = player->pitch;
	this->yaw = player->yaw - 90;
	m.setTranslation(camera->eye.x, camera->eye.y - 0.5, camera->eye.z);
	m.rotate(yaw * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	render(shader, tiling);
}
