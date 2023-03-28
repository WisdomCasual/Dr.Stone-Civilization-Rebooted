#pragma once
#include"State.h"

class Background : public State
{
private:
	Sprite bg;
	RenderWindow* window;
	float win_x = 0, win_y = 0;
	float bg_x = 0, bg_y = 0;
	float scale = 0;

public:
	Background();
	~Background();
	void pollevent(Event, RenderWindow*) = 0;
	void update(float) = 0;
	void render(RenderWindow*);
};

