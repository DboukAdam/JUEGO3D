#include "gui.h"
#include "camera.h"
#include "game.h"

Gui::Gui(Shader* shader, Texture* atlas) {
	this->shader = shader;
	this->atlas = atlas;
}

void Gui::initAtlas() {
	atlasRanges[0] = Vector4(0, 0, 640 / atlas->width, 160 / atlas->height); //greenButton
	atlasRanges[1] = Vector4(0, (160 + 16) / atlas->height, 640 / atlas->width, 208 / atlas->height); //greenButtonPressed
	atlasRanges[2] = Vector4(1 - 80 / atlas->width, 0, 80 / atlas->width, 80 / atlas->height); //crosshair
	atlasRanges[3] = Vector4(0, (160 + 16 + 208 + 16) / atlas->height, 640 / atlas->width, 160 / atlas->height); //fleshButton
	atlasRanges[4] = Vector4(0, (160 + 16 + 208 + 16 + 160 + 16) / atlas->height, 640 / atlas->width, 208 / atlas->height); //fleshButtonPressed
}

void Gui::initIntroButtons() {
	Game* game = Game::instance;
	float button_width = 320;
	float button_height = 80;
	float buttonOffset = 10;
	float xPos = button_width / 2 + buttonOffset;
	float yPos = game->window_height / (numIntroButtons + 1);
	for (int i = 1; i < numIntroButtons + 1; i++) {
		introButtons[i - 1] = new Button(Vector2(xPos, yPos * i), atlasRanges[greenButton], button_width, button_height, false);
	}
}

void Gui::initPauseButtons() {
	Game* game = Game::instance;
	float button_width = 320;
	float button_height = 80;
	float buttonOffset = 10;
	float xPos = button_width / 2 + buttonOffset;
	float yPos = game->window_height / (numPauseButtons + 1);
	for (int i = 1; i < numPauseButtons + 1; i++) {
		pauseButtons[i - 1] = new Button(Vector2(xPos, yPos * i), atlasRanges[fleshButton], button_width, button_height, false);
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

void Gui::RenderPauseMenu() {
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

		for (int i = 0; i < numPauseButtons; i++) {
			Button* button = pauseButtons[i];
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
	std::string text[] = { "resume", "settings", "main menu" };
	for (int i = 0; i < numPauseButtons; i++) {
		float yPos = pauseButtons[i]->pos.y;
		drawText(xOffset, yPos - yOffset - textSize / 2, text[i], Vector3(1, 1, 1), scale);
	}
}

void Gui::introButtonPressed(Vector2 pos) {
	Game* game = Game::instance;
	int i = 0;
	for (i; i < numIntroButtons; i++) {
		Button* button = introButtons[i];
		int min_x = button->pos.x - (button->width / 2);
		int min_y = button->pos.y - (button->height / 2);
		int max_x = button->pos.x + (button->width / 2);
		int max_y = button->pos.y + (button->height / 2);
		if (pos.x > min_x && pos.y > min_y && pos.x < max_x && pos.y < max_y) break;
		if (i == numIntroButtons - 1) i++;
	}
	if (i == 0) game->setPlayStage();
	if (i == 1) game->setEditorStage();
	if (i == 2) std::cout << "Hola, soy las settings, encantado." << std::endl;
	if (i == 3) game->must_exit = true;
}

void Gui::pauseButtonPressed(Vector2 pos) {
	Game* game = Game::instance;
	int i = 0;
	for (i; i < numPauseButtons; i++) {
		Button* button = pauseButtons[i];
		int min_x = button->pos.x - (button->width / 2);
		int min_y = button->pos.y - (button->height / 2);
		int max_x = button->pos.x + (button->width / 2);
		int max_y = button->pos.y + (button->height / 2);
		if (pos.x > min_x && pos.y > min_y && pos.x < max_x && pos.y < max_y) break;
		if (i == numIntroButtons - 1) i++;
	}
	if (i == 0) {
		game->mouse_locked = !game->mouse_locked;
		SDL_ShowCursor(!game->mouse_locked);
	}
	if (i == 1) std::cout << "Hola, soy las settings, encantado." << std::endl;
	if (i == 2) game->setIntroStage();
}
