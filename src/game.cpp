#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"

#include <cmath>


//some globals
Animation* anim = NULL;
FBO* fbo = NULL;
Game* Game::instance = NULL;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
	
	initWorldTienda();

	currentWorld = tienda;
	currentStage = intro;

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/gui.fs");
	Texture* atlas = Texture::Get("data/atlas.png");
	//Texture* atlas = Texture::Get("data/gui/Crosshair.png");

	gui = new Gui(shader, atlas);
	gui->initAtlas();
	gui->initIntroButtons();
	gui->initPauseButtons();

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	
	currentStage->render(currentWorld);
	
	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	currentStage->update(seconds_elapsed, currentWorld);
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		//case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}

	
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse
	{
		Camera* camera = Camera::current;
		if (currentStage == intro) {
			gui->introButtonPressed(Vector2(Input::mouse_position.x, Input::mouse_position.y));
		}
		if (currentStage == play) {
			if (mouse_locked) currentWorld->disparar();
			else gui->pauseButtonPressed(Vector2(Input::mouse_position.x, Input::mouse_position.y));
		}
		if (currentStage == editor) {
			if (mouse_locked) {
				Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, this->window_width, this->window_height);
				currentWorld->selectEntityEditor(dir);
			}
			else gui->pauseButtonPressed(Vector2(Input::mouse_position.x, Input::mouse_position.y));
		}
	}
	if (event.button == SDL_BUTTON_RIGHT) //right mouse
	{
		if (currentStage == editor) {
			if (currentWorld->selectedEntity != NULL) {
				currentWorld->selectedEntity->bounding = false;
				currentWorld->selectedEntity = NULL;
			}
		}
	}

}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{
}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}



void Game::initWorldTienda(){

	// example of shader loading using the shaders manager
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	tienda = new World(shader);
	Matrix44 m;
	m.setTranslation(0, 0, 0);

	//libros se ve to mal
	Entity* barba =  new Entity(0,0,0 , m);
	barba->loadMesh("data/classic/2.Structure/Doors/marco_grande.obj");
	barba->loadTexture("data/classic/2.Structure/Doors/Materials/cabinet.png");
	tienda->addEditorEntity(barba);

	/*world->loadEntity(Entity, patata);*/
	
	maxEditorEntities += 1;
	Entity* box = new Entity(0, 0, 0, m);
	box->loadMesh("data/classic/1.Decoration/Boxes/caja.obj");
	box->loadTexture("data/classic/1.Decoration/Boxes/Materials/box4.png");
	tienda->addEditorEntity(box);
	
	
	maxEditorEntities += 1;
	Entity* player = new Entity(0, 0, 0, m);
	player->loadMesh("data/players/export_man.obj");
	player->loadTexture("data/players/man.png");
	tienda->addEditorEntity(player);

	
	Entity* ciudad = new Entity(0, 0, 0, m);
	ciudad->mesh = new Mesh();
	ciudad->mesh->createPlane(2000);
	//hacerlo tiled
	ciudad->loadTexture("data/classic/2.Structure/Ground/Materials/Rocks_05.png");
	tienda->addEntity(ciudad);

	Vector3 playerInitPos = Vector3(0, 0.5, 0);
	initPlayer(playerInitPos, tienda);

	//tienda->createZombies();

	initSky(tienda);
	
	initCamera(tienda);

}

void Game::initPlayer(Vector3 pos, World* world) {
	Matrix44 m;
	m.setTranslation(pos.x, pos.y, pos.z);
	Player* player = (Player*) new Entity(pos, m);
	player->loadMesh("data/LowPolyCharacterPack/mujeh1.obj");
	player->loadTexture("data/LowPolyCharacterPack/mujeh1.png");
	player->setVel(2.0f);
	world->addPlayer(player);
}

void Game::initSky(World* world) {
	Matrix44 m;
	Vector3 playerPos = world->player->getPos();
	m.setTranslation(playerPos.x, playerPos.y, playerPos.z);
	world->sky = new Entity(playerPos, m);
	world->sky->loadMesh("data/Ambiente/cielo.ASE");
	world->sky->loadTexture("data/Ambiente/cielo.tga");
}

void Game::initCamera(World* world) {
	camera = new Camera();
	Camera::current = camera;
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
	Vector3 eye = world->player->m * Vector3(0.f, 1.f, 0.f);
	Vector3 center = world->player->m * Vector3(0.f, 5.f, -1.f);
	Vector3 up = world->player->m.rotateVector(Vector3(0.f, 1.f, 0.f));
	camera->lookAt(eye, center, up);
}

void Game::setIntroStage(){
	currentStage = intro;
	mouse_locked = false;
	SDL_ShowCursor(!mouse_locked);
}

void Game::setPlayStage(){
	currentStage = play;
	mouse_locked = true;
	SDL_ShowCursor(!mouse_locked);
}

void Game::setEditorStage(){
	currentStage = editor;
	mouse_locked = true;
	SDL_ShowCursor(!mouse_locked);
}

void Game::setEndStage(){
	currentStage = end;
	mouse_locked = false;
	SDL_ShowCursor(!mouse_locked);
}
