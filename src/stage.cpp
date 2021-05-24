#include "stage.h"
#include "input.h"

bool free_camera = false;

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
	player->m.setTranslation(player->pos.x, player->pos.y, player->pos.z);
	player->m.rotate(player->angle * DEG2RAD, Vector3(0.0f, 1.0f, 0.0f));
	camera->eye = player->m * Vector3(0.f, 2.f, 0.f);

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//enable shader
	Shader* shader = world->shader;
	shader->enable();
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* entity = world->entities[i];
		if (entity == NULL) {
			break;
		}
		entity->render(shader);
	}
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		Zombie* zombie = world->zombies[i];
		if (zombie == NULL) {
			break;
		}
		zombie->render(shader);
	}
	//disable shader
	shader->disable();
	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}

void PlayStage::update(double seconds_elapsed, World* world) {
	Game* game = Game::instance;
	Camera* camera = Camera::current;
	Player* player = world->player;

	float speed = seconds_elapsed * game->mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
	
	if (free_camera) {
		//async input to move the camera around
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}
	else {
		// mouse input to rotate the cam
		if ((Input::mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked) //is left button pressed?
		{
			//camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			//camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
			player->angle = Input::mouse_delta.x * 0.005f;
			camera->center = camera->unproject(Vector3(Input::mouse_position.x, Input::mouse_position.y, 1), game->window_width, game->window_height);
		}

		Matrix44 playerRot;
		playerRot.setRotation(player->angle * DEG2RAD, Vector3(0, 1, 0));

		Vector3 playerFront = playerRot.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
		Vector3 playerRight = playerRot.rotateVector(Vector3(1.0f, 0.0f, 0.0f));
		Vector3 playerSpeed;

		if (Input::isKeyPressed(SDL_SCANCODE_W)) {
			playerSpeed = playerSpeed + (playerFront * -speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_S)) {
			playerSpeed = playerSpeed + (playerFront * speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_A)) {
			playerSpeed = playerSpeed + (playerRight * speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_D)) {
			playerSpeed = playerSpeed + (playerRight * -speed);
		}

		Vector3 targetPos = player->pos + playerSpeed;
		player->pos = targetPos;
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) {
		free_camera = !free_camera;
	}

	//to navigate with the mouse fixed in the middle
	if (game->mouse_locked)
		Input::centerMouse();
}

