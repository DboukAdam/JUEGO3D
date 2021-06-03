#pragma once
#include "texture.h"

class Gui{
public:
	Shader* shader;
	Texture* atlas;

	Gui(Shader* shader, Texture* atlas);
	void Render();
};

