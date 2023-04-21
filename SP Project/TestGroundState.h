#pragma once
#include"State.h"
#include "PauseState.h"

#include"Global.h"

using namespace globalvar;

using namespace std;

struct TestGroundState : public State
{
private:

	dialogue test[1] = { {"Meow", "Hello Comrade, my name is Griti, and I love eating souls so much!!!"} };
	

public:
	//constructors/destructors:
	TestGroundState();
	~TestGroundState();

	//public functions:
	void update();
	void render();
	void pollevent();
};