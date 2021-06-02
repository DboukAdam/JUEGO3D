#include "stage.h"
#include "input.h"
#include "game.h"
#include "pathfinders.h"


int startx; //ULTRA SUCIO
int starty;
int output[100];
int path_steps = -1;
float tileSizeX = 10.f; //MEGA SUCIO
float tileSizeY = 10.f;
int width = 300;
int height = 300; //SUPER SUCIO

bool free_camera = false;


void IntroStage::render(World* world) {

	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 1.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//gui class

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}

void IntroStage::update(double seconds_elapsed, World* world) {

	if (Input::mouse_state & SDL_BUTTON_LEFT) {

	}
	
	
}

void PlayStage::render(World* world) {
	Game* game = Game::instance;
	//set the clear color (the background color) and Clear the window and the depth buffer
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	Camera* camera = Camera::current;

	
	world->player->CamPlayer(camera);
	

	//skymap
	world->sky->m.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//enable shader
	Shader* shader = world->shader;
	shader->enable();
	world->sky->render(shader);


	//.........................................................................................
	//IA CAMBIAR
	//if (path_steps > 0) {
	//	Mesh mesh;
	//	for (size_t i = 0; i < path_steps; i++) {
	//		int gridIndex = output[i];
	//		int posxgrid = gridIndex % width;
	//		int posygrid = gridIndex / width;

	//		Vector3 pos = Vector3(posxgrid*tileSizeX, 1.0f, posygrid*tileSizeY);
	//		mesh.vertices.push_back(pos);
	//	}
	//	if (shader)
	//	{
	//		//upload uniforms
	//		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	//		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	//		shader->setUniform("u_time", time);
	//		//shader->setUniform("u_texture", texture, 0);
	//		shader->setUniform("u_model", Matrix44());
	//		//shader->setUniform("u_texture_tiling", tiling);
	//		mesh.render(GL_LINE_STRIP);
	//	}
	//}
	//.........................................................................................

	world->RenderEntities(camera);
	world->RenderZombies(camera);	

	//disable shader
	shader->disable();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}

void PlayStage::update(double seconds_elapsed, World* world) {
	Game* game = Game::instance;
	Camera* camera = Camera::current;
	Player* player = world->player;
	Shader* shader = world->shader;

	float speed = seconds_elapsed * game->mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
	
	
	if (game->mouse_locked)
	{
		float camera_speed = 10.0f;
		float targetPitch = player->pitch - (Input::mouse_delta.y * seconds_elapsed * camera_speed);

		player->yaw -= Input::mouse_delta.x * seconds_elapsed * camera_speed;
		if(targetPitch > -80 && targetPitch < 80) player->pitch -= Input::mouse_delta.y * seconds_elapsed * camera_speed;
		Input::centerMouse();
		Matrix44 playerRot;
		playerRot.setRotation(player->yaw * DEG2RAD, Vector3(0, 1, 0));

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
	else {
		//menu de pause
	}

	
	//dejar solo mientras desarrollamos
	if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) {
		Game* game = Game::instance;
		game->currentStage = game->editor;
	}
	

	
	//.................................................................................................................................................
	//IA CAMBIAR
	//if (Input::wasKeyPressed(SDL_SCANCODE_J)) {
	//	Vector3 origion = camera->eye;
	//	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, game->window_width, game->window_height);
	//	Vector3 pos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), origion, dir);

	//	//startx = clamp(pos.x / tileSizeX, 0, width);
	//	//starty = clamp(pos.y / tileSizeY, 0, height);
	//	startx = pos.x / tileSizeX;
	//	starty = pos.y / tileSizeY;
	//}
	//if (Input::wasKeyPressed(SDL_SCANCODE_K)) {
	//	Vector3 origion = camera->eye;
	//	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, game->window_width, game->window_height);
	//	Vector3 pos = RayPlaneCollision(Vector3(), Vector3(0, 1, 0), origion, dir);

	//	float targetx = clamp(pos.x / tileSizeX, 0, width);
	//	float targetz = clamp(pos.z / tileSizeY, 0, height);

	//	path_steps = AStarFindPathNoTieDiag(
	//		startx, starty, //origin (tienen que ser enteros)
	//		targetx, targetz, //target (tienen que ser enteros)
	//		world->map, //pointer to map data
	//		width, height, //map width and height
	//		output, //pointer where the final path will be stored
	//		100); //max supported steps of the final path

	//	std::cerr << "Number of steps: " << path_steps << std::endl;
	//}
	//.................................................................................................................................................

	//to navigate with the mouse fixed in the middle
	if (game->mouse_locked)
		Input::centerMouse();
}

