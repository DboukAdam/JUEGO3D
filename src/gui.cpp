#include "gui.h"
#include "camera.h"
#include "game.h"

Gui::Gui(Shader* shader, Texture* atlas) {
	this->shader = shader;
	this->atlas = atlas;
}

void Gui::initAtlas() {
	atlasRanges[0] = Vector4(0, 0, 640 / atlas->width, 160 / atlas->height); //buttonGreen
	atlasRanges[1] = Vector4(0, 160 / atlas->height, 640 / atlas->width, 208 / atlas->height); //buttonGreenPressed
	atlasRanges[2] = Vector4(1 - 80 / atlas->width, 0, 80 / atlas->width, 80 / atlas->height); //crosshair
}

void Gui::RenderIntroGui()
{
	Game* game = Game::instance;
	Camera cam2D;
	cam2D.setOrthographic(0, game->window_width, game->window_height, 0, -1, 1);
	
	float button_width = 320;
	float button_height = 80;
	float buttonOffset = 10;
	float xPos = button_width/2 + buttonOffset;
	float yPos = game->window_height / 5;

	Mesh quad; //MODIFICAR
	quad.createQuad(xPos, yPos, button_width, button_height, false);

	shader->enable();
	if (shader)
	{
		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
		shader->setUniform("u_texture", atlas, 0);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_texture_tiling", 1.0f);
		//First button
		shader->setUniform("u_tex_range", atlasRanges[greenButton]);
		quad.render(GL_LINE_STRIP);
		quad.render(GL_TRIANGLES);
		//Second button
		quad.createQuad(xPos, yPos * 2, button_width, button_height, false);
		shader->setUniform("u_tex_range", atlasRanges[greenButton]);
		quad.render(GL_LINE_STRIP);
		quad.render(GL_TRIANGLES);
		//Third button
		quad.createQuad(xPos, yPos * 3, button_width, button_height, false);
		shader->setUniform("u_tex_range", atlasRanges[greenButton]);
		quad.render(GL_LINE_STRIP);
		quad.render(GL_TRIANGLES);
		//Fourth button
		quad.createQuad(xPos, yPos * 4, button_width, button_height, false);
		shader->setUniform("u_tex_range", atlasRanges[greenButton]);
		quad.render(GL_LINE_STRIP);
		quad.render(GL_TRIANGLES);
	}
	shader->disable();
	
	float scale = 5.0f;
	float textSize = 7 * scale;
	float yOffset = 14.0f;
	float xOffset = buttonOffset + 20;
	drawText(xOffset, yPos - yOffset - textSize/2, "play", Vector3(1, 1, 1), scale);
	drawText(xOffset, yPos * 2 - yOffset - textSize / 2, "editor", Vector3(1, 1, 1), scale);
	drawText(xOffset, yPos * 3 - yOffset - textSize / 2, "settings", Vector3(1, 1, 1), scale);
	drawText(xOffset, yPos * 4 - yOffset - textSize / 2, "exit", Vector3(1, 1, 1), scale);
}

void Gui::RenderCrosshair(){
	Game* game = Game::instance;
	Camera cam2D;
	cam2D.setOrthographic(0, game->window_width, game->window_height, 0, -1, 1);
	//cam2D.enable();

	Mesh quad; //MODIFICAR
	quad.createQuad(game->window_width/2, game->window_height/2, 10, 10, false);

	shader->enable();
	if (shader)
	{
		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
		shader->setUniform("u_tex_range", atlasRanges[crosshair]);
		shader->setUniform("u_texture", atlas, 0);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_texture_tiling", 1.0f);
		quad.render(GL_LINE_STRIP);
	}
	//hacer draw call
	quad.render(GL_TRIANGLES);

	shader->disable();
}
