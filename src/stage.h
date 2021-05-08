#include "game.h"

class Stage {
public:
	virtual void render() {};
	virtual void update() {};
};

class IntroStage : public Stage {
public:
	virtual void render();
	virtual void update();
};

class PlayStage : public Stage {
public:
	virtual void render();
	virtual void update();
};

class EndStage : public Stage {
public:
	virtual void render();
	virtual void update();
};