void EditorStage::render(World* world)
{
	Game* game = Game::instance;
	//set the clear color (the background color) and Clear the window and the depth buffer
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	Camera* camera = Camera::current;

	//skymap
	world->sky->m.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//enable shader
	Shader* shader = world->shader;
	shader->enable();
	world->sky->render(shader);

	world->RenderEntities(camera);
	world->RenderZombies(camera);

	//disable shader
	shader->disable();

	//pintando bounding muy feo
	world->RenderBoundingEntities(camera);
	world->RenderBoundingZombies(camera);

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}

void EditorStage::update(double seconds_elapsed, World* world)
{
	Game* game = Game::instance;
	Camera* camera = Camera::current;
	Player* player = world->player;
	Shader* shader = world->shader;

	float speed = seconds_elapsed * game->mouse_speed;

	if (game->mouse_locked)
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 30;
		if (Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);

		if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {
			Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, game->window_width, game->window_height);
			world->addObjectEditor(Mesh::Get("data/LowPolyCharacterPack/mujeh1.obj"), Texture::Get("data/LowPolyCharacterPack/mujeh1.png"), dir);
		}

		if (Input::wasKeyPressed(SDL_SCANCODE_X)) {
			Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, game->window_width, game->window_height);
			world->addObjectEditor(Mesh::Get("data/LowPolyCharacterPack/mujeh1.obj"), Texture::Get("data/LowPolyCharacterPack/mujeh1.png"), dir);
		}

		/*if (Input::mouse_state & SDL_BUTTON_LEFT) {
			Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, game->window_width, game->window_height);
			world->selectEntityEditor(dir);
		}*/

		if (Input::wasKeyPressed(SDL_SCANCODE_Q)) {
			if (!world->selectedEntity == NULL) {
				world->selectedEntity->m.rotate(10.0f * DEG2RAD, Vector3(0, 1, 0));
			}
		}

		if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
			if (!world->selectedEntity == NULL) {
				world->selectedEntity->m.rotate(-10.0f * DEG2RAD, Vector3(0, 1, 0));
			}
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_A)) {
			if (!world->selectedEntity == NULL) {
				world->selectedEntity->m.translate(-1, 0, 0);
			}
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_D)) {
			if (!world->selectedEntity == NULL) {
				world->selectedEntity->m.translate(1, 0, 0);
			}
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_S)) {
			if (!world->selectedEntity == NULL) {
				world->selectedEntity->m.translate(0, 0, -1);
			}
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_W)) {
			if (!world->selectedEntity == NULL) {
				world->selectedEntity->m.translate(0, 0, 1);
			}
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_S)) {
			if (!world->selectedEntity == NULL) {
				world->selectedEntity->m.translate(0, 1, 0);
			}
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_W)) {
			if (!world->selectedEntity == NULL) {
				world->selectedEntity->m.translate(0, -1, 0);
			}
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_P)) {
			if (!world->selectedEntity == NULL) {
				//no funciona bien
				world->selectedEntity->~Entity();
			}
		}

		if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) {
			Game* game = Game::instance;
			game->currentStage = game->play;
		}
		
	}
	else { //menu de pause


	}

}
