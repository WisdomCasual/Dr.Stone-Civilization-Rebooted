#pragma once
#include"State.h"
#include"Game.h"

#include"Global.h"
using namespace globalvar;

struct GameState : public State
{
private:
	//variables:
	Vector2f clicked_on = { -1, -1 };


public:
	//constructors/destructors
	GameState();
	~GameState();

	//public functions:
	void update();
	void render();
	void pollevent();
};
