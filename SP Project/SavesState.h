#pragma once
#include"State.h"
#include"Background.h"
#include"MainMenuState.h"

#include"Global.h"
using namespace globalvar;

struct SavesState :
	public State
{
private:
	//variables:
	struct save { // stores save 
		int x, y, no;
		int progress = 0, character_id = 0, health = 100;
		string name, current_map;
		bool empty = 1, hover = 0, del_hover = 0, pressed = 0, del_pressed = 0, del = 0;
		Vector2f player_pos = { 0,0 };
	};

	Text del;
    float scale = 1, transparency = 0, darkness = 0;
	save saves[3] = { {-130,0,1}, {0,0,2}, {130,0,3} };
	Vector2f clicked_on = { -1, -1 };

	Sprite save_bg, savesBG, charactertex, arrow;
	
	RectangleShape tint, blackscreen;
	
	float x_win = 0, y_win = 0;
	float x = 0, y = 0;
	float pos_x = 0, pos_y = 0, dis = 0;
	bool arrow_pressed = 0, destruct = 0, del_save = 0, back = 0, blacking_out = 0;
	int del_save_no = 0, selected_save = 0;

	//private functions:
	void fade_in();
	bool fade_out();
	bool black_out();
	void initial_saves();
	void update_saves();
	void update_arrow();
	void render_saves();

public:
	//constructors/destructors:
	SavesState();
	~SavesState();
	//public functions:
	void update();
	void render();
	void pollevent();
};
