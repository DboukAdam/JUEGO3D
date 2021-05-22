#include "stage.h"
#include "input.h"

void IntroStage::render(World* world) {
	
}

void IntroStage::update(double seconds_elapsed, World* world) {

}

void PlayStage::render(World* world) {
	Game* game = Game::instance;
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	Camera* camera = Camera::current;
	camera->enable();
	Player* player = world->player;

	Vector3 eye = player->m * Vector3(0.f, 6.f, 0.f);// donde la X sera la pos de la cabeza
	Vector3 center = player->m * Vector3(0.f, 0.f, 1.f);
	camera->lookAt(eye, center, Vector3(0.f,1.f,0.f));//, donde el up sera(0, 1, 0) y no hara falta preocuparse por ahora por el hecho de que el zombie no rotara sobre si mismo

	

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//create model matrix for cube
	Matrix44 m;
	//m.rotate(game->angle * DEG2RAD, Vector3(0, 1, 0));
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* entity = world->entities[i];
		if (entity == NULL) {
			break;
		}
		entity->render();
	}
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		Zombie* zombie = world->zombies[i];
		if (zombie == NULL) {
			break;
		}
		zombie->render();
	}

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}

void PlayStage::update(double seconds_elapsed, World* world) {
	Game* game = Game::instance;
	Camera* camera = Camera::current;

	float speed = seconds_elapsed * game->mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}

	//async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	
	if (Input::isKeyPressed(SDL_SCANCODE_W)) {
		Player* player = world->player;
		player->m.translate(0, 0, player->vel);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_S)) {
		Player* player = world->player;
		player->m.translate(0, 0, -player->vel);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_A)) {
		Player* player = world->player;
		player->m.translate(player->vel, 0, 0);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_D)) {
		Player* player = world->player;
		player->m.translate(-player->vel, 0, 0);
	}

	//to navigate with the mouse fixed in the middle
	if (game->mouse_locked)
		Input::centerMouse();
}

