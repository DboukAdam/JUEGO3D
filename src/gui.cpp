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

void Gui::initIntroButtons() {
	Game* game = Game::instance;
	float button_width = 320;
	float button_height = 80;
	float buttonOffset = 10;
	float xPos = button_width / 2 + buttonOffset;
	float yPos = game->window_height / 5;
	for (int i = 1; i < numIntroButtons + 1; i++) {
		introButtons[i - 1] = new Button(Vector2(xPos, yPos * i), atlasRanges[greenButton], button_width, button_height);
	}
}

void Gui::RenderIntroGui()
{
	Game* game = Game::instance;
	Camera cam2D;
	cam2D.setOrthographic(0, game->window_width, game->window_height, 0, -1, 1);

	shader->enable();
	if (shader)
	{
		//upload uniforms
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_viewprojection", cam2D.viewprojection_matrix);
		shader->setUniform("u_texture", atlas, 0);
		shader->setUniform("u_model", Matrix44());
		shader->setUniform("u_texture_tiling", 1.0f);

		for (int i = 0; i < numIntroButtons; i++) {
			Button* button = introButtons[i];
			shader->setUniform("u_tex_range", button->range);
			button->mesh.render(GL_LINE_STRIP);
			button->mesh.render(GL_TRIANGLES);
		}
	}
	shader->disable();
	
	float scale = 5.0f;
	float textSize = 7 * scale;
	float yOffset = 14.0f;
	float xOffset = 20;
	std::string text[] = { "play", "editor", "settings", "exit" };
	for (int i = 0; i < numIntroButtons; i++) {
		float yPos = introButtons[i]->pos.y;
		drawText(xOffset, yPos - yOffset - textSize/2, text[i], Vector3(1, 1, 1), scale);
	}
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
