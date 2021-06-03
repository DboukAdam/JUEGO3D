/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "world.h"
#include "stage.h"
#include "gui.h"

class Game
{
public:
	static Game* instance;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;

	//some globals
	long frame;
    float time;
	float elapsed_time;
	int fps;
	bool must_exit;

	//some vars
	Camera* camera; //our global camera
	bool mouse_locked; //tells if the mouse is locked (not seen)

	//stages
	Stage* currentStage;
	IntroStage* intro = new IntroStage();
	PlayStage* play = new PlayStage();
	EndStage* end = new EndStage();
	EditorStage* editor = new EditorStage();

	//Our stuff
	World* tienda;
	World* currentWorld;
	Gui* gui;
	float mouse_speed = 10.0f;
	

	Game( int window_width, int window_height, SDL_Window* window );

	//main functions
	void render( void );
	void update( double dt );

	//events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);

	void initWorldTienda();
	void initPlayer(Vector3 initPos, World* world);
	void initSky(World* world);
	void initCamera(World* world);
};


#endif 