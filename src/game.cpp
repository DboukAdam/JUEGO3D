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
	mouse_locked = false;

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

	tienda = new World();

	Matrix44 m;
	m.rotate(angle * DEG2RAD, Vector3(0, 1, 0));
	Entity* suelo = new Entity(0, 0, 0, m, shader);
	suelo->loadMesh("data/Shop/Shop-4-GroundTile.obj");
	suelo->loadTexture("data/Shop/Shop-4-GroundTile.png");

	tienda->addEntity(suelo);

	m.translate(0, 3.5, 1);
	Zombie* zombie = (Zombie*) new Entity(0, 0, 0, m, shader);
	zombie->loadMesh("data/Zombie/Zed_1.obj");
	zombie->loadTexture("data/Zombie/Zed_1.png");
	zombie->setVel(2.0f);
	tienda->addZombie(zombie);
	
	//create our camera
	camera = new Camera();
	
	camera->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
	Camera::current = camera;

	/*m.translate(0.0f, 3.5f, 0.0f);
	Entity* shop = new Entity(0, 5, 0, m);
	shop->loadMesh("data/Shop/Shop-1-ShopBuilding_2.obj");
	shop->loadTexture("data/Shop/Shop-1-ShopBuilding_2.png");

	tienda->addEntity(shop);*/
}

