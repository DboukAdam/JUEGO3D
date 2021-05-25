#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"

#include <cmath>
#include "stage.h"

//some globals
Animation* anim = NULL;

FBO* fbo = NULL;

Game* Game::instance = NULL;

Stage* current;
IntroStage* intro = new IntroStage();
PlayStage* play = new PlayStage();
EndStage* end = new EndStage();

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
	mouse_locked = true;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	current = play;

	initWorldTienda();

	currentWorld = tienda;

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	current->render(currentWorld);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	current->update(seconds_elapsed, currentWorld);
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
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
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
	mouse_speed *= event.y > 0 ? 1.1 : 0.9;
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
	m.setTranslation(0, -0.65, 0);
	Entity* suelo = new Entity(0, -0.65, 0, m);
	suelo->loadMesh("data/Shop/groundScale.obj");
	suelo->loadTexture("data/Shop/Shop-4-GroundTile.png");
	tienda->addEntity(suelo);
	
	m.setTranslation(0.000, 4.04, -3.34);
	Entity* shop = new Entity(0.000, 4.04, -3.34, m);
	shop->loadMesh("data/Shop/Shop-0-ShopBuilding_1.obj");
	shop->loadTexture("data/Shop/Shop-0-ShopBuilding_1.png");
	tienda->addEntity(shop);

	Vector3 playerInitPos = Vector3(0, 0.5, 0);
	initPlayer(playerInitPos, tienda);

	tienda->createZombies();

	initSky(tienda);
	
	initCamera(tienda);

	m.setTranslation(0, -0.6, 0);
	tienda->cesped = new Entity(0,-0.6,0, m);
	//tienda->cesped->mesh = new Mesh();
	//tienda->cesped->mesh->createPlane(2000);
	tienda->cesped->loadMesh("data/country/CountrySide-3-GroundTile1.obj");
	tienda->cesped->loadTexture("data/country/obj/CountrySide-3-GroundTile1.png");

	m.setTranslation(0, 0, 0);
	tienda->crossHair = new Entity(0, 0, 0, m);
	tienda->crossHair->mesh = new Mesh();
	tienda->crossHair->mesh->createPlane(20);
	tienda->crossHair->loadTexture("data/crosshair.png");

}

void Game::initPlayer(Vector3 pos, World* world) {
	Matrix44 m;
	m.setTranslation(pos.x, pos.y, pos.z);
	Player* player = (Player*) new Entity(pos, m);
	player->loadMesh("data/Zombie/Zed_1.obj");
	player->loadTexture("data/Zombie/Zed_1.png");
	player->setVel(2.0f);
	world->addPlayer(player);
}

void Game::initSky(World* world) {
	Matrix44 m;
	Vector3 playerPos = world->player->getPos();
	m.setTranslation(playerPos.x, playerPos.y, playerPos.z);
	world->sky = new Entity(playerPos, m);
	world->sky->loadMesh("data/cielo.ASE");
	world->sky->loadTexture("data/cielo.tga");
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