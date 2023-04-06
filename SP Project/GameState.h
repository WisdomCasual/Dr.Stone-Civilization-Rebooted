#pragma once
#include"State.h"

struct GameState : public State
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
	void pollevent(Event, RenderWindow*, int*, deque<State*>*);
};
