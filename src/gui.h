#pragma once
#include "texture.h"

<<<<<<< Updated upstream
	

};
/*
//CAMBIAR:
Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/gui.fs");
World* menuInicio = new World(shader);

Entity* boton = new Entity(0, 0, 0, Matrix44());
boton->loadTexture("data/Button_Flesh.png");
menuInicio->addEntity(boton);


Game* game = Game::instance;

Camera cam2D;
cam2D.setOrthographic(0, game->window_width, game->window_height, 0, -1, 1);
cam2D.enable();

Mesh quad;
quad.createQuad(100, 100, 512, 128, false);

Shader* shader = world->shader;
Texture* texture = world->entities[0]->texture;
shader->enable();
if (shader)
{
	//upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
	shader->setUniform("u_tex_range", Vector4(0, 0, 1, 1)); //rango de 0 a 1
	shader->setUniform("u_texture", texture, 0);
	shader->setUniform("u_model", Matrix44());
	shader->setUniform("u_texture_tiling", 1.0f);
	quad.render(GL_LINE_STRIP);
}
//hacer draw call
quad.render(GL_TRIANGLES);

shader->disable();
*/
=======
class Gui{
public:
	Shader* shader;
	Texture* atlas;

	Gui(Shader* shader, Texture* atlas);
	void Render();
};
>>>>>>> Stashed changes
