#include "entity.h"
#include "pathfinders.h"

int * Zombie::AStarPath(Vector3 target, uint8* map) {
	int output[100];
	int path_steps = -1;
	float tileSizeX = 10.f; //MEGA SUCIO
	float tileSizeY = 10.f;
	int width = 10;
	int height = 10; //SUPER SUCIO

	int startx = pos.x / tileSizeX;
	int starty = pos.y / tileSizeY;

	float targetx = clamp(pos.x / tileSizeX, 0, width);
	float targetz = clamp(pos.z / tileSizeY, 0, height);

	path_steps = AStarFindPathNoTieDiag(
		startx, starty, //origin (tienen que ser enteros)
		targetx, targetz, //target (tienen que ser enteros)
		map, //pointer to map data
		width, height, //map width and height
		output, //pointer where the final path will be stored
		100); //max supported steps of the final path

	std::cerr << "Number of steps: " << path_steps << std::endl;

	return output;
}

void Zombie::move(Vector3 target) {

}

void Zombie::setVel(float v){
	this->vel = v;
}

void Zombie::renderAnimation(Shader* shader, float time, float tiling) {
	Shader* shaderAnim = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	shaderAnim->enable();
	Camera* camera = Camera::current;
	Animation* walk = Animation::Get("data/Zombies/Animation/animations_walking.skanim");
	walk->assignTime(time);
	if (shaderAnim) {
		shaderAnim->setUniform("u_color", Vector4(1, 1, 1, 1));
		shaderAnim->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shaderAnim->setUniform("u_texture", texture);
		shaderAnim->setUniform("u_time", time);
		shaderAnim->setUniform("u_model", Matrix44());
		shaderAnim->setUniform("u_texture_tiling", tiling);
		mesh->renderAnimated(GL_TRIANGLES, &walk->skeleton);
		//walk->skeleton.renderSkeleton(camera, Matrix44());
	}
	shaderAnim->disable();
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
	shader->enable();
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
	shader->disable();
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
	//Vector3 dir = camera->getRayDirection(400, 300, 800, 600);
	//Vector3 noSe = dir * Vector3(0.1, 0, 0);
	m.setTranslation(camera->eye.x + 0.1, camera->eye.y - 0.3, camera->eye.z);
	m.rotate(yaw * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	m.rotate(pitch * DEG2RAD, Vector3(0.0f, 0.0f, 1.0f));
	render(shader, tiling);
}

void Spawn::spawnAZombie(Zombie* zombie, int round)
{
	zombie->setPos(this->pos);
	float prob = random();
	if (round < 5) {
		
		if (prob < 0.5) zombie->loadTexture("COLOR VERDE");
		zombie->loadTexture("COLOR AMARILLO");									//AÑADIR LA VIDA DISTINTA Y DEMAS
	}
	else if (round < 10) {
		if(prob < 0.3) zombie->loadTexture("COLOR VERDE");
		if(prob > 0.3 && prob < 0.7) zombie->loadTexture("COLOR AMARILLO");
		zombie->loadTexture("COLOR ROJO");

	}
	else {
		if(prob < 0.4) zombie->loadTexture("COLOR AMARILLO");
		zombie->loadTexture("COLOR ROJO");
	}
}
