#include "stage.h"
#include "input.h"
#include "game.h"
#include "pathfinders.h"
#define MAX_ENTITIES 1000

bool free_camera = false;

void IntroStage::render(World* world) {
	Game* game = Game::instance;

	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	game->gui->RenderIntroGui();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}

void IntroStage::update(double seconds_elapsed, World* world) {
	Game* game = Game::instance;
	
}

void SelectWorldStage::render(World* world) {
	Game* game = Game::instance;

	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	game->gui->RenderWorldsGui();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);
}

void SelectWorldStage::update(double seconds_elapsed, World* world) {

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
	//world->ground->render(shader);

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
	world->RenderPlayer(camera);
	world->RenderStatic(camera);
	world->RenderDynamic(camera);
	world->RenderZombies(camera, game->time);
	//disable shader
	shader->disable();

	//GUI STUFF
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (game->mouse_locked) {
		game->gui->RenderCrosshair();
	}
	else {
		game->gui->RenderPauseMenu();
	}

	world->moveZombies();
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
		Vector3 playerTargetCenter = targetPos + Vector3(0, 1, 0);
		for (int i = 0; i < MAX_ENTITIES; i++) {
			
			if (!world->staticEntities[i] == NULL) {

				Entity* current = world->staticEntities[i];
			
				Vector3 coll;
				Vector3 collNormal;
				if (current->mesh != NULL) {

					if (!current->mesh->testSphereCollision(current->m, playerTargetCenter, 0.5, coll, collNormal)) continue;
			
					Vector3 push_away = normalize(coll - playerTargetCenter) * seconds_elapsed;
					targetPos = player->pos - push_away;
					targetPos.y = player->pos.y;
				}
			}

			if (!world->dynamicEntities[i] == NULL) {

				Entity* current = world->dynamicEntities[i];

				Vector3 coll;
				Vector3 collNormal;

				if (!current->mesh->testSphereCollision(current->m, playerTargetCenter, 0.5, coll, collNormal)) continue;
				current->m.translate(0,0,-1);

			}
			
		}
		player->pos = targetPos;
	}
	else {
	}
	
	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
		game->mouse_locked = !game->mouse_locked;
		SDL_ShowCursor(!game->mouse_locked);
	}
	
	//dejar solo mientras desarrollamos
	if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) {
		game->setEditorStage();
	}

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

	
	Shader* shader = world->shader;
	
	world->sky->render(shader);
	//world->ground->render(shader);
	world->RenderStatic(camera);
	world->RenderDynamic(camera);
	world->RenderZombies(camera, game->time);

	if (world->selectedEntity == NULL) {
		Vector3 origin = camera->eye;//unproject center coord of the screen
		Vector3 dir = camera->getRayDirection(400, 300, 800, 600);
		Vector3 pos = RayPlaneCollision(Vector3(0, 0, 0), Vector3(0, 1, 0), origin, dir);
		if (world->isStaticObject) {
			Entity* entidad = world->structures[world->numEntity];
			entidad->m.setTranslation(pos.x, pos.y, pos.z);
			entidad->m.rotate(entidad->yaw * DEG2RAD, Vector3(0,1,0));
			entidad->render(shader);
		}
		else {
			Entity* entidad = world->decoration[world->numEntity];
			entidad->m.setTranslation(pos.x, pos.y, pos.z);
			entidad->m.rotate(entidad->yaw * DEG2RAD, Vector3(0, 1, 0));
			entidad->render(shader);
		}
	}

	if (world->isStaticObject) {
		world->numEntity = world->numStructure;
	}
	else {
		world->numEntity = world->numDecoration;
	}

	//pintando bounding muy feo
	world->RenderBoundingStatic(camera);
	world->RenderBoundingDynamic(camera);
	world->RenderBoundingZombies(camera);

	//Draw the floor grid
	drawGrid();

	//Render GUI
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (game->mouse_locked) {
		game->gui->RenderCrosshair();
	}
	else {
		game->gui->RenderPauseMenu();
	}
	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//draw text para ver la mesh que voy a pintar
	string asset;
	
	if (world->isStaticObject) {
		drawText(20, 20, world->structures[world->numEntity]->type, Vector3(1, 1, 1), 2);
	}
	else {
		drawText(20, 20, world->decoration[world->numEntity]->type, Vector3(1, 1, 1), 2);
	}
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
		float angleY = Input::mouse_delta.y * 0.0005f;
		
		if (angleY > -80 && angleY < 80) {

			camera->rotate(Input::mouse_delta.x * 0.0005f, Vector3(0.0f, -1.0f, 0.0f));
			camera->rotate(angleY, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}
		
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 30;
		if (Input::isKeyPressed(SDL_SCANCODE_W)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);

		if (Input::wasKeyPressed(SDL_SCANCODE_C)) {
			world->isStaticObject = !world->isStaticObject;
			if (world->isStaticObject) {
				std::cout << "Cambio de objectos a Structure" << std::endl;
			}
			else {
				std::cout << "Cambio de objectos a Decoration" << std::endl;
			}
		}

		if (Input::wasKeyPressed(SDL_SCANCODE_SPACE)) {
			
			Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, 800, 600);
			if (world->isStaticObject) {
				world->addObjectEditor(world->structures[world->numEntity], dir);
			}
			else {
				world->addObjectEditor(world->decoration[world->numEntity], dir);
			}
			
		}

		if (Input::wasKeyPressed(SDL_SCANCODE_X)) {
			if (world->isStaticObject) {
				int tmp = world->numStructure + 1;
				if (tmp > world->maxStructure) {
					world->numStructure = 0;
				}
				else {
					world->numStructure += 1;
				}
			}
			else {
				int tmp = world->numDecoration + 1;
				if (tmp > world->maxDecoration) {
					world->numDecoration = 0;
				}
				else {
					world->numDecoration += 1;
				}
			}
			
		}

		if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {
			if (world->isStaticObject) {
				int tmp = world->numStructure - 1;
				if (tmp < 0) {
					world->numStructure = world->maxStructure;
				}
				else {
					world->numStructure -= 1;
				}
			}
			else {
				int tmp = world->numDecoration - 1;
				if (tmp < 0) {
					world->numDecoration = world->maxDecoration;
				}
				else {
					world->numDecoration -= 1;
				}
			}
		}


		if (Input::wasKeyPressed(SDL_SCANCODE_Q)) {
			if (world->selectedEntity != NULL) {
				world->selectedEntity->m.rotate(45.0f * DEG2RAD, Vector3(0, 1, 0));
			}
			else {
				if (world->isStaticObject) {
					world->structures[world->numEntity]->yaw += 45.0f;
				}
				else {
					world->decoration[world->numEntity]->yaw += 45.0f;
				}
			}
		}

		if (Input::wasKeyPressed(SDL_SCANCODE_E)) {
			if (world->selectedEntity != NULL) {
				world->selectedEntity->m.rotate(-45.0f * DEG2RAD, Vector3(0, 1, 0));
			}
			else {
				if (world->isStaticObject) {
					world->structures[world->numEntity]->yaw -= 45.0f;
				}
				else {
					world->decoration[world->numEntity]->yaw -= 45.0f;
				}
			}
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_UP)) {
			if (!world->selectedEntity == NULL) {
				world->selectedEntity->m.translate(0, 0.5, 0);
			}
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)) {
			if (!world->selectedEntity == NULL) {
				world->selectedEntity->m.translate(0, -0.5, 0);
			}
		}

		if (Input::wasKeyPressed(SDL_SCANCODE_LEFT)) {
			if (!world->selectedEntity == NULL) {
				world->selectedEntity->m.translate(-0.5, 0, 0);
			}
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT)) {
			if (!world->selectedEntity == NULL) {
				world->selectedEntity->m.translate(0.5, 0, 0);
			}
		}

		if (Input::wasKeyPressed(SDL_SCANCODE_P)) {
			if (world->selectedEntity != NULL) {
				world->deleteEntity(world->selectedEntity);
				world->selectedEntity->DeleteEntity();
				world->selectedEntity = NULL;
			}
		}

		if (Input::wasKeyPressed(SDL_SCANCODE_TAB)) {
			game->setPlayStage();
		}

		if (game->mouse_locked)
			Input::centerMouse();

		if (Input::wasKeyPressed(SDL_SCANCODE_G)) {
			cout << "Que nombre quieres ponerle?" << endl;
			string filename;
			getline(cin, filename);
			world->saveWorldInfo(filename);
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_L)) {
			cout << "Que archivo quieres cargar?" << endl;
			string filename;
			getline(cin, filename);
			world->loadWorldInfo(filename);
		}
	}
	else { //menu de pause

	}
	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
		game->mouse_locked = !game->mouse_locked;
		SDL_ShowCursor(!game->mouse_locked);
	}
}

