#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "entity.h"

#include <cmath>
#include "world.h"

//some globals
Mesh* mesh = NULL;
Texture* texture = NULL;
Shader* shader = NULL;
Animation* anim = NULL;
float angle = 0;
float mouse_speed = 100.0f;
FBO* fbo = NULL;
Zombie* zzz;

Entity* shop;
Entity* suelo;
Entity* zombie;
Game* Game::instance = NULL;
World* world;

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

	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective

	//world->initWorld();


	//load one texture without using the Texture Manager (Texture::Get would use the manager)
	texture = new Texture();
 	//texture->load("data/texture.tga");
	texture->load("data/modelos/ambulance.png");
	// example of loading Mesh from Mesh Manager
	//mesh = Mesh::Get("data/box.ASE");
	mesh = Mesh::Get("data/modelos/ambulance.obj");
	// example of shader loading using the shaders manager
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	Matrix44 m;
	Matrix44 modelZ; modelZ.setTranslation(0, 0, 0);
	m.rotate(angle * DEG2RAD, Vector3(0, 1, 0));
	suelo = new Entity(0, 0, 0, m);
	suelo->loadMesh("data/Shop/Shop-4-GroundTile.obj");
	suelo->loadTexture("data/Shop/Shop-4-GroundTile.png");

	//zombie = new Entity(0, 0, 0, modelZ);
	//zombie->loadMesh("data/Zombie/Zed_1.obj");
	//zombie->loadTexture("data/Zombie/Zed_1.png");
	
	shop = new Entity(0, 5, 0, m);
	m.translate(0.0f, 3.5f, 0.0f);
	shop->loadMesh("data/Shop/Shop-1-ShopBuilding_2.obj");
	shop->loadTexture("data/Shop/Shop-1-ShopBuilding_2.png");
	
	zzz = new Zombie();
	zzz->m = modelZ;
	zzz->pos.set(0.0f, 3.5f, 5.0f);
	zzz->loadMesh("data/Zombie/Zed_1.obj");
	zzz->loadTexture("data/Zombie/Zed_1.png");
	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse
}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	camera->enable();

	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   
	//create model matrix for cube
	Matrix44 m;
	m.rotate(angle*DEG2RAD, Vector3(0, 1, 0));

	if(shader)
	{
		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", Vector4(1,1,1,1));
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
		shader->setUniform("u_texture", texture, 0);
		shader->setUniform("u_model", m);
		shader->setUniform("u_time", time);

		//do the draw call
		//mesh->render( GL_TRIANGLES );

		shader->setUniform("u_texture", shop->texture, 0);
		shader->setUniform("u_model", shop->m);
		shop->mesh->render(GL_TRIANGLES);
		shader->setUniform("u_texture", zzz->texture, 0);
		shader->setUniform("u_model", zzz->m);
		zzz->mesh->render(GL_TRIANGLES);
		shader->setUniform("u_model", suelo->m);
		shader->setUniform("u_texture", suelo->texture, 0);
		suelo->mesh->render(GL_TRIANGLES);

		//disable shader
		shader->disable();
	}

	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	//example
	angle += (float)seconds_elapsed * 10.0f;

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked ) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f,-1.0f,0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector( Vector3(-1.0f,0.0f,0.0f)));
	}

	//async input to move the camera around
	if(Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f,-1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f,0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) zzz->pos.z +=	1.0f *speed;
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) zzz->pos.z -= 1.0f * speed;
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) zzz->pos.x += 1.0f * speed;
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) zzz->pos.x -= 1.0f * speed;

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();
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

