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
	bool arrow_pressed = 0;
	float scale = 1, win_x = 0, win_y = 0, x = 1, y = 1, transparency = 0, darkness = 0;
	Vector2f mouse_pos = { 0,0 }, clicked_on = { -1, -1 };
	Vector2u prev_win = { 0, 0 };
	int save_no;

	//button
	bool confirmed=0;
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
	char selected = 0;
	bool pressed = 0, back = 0;

	//private functions
	void fade_in();
	bool fade_out();

	void update_arrow();

	void update_buttons();
	void render_buttons();

	void update_characters();
	void render_characters();
	void add_save();
public:
	//constructors/destructors:
	NewSaveState(int);
	~NewSaveState();
	//public functions:
	void update();
	void render();
	void pollevent();

};

