#pragma once
#include "texture.h"
#include "mesh.h"

#define numImagesAtlas 5
#define numIntroButtons 4
#define numPauseButtons 3

enum atlasGuide {
	greenButton,
	greenButtonPressed,
	crosshair,
	fleshButton,
	fleshButtonPressed,
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

	Gui(Shader* shader, Texture* atlas);
	void initAtlas();
	void initIntroButtons();
	void initPauseButtons();
	void RenderIntroGui();
	void RenderCrosshair();
	void RenderPauseMenu();
	void introButtonPressed(Vector2 pos);
	void pauseButtonPressed(Vector2 pos);
};
