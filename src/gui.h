#pragma once
#include "texture.h"

#define numImagesAtlas 3

enum atlasGuide {
	greenButton,
	greenButtonPressed,
	crosshair
};

class Gui{
public:
	Shader* shader;
	Texture* atlas;
	Vector4 atlasRanges[numImagesAtlas];

	Gui(Shader* shader, Texture* atlas);
	void initAtlas();
	void RenderIntroGui();
	void RenderCrosshair();
};

