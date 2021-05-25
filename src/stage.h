#include "game.h"

class Stage {
public:
	virtual void render(World* world) {};
	virtual void update(double seconds_elapsed, World* world) {};
};

class IntroStage : public Stage {
public:
	virtual void render(World* world);
	virtual void update(double seconds_elapsed, World* world);
};

class PlayStage : public Stage {
public:
	void addObjectEditor(Mesh* mesh, Texture* texture);
	void selectEntityEditor();
	virtual void render(World* world);
	virtual void update(double seconds_elapsed, World* world);
};

class EndStage : public Stage {
public:
	virtual void render(World* world) {};
	virtual void update(double seconds_elapsed, World* world) {};
};
