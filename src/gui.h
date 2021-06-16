#pragma once
#include "texture.h"
#include "mesh.h"

#define numImagesAtlas 8
#define numIntroButtons 4
#define numPauseButtons 3
#define numSaves 5

enum atlasGuide {
	greenButton,
	greenButtonPressed,
	crosshair,
	fleshButton,
	fleshButtonPressed,
	turnPageRight,
	turnPageLeft,
	brownButton
};
class Button {
public:
	Vector2 pos;
	Mesh mesh;
	Vector4 range;
	float width;
	float height;
	Button(Vector2 pos, Vector4 range, float width, float height, bool flipuvs) {
		this->pos = pos;
		this->range = range;
		this->width = width;
		this->height = height;
		mesh.createQuad(pos.x, pos.y, width, height, flipuvs);
	};
};

class Gui{
public:
	Shader* shader;
	Texture* atlas;
	Vector4 atlasRanges[numImagesAtlas];
	Button* introButtons[numIntroButtons];
	Button* pauseButtons[numPauseButtons];
	Button* worldSavesButtons[numSaves];
	Button* turnPageRightButton;
	Button* turnPageLeftButton;

	int worldPage;
	std::vector<std::string> entries;

	Gui(Shader* shader, Texture* atlas);
	//Inits
	void initAtlas();
	void initIntroButtons();
	void initPauseButtons();
	void initWorldButtons();
	void initTurnPageButtons();
	//Renders
	void RenderIntroGui();
	void RenderWorldsGui();
	void RenderCrosshair();
	void RenderPauseMenu();
	//ButtonPressed
	void introButtonPressed(Vector2 pos);
	int worldButtonPressed(Vector2 pos);
	void changePageButtonPressed(Vector2 pos);
	void pauseButtonPressed(Vector2 pos);
};
