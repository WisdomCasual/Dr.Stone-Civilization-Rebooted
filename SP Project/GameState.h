#pragma once
#include"State.h"

#include"Global.h"
using namespace globalvar;

struct GameState : public State
{
private:
	//variables:


public:
	//constructors/destructors
	GameState();
	~GameState();

	//public functions:
	void scaling();
	void update();
	void render();
	void pollevent();
};
