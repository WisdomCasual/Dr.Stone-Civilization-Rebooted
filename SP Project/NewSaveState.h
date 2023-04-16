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
	Sprite tissue, back_arrow, panel;
	bool arrow_pressed = 0, destruct = 0;
	float scale = 1, win_x=0, win_y=0, x=1, y=1;
	Vector2f mouse_pos = { 0,0 }, clicked_on = { -1, -1 };
	Vector2u prev_win = { 0, 0 };

	//button
	bool confirmed=0;
	Color button_color;
	Text button_text;
	Sprite buttontex;
	bool button_pressed = 0;
	struct button {
		string txt;
		int x = 0, y = 0;
		bool& execute;
		int pressed = 0;
		bool hover = 0;
	} confirm = { "Confirm", 0 ,340 , confirmed };

	//character selection
	Sprite characters;
	Color char_color,og_color;
	bool selected[3] = {0,0,0};

	//private functions
	void update_arrow();

	void update_buttons();
	void render_buttons();

	void update_characters();
	void render_characters();
public:
	//constructors/destructors:
	NewSaveState();
	~NewSaveState();
	//public functions:
	void update();
	void render();
	void pollevent();

};

