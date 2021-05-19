#include "utils.h"
#include "stage.h"
#include "mesh.h"
#include "texture.h"
#include "animation.h"
#include "entity.h"
#include "shader.h"

class World {

public:

	World();
	void initWorld();

	Stage* current;
	IntroStage* intro = new IntroStage();
	HomeStage* home = new HomeStage();
	MarketStage* market = new MarketStage();
	MenuStage* menu = new MenuStage();
	EndStage* end = new EndStage();
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Animation* anim;
	FBO* fbo;
	Zombie* zzz;
	Entity* shop;
	Entity* suelo;
};
