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
	if (!free_camera) {
		//camera->eye = player->m * Vector3(0.f, 2.f, 0.f);
		Vector3 eye = player->m * Vector3(0.0f, 2.0f, -0.5f);
		Vector3 forward = player->m.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
		Vector3 center = eye + forward;
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		camera->lookAt(eye, center, up);
		//world->crossHair->m.setTranslation(camera->center.x, camera->center.y, camera->center.z);
	}
	world->sky->m.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);


	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//enable shader
	Shader* shader = world->shader;
	shader->enable();
	
	const int planewidth = 100;
	const int planeheight = 100;
	const float padding = world->cesped->mesh->box.center.distance(world->cesped->mesh->box.halfsize);
	const float offset = 300;

	for (int i = 0; i < planewidth; i++)
	{
		for (int j = 0; j < planeheight; j++)
		{
			world->cesped->m.setTranslation((i * padding)-offset, -0.60f, (j * padding)-offset);
			BoundingBox currentBox = transformBoundingBox(world->cesped->m, world->cesped->mesh->box);
			if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
			world->cesped->render(shader);
		}
	}

	//world->crossHair->render(shader);


	world->sky->render(shader);
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* entity = world->entities[i];
		if (entity == NULL) {
			break;
		}
		BoundingBox currentBox = transformBoundingBox(entity->m, entity->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		entity->render(shader);
		
	}
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		Zombie* zombie = world->zombies[i];
		if (zombie == NULL) {
			break;
		}
		BoundingBox currentBox = transformBoundingBox(zombie->m, zombie->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		zombie->render(shader);
		
	}

	//world->cesped->render(shader, 100);
	world->cesped->render(shader);

	//disable shader
	shader->disable();
	
	//pintando bounding muy feo
	for (int i = 0; i < MAX_ENTITIES; i++) {
		Entity* entity = world->entities[i];
		if (entity == NULL) break;
		BoundingBox currentBox = transformBoundingBox(entity->m, entity->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		if(entity->bounding) entity->mesh->renderBounding(entity->m);
	}
	for (int i = 0; i < MAX_ZOMBIES; i++) {
		Zombie* zombie = world->zombies[i];
		if (zombie == NULL) break;
		BoundingBox currentBox = transformBoundingBox(zombie->m, zombie->mesh->box);
		if (!camera->testBoxInFrustum(currentBox.center, currentBox.halfsize)) continue;
		if (zombie->bounding) zombie->mesh->renderBounding(zombie->m);
	}
	
	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}

void PlayStage::update(double seconds_elapsed, World* world) {
	Game* game = Game::instance;
	Camera* camera = Camera::current;
	Player* player = world->player;
	Shader* shader = world->shader;

	float speed = seconds_elapsed * game->mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
	
	if (free_camera) {
		if ((Input::mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked)
		{
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}

		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 100;
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}
	else {
		if ((Input::mouse_state & SDL_BUTTON_LEFT) || game->mouse_locked)
		{
			player->angle += -Input::mouse_delta.x * 0.1f;
			//camera->center = camera->unproject(Vector3(Input::mouse_position.x, Input::mouse_position.y, 1), game->window_width, game->window_height);
		}

		Matrix44 playerRot;
		playerRot.setRotation(player->angle * DEG2RAD, Vector3(0, 1, 0));

		Vector3 playerFront = playerRot.rotateVector(Vector3(0.0f, 0.0f, -1.0f));
		Vector3 playerRight = playerRot.rotateVector(Vector3(1.0f, 0.0f, 0.0f));
		Vector3 playerSpeed;

		if (Input::isKeyPressed(SDL_SCANCODE_W)) {
			playerSpeed = playerSpeed + (playerFront * speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_S)) {
			playerSpeed = playerSpeed + (playerFront * -speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_A)) {
			playerSpeed = playerSpeed + (playerRight * -speed);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_D)) {
			playerSpeed = playerSpeed + (playerRight * speed);
		}

		Vector3 targetPos = player->pos + playerSpeed;
		player->pos = targetPos;
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) {
		free_camera = !free_camera;
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
		world->disparar();
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_1)) {
		Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, game->window_width, game->window_height);
		world->addObjectEditor(Mesh::Get("data/Shop/Shop-0-ShopBuilding_1.obj"), Texture::Get("data/Shop/Shop-0-ShopBuilding_1.png"), dir);
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_2)) {
		Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, game->window_width, game->window_height);
		world->addObjectEditor(Mesh::Get("data/ZombieScale.obj"), Texture::Get("data/Zombie/Zed_1.png"), dir);
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_0)) {
		Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, game->window_width, game->window_height);
		world->selectEntityEditor(dir);
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_9)) {
		if (!world->selectedEntity == NULL) {
			world->selectedEntity->m.rotate(10.0f * DEG2RAD, Vector3(0, 1, 0));
		}
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_8)) {
		if (!world->selectedEntity == NULL) {
			world->selectedEntity->m.rotate(-10.0f * DEG2RAD, Vector3(0, 1, 0));
		}
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_7)) {
		if (!world->selectedEntity == NULL) {
			world->selectedEntity->m.translate(0, -1, 0);
		}
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_6)) {
		if (!world->selectedEntity == NULL) {
			world->selectedEntity->m.translate(0, 1, 0);
		}
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_P)) {
		if (!world->selectedEntity == NULL) {
			world->selectedEntity->~Entity();
		}
	}
	//to navigate with the mouse fixed in the middle
	if (game->mouse_locked)
		Input::centerMouse();
}

