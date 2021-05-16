#include "utils.h"
#include "stage.h"

class World {

	Stage* current;
	IntroStage* intro = new IntroStage();
	HomeStage* home = new HomeStage();
	MarketStage* market = new MarketStage();
	MenuStage* menu = new MenuStage();
	EndStage* end = new EndStage();


public:

	World();
	void initWorld();
	int width;
	int height;
};
