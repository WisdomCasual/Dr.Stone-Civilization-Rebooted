#pragma once
#include"State.h"

class GameState : public State
{
private:
	//variables:


public:
	//constructors/destructors
	GameState();
	~GameState();

	//public functions:
	void update(float, RenderWindow*, int*, deque<State*>*);
	void render();
	void pollevent(Event, RenderWindow*);
};
