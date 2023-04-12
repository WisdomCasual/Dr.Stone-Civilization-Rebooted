#pragma once
#include"State.h"

struct Background : public State
{
private:
	Sprite bg;
	float win_x = 0, win_y = 0; //<-- window dimentions
	float bg_x = 0, bg_y = 0;   //<-- image dimentions
	float scale = 0;

public:
	Background();
	~Background();
	void pollevent(Event, RenderWindow*, int*, map<int, State*>*);
	void update(float, RenderWindow*, int*, map<int, State*>*);
	void render(RenderWindow*);
};

