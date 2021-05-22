#include "entity.h"
#include "camera.h"

void Zombie::AStarPath()
{

}

void Zombie::setVel(float v){
	this->vel = v;
}

void Entity::render() {
	Camera* camera = Camera::current;
	if (shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_time", time);
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", m);

		mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}
}