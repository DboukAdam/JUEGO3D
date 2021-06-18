#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"

#include <cmath>
#include "audio.h"

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
	
	//Worlds
	initWorld();
	currentWorld = editorWorld;
	currentStage = intro;

	//GUI
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/gui.fs");
	Texture* atlas = Texture::Get("data/atlas.png");
	gui = new Gui(shader, atlas);

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

	//Audio
	//Audio* audio = Audio::Get("data/Audio/musicbox-silent-night.wav");
	if (BASS_Init == false) {
		std::cout << "AUDIO ERROR: tarjeta de sonido" << std::endl;
	}
	Audio* audio = new Audio();
	audio->load("data/Audio/musicbox-silent-night.wav");
	audio->play(1);
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
		if (currentStage == selectWorld) {
			gui->changePageButtonPressed(Vector2(Input::mouse_position.x, Input::mouse_position.y));
			int worldPos = gui->worldButtonPressed(Vector2(Input::mouse_position.x, Input::mouse_position.y));
			if (worldPos >= 0) {
				std::string filename = gui->entries[worldPos + (gui->worldPage * 5)];
				initWorld(filename);
				setPlayStage();
			}
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

void Game::initWorld(std::string filename){
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Matrix44 m;
	m.setTranslation(0, 0, 0);
	//Decorations
	Entity* caja = new Entity(0, 0, 0, m);
	caja->loadMesh("data/classic/1.Decoration/Boxes/caja.obj");
	caja->loadTexture("data/classic/1.Decoration/Boxes/Materials/box1.png");
	//Structures
	Entity* marco = new Entity(0, 0, 0, m);
	marco->loadMesh("data/classic/2.Structure/Doors/marco_grande.obj");
	marco->loadTexture("data/classic/2.Structure/Doors/Materials/cabinet.png");
	//Player
	Vector3 playerInitPos = Vector3(0, 0.5, 0);
	Mesh* mesh = Mesh::Get("data/players/man.obj");
	Texture* text = Texture::Get("data/players/man.png");
	//Sky
	Mesh* meshCielo = Mesh::Get("data/Ambiente/cielo.ASE");
	Texture* textCielo = Texture::Get("data/Ambiente/cielo.tga");
	//Ground
	text = Texture::Get("data/classic/2.Structure/Ground/Materials/Rocks_05.png");
	//Camera
	camera = new Camera();
	

	if (filename == "") {
		editorWorld = new World(shader);
		editorWorld->addDecoration(caja);
		editorWorld->addStructure(marco);
		editorWorld->initPlayer(playerInitPos, mesh, text);
		editorWorld->initSky(meshCielo, textCielo);
		editorWorld->initGround(text);
		editorWorld->initCamera(camera);
	}
	else {
		World* world = new World(shader);
		world->loadWorldInfo(filename);
		world->addDecoration(caja);
		world->addStructure(marco);
		world->initPlayer(playerInitPos, mesh, text);
		world->initSky(meshCielo, textCielo);
		world->initGround(text);
		world->initCamera(camera);
		currentWorld = world;
	}
}


//STAGES SETTINGS 
void Game::setIntroStage(){
	currentStage = intro;
	mouse_locked = false;
	SDL_ShowCursor(!mouse_locked);
}

void Game::setSelectWorldStage() {
	currentStage = selectWorld;
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
//:::::::::::::::::::::::::::::::::::::::::::::::::::::