#pragma once
#include"State.h"
#include"TextBox.h"
#include "Global.h"
using namespace globalvar;
using namespace sf;
using namespace std;

struct NewSaveState :public State
{
private:
	//variables
	string test_str = "";
	TextBox txt_box;

public:
	//constructors/destructors:
	NewSaveState();
	~NewSaveState();
	//public functions:

	void update();
	void render();
	void pollevent();

};

