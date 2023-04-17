#pragma once
#include"State.h"
#include"Game.h"
#include"TextBox.h"

#include"Global.h"
using namespace globalvar;

struct PasswordState : public State
{
private:
	//variables:
	Vector2f clicked_on = { -1, -1 };

	bool confirmed = 0;

	struct button {
		string txt;
		int x = 0, y = 0;
		bool& execute;
		int pressed = 0;
		bool hover = 0;
	} confirm = { "Confirm", 0 ,30 , confirmed };

	Sprite bg, back_arrow, tissue, buttontex;
	RectangleShape tint;
	Vector2f mouse_pos = { 0,0 };
	Vector2u prev_win = { 0, 0 };
	Text button_text;
	TextBox txt_box;
	string input_password;
	Color button_color;
	float x = 0, y = 0;
	float win_x = 0, win_y = 0, scale = 1, previous_scale = 1;
	bool button_pressed = 0, arrow_pressed = 0, destruct = 0, wrong_password = 0;


	//private functions:
	void update_arrow();
	void update_buttons();
	void render_buttons();

public:

	//constructors/destructors
	PasswordState();
	~PasswordState();

	//public functions:
	void update();
	void render();
	void pollevent();
};

