#pragma once
#include"State.h"
#include"TextBox.h"
#include"Game.h"

#include "Global.h"
using namespace globalvar;
using namespace sf;
using namespace std;

struct NewSaveState :public State
{
private:
	//variables
	string test_str = "";
	RectangleShape tint;
	TextBox txt_box;
	RectangleShape black_box;
	Sprite tissue, back_arrow;
	bool arrow_pressed = 0, destruct = 0;
	float scale = 1, win_x=0, win_y=0, x=1, y=1;
	Vector2f mouse_pos = { 0,0 }, clicked_on = { -1, -1 };
	Vector2u prev_win = { 0, 0 };


	//private functions
	void update_arrow();
public:
	//constructors/destructors:
	NewSaveState();
	~NewSaveState();
	//public functions:
	void update();
	void render();
	void pollevent();

};

