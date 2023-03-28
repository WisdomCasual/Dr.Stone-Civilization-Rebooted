#pragma once
#include"State.h"

class Background : public State
{
private:
	Sprite bg;
	float win_x = 0, win_y = 0; //<-- window dimentions
	float bg_x = 0, bg_y = 0;   //<-- image dimentions
	float scale = 0;

public:
	Background();
	~Background();
	void pollevent(Event, RenderWindow*);
	void update(float);
	void render(RenderWindow*);
};

