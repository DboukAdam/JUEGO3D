#include "game.h"

class Stage {
public:
	virtual void render() {};
	virtual void update(double seconds_elapsed) {};
};

class MenuStage : public Stage {
public:
	virtual void render();
	virtual void update(double seconds_elapsed);
};

class IntroStage : public Stage {
public:
	virtual void render();
	virtual void update(double seconds_elapsed);
};

class PlayStage : public Stage {
public:
	virtual void render();
	virtual void update(double seconds_elapsed);
};

class HomeStage : public PlayStage {
public:
	virtual void render();
	virtual void update(double seconds_elapsed);
};

class MarketStage : public PlayStage {
public:
	virtual void render();
	virtual void update(double seconds_elapsed);
};

class EndStage : public Stage {
public:
	virtual void render();
	virtual void update(double seconds_elapsed);
};
