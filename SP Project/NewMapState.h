#pragma once
#include"State.h"
#include"Game.h"
#include"TextBox.h"
#include"WorldMapState.h"

#include"Global.h"
using namespace globalvar;

struct NewMapState : public State
{
private:
	//variables:
	Vector2f clicked_on = { -1, -1 };


	IntRect tipsright[5] = { {190,330,9,18} ,{190,348,9,18} ,{372,312,9,18}, { 190,294,9,18 }, {190,312,9,18} },
		tipsleft[5] = { {370,126,9,18} ,{370,90,9,18} ,{372,294,9,18},{ 372,330,9,18 }, {370,108,9,18} },
		mids[5] = { {338,449,18,18} ,{356,368,18,18} ,{356,431,18,18},{338,386,18,18}, {338,368,18,18} },
		arrow = { 303,486,22,21 },
		buttons[4] = { {290,139,93,94} ,{240,0,93,94} ,{290,49,93,94},{201,296,92,92} },
		checkMark = { 369,184,16,15 };

	short x_size = 128, y_size = 128;

	struct slider_info {
		float x = 0, y = 0;
		short int mx = 1, color = 0;
		short int* linker = nullptr;
		string name = "";
		char text_type = 0;     //0 = plain text, 1 = percentage (a%), 2 = custom (link string array)
		string* txt = nullptr;
		float tipx = 0;
		float midscale = 1;
		bool presssed = 0;

	}sliders[2] = { {-64, 0, 255, 2, &x_size, "X", 0}, {-64, 15, 255, 4, &y_size, "Y", 0} };

	bool confirmed = 0;

	struct button {
		string txt;
		int x = 0, y = 0;
		bool& execute;
		int pressed = 0;
		bool hover = 0;
	} confirm = {"Confirm", 0 ,112 , confirmed};

	Sprite bg, back_arrow, tip, tissue, buttontex;
	RectangleShape tint;
	Vector2f mouse_pos = { 0,0 };
	Vector2u prev_win = { 0, 0 };
	Text slider_text, button_text;
	TextBox txt_box;
	string map_name = "";
	float x = 0, y = 0;
	float win_x = 0, win_y = 0, scale = 1, previous_scale = 1, transparency = 0, darkness = 0;
	bool button_pressed = 0, arrow_pressed = 0, back = false;


	//private functions:
	void fade_in();
	bool fade_out();
	void update_arrow();
	void update_slider(slider_info*, int);
	void render_slider(int);
	void update_buttons();
	void render_buttons();
	void initialize_sliders();

public:

	//constructors/destructors
	NewMapState();
	~NewMapState();

	//public functions:
	void update();
	void render();
	void pollevent();
};