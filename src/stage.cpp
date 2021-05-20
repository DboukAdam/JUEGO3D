#include "game.h"
#include "input.h"
#include "stage.h"
#include "shader.h"

Game* game = Game::instance;


void IntroStage::render() {

}

void IntroStage::update(double seconds_elapsed) {
	World* world = Game::instance->world;
	if (Input::isKeyPressed(SDL_SCANCODE_P)) world->current = world->home;
	if (Input::isKeyPressed(SDL_SCANCODE_M)) world->current = world->menu;
}

void PlayStage::render() {

}

void PlayStage::update(double seconds_elapsed) {

}

void EndStage::render() {

}

void EndStage::update(double seconds_elapsed) {
	World* world = Game::instance->world;
	if (Input::isKeyPressed(SDL_SCANCODE_R)) world->current = world->intro;
	if (Input::isKeyPressed(SDL_SCANCODE_M)) world->current = world->menu;
}

void MenuStage::render()
{
}

void MenuStage::update(double seconds_elapsed)
{
	World* world = Game::instance->world;
	if (Input::isKeyPressed(SDL_SCANCODE_ESCAPE)) world->current = world->home;
}

void HomeStage::render()
{
}

void HomeStage::update(double seconds_elapsed)
{
	World* world = Game::instance->world;
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) world->current = world->market;
}

void MarketStage::render()
{
	World* world = Game::instance->world;
	//create model matrix for cube
	Matrix44 m;
	m.rotate(game->angle * DEG2RAD, Vector3(0, 1, 0));

	if (world->shader)
	{
		//enable shader
		world->shader->enable();

		//upload uniforms
		world->shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		world->shader->setUniform("u_viewprojection", game->camera->viewprojection_matrix);
		world->shader->setUniform("u_texture", world->texture, 0);
		world->shader->setUniform("u_model", m);
		world->shader->setUniform("u_time", time);

		//do the draw call
		//mesh->render( GL_TRIANGLES );

		world->shader->setUniform("u_texture", world->shop->texture, 0);
		world->shader->setUniform("u_model", world->shop->m);
		world->shop->mesh->render(GL_TRIANGLES);
		//world->shader->setUniform("u_texture", world->zzz->texture, 0);
		//world->shader->setUniform("u_model", world->zzz->m);
		//world->zzz->mesh->render(GL_TRIANGLES);
		world->shader->setUniform("u_model", world->suelo->m);
		world->shader->setUniform("u_texture", world->suelo->texture, 0);
		world->suelo->mesh->render(GL_TRIANGLES);

		//disable shader
		world->shader->disable();
	}
}

void MarketStage::update(double seconds_elapsed)
{
	World* world = Game::instance->world;
	float speed = 5.0f;
	//if (Input::isKeyPressed(SDL_SCANCODE_UP)) world->zzz->pos.z += 1.0f * speed;
	//if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) world->zzz->pos.z -= 1.0f * speed;
	//if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) world->zzz->pos.x += 1.0f * speed;
	//if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) world->zzz->pos.x -= 1.0f * speed;

	World* world = Game::instance->world;
	if (Input::isKeyPressed(SDL_SCANCODE_A)) world->current = world->home;
}
