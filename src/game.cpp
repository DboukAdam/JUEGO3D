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
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
Animation* anim = NULL;
float angle = 0;

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

	//example
	angle += (float)seconds_elapsed * 10.0f;

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
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	tienda = new World(shader);

	Matrix44 m;
	m.rotate(angle * DEG2RAD, Vector3(0, 1, 0));
	m.setTranslation(9.99, 0.000, -7.08);
	Entity* suelo = new Entity(9.99, 0.000, -7.08, m);
	suelo->loadMesh("data/Shop/Shop-4-GroundTile.obj");
	suelo->loadTexture("data/Shop/Shop-4-GroundTile.png");
	tienda->addEntity(suelo);
	m.setTranslation(-10.01, 0.000, -7.07);
	Entity* suelo2 = new Entity(-10.01, 0.000, -7.07, m);
	suelo2->loadMesh("data/Shop/Shop-4-GroundTile.obj");
	suelo2->loadTexture("data/Shop/Shop-4-GroundTile.png");
	tienda->addEntity(suelo2);
	m.setTranslation(0.000, 4.04, -3.34);
	Entity* shop = new Entity(0.000, 4.04, -3.34, m);
	shop->loadMesh("data/Shop/Shop-0-ShopBuilding_1.obj");
	shop->loadTexture("data/Shop/Shop-0-ShopBuilding_1.png");
	tienda->addEntity(shop);

	m.setTranslation(0, 0.5, 0);
	Player* player = (Player*) new Entity(0, 0.5, 0, m);
	player->loadMesh("data/Zombie/Zed_1.obj");
	player->loadTexture("data/Zombie/Zed_1.png");
	player->setVel(2.0f);
	tienda->addPlayer(player);

	tienda->createZombies();
	
	Entity* sky = new Entity(0, 0, 0, m);
	sky->loadMesh("data/Ambiente/cielo.ASE");
	sky->loadTexture("data/Ambiente/cielo.tga");
	tienda->addEntity(sky);

	//create our camera
	camera = new Camera();
	Camera::current = camera;
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
	Vector3 eye = player->m * Vector3(0.f, 2.f, 0.f);
	Vector3 center = player->m * Vector3(0.f, 5.f, -1.f);
	Vector3 up = player->m.rotateVector(Vector3(0.f, 1.f, 0.f));
	camera->lookAt(eye, center, up);

	/*m.translate(0.0f, 3.5f, 0.0f);
	Entity* shop = new Entity(0, 5, 0, m);
	shop->loadMesh("data/Shop/Shop-1-ShopBuilding_2.obj");
	shop->loadTexture("data/Shop/Shop-1-ShopBuilding_2.png");

	tienda->addEntity(shop);*/
}