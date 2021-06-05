#pragma once
#include "texture.h"
#include "mesh.h"

#define numImagesAtlas 3
#define numIntroButtons 4

enum atlasGuide {
	greenButton,
	greenButtonPressed,
	crosshair
};
class Button {
public:
	Vector2 pos;
	Mesh mesh;
	Vector4 range;
	float width;
	float height;
	Button(Vector2 pos, Vector4 range, float width, float height) {
		this->pos = pos;
		this->range = range;
		this->width = width;
		this->height = height;
		mesh.createQuad(pos.x, pos.y, width, height, false);
	};
};

class Gui{
public:
	Shader* shader;
	Texture* atlas;
	Vector4 atlasRanges[numImagesAtlas];
	Button* introButtons[numIntroButtons];

	Gui(Shader* shader, Texture* atlas);
	void initAtlas();
	void initIntroButtons();
	void RenderIntroGui();
	void RenderCrosshair();
};
