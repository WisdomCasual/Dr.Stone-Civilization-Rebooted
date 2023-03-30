#pragma once
#include"State.h"
#include "CreativeMode.h"
#include"Background.h"
#include"MainMenuState.h"

class SavesState :
	public State
{
private:
	//variables:
	struct save {
		int x, y, no;
		int progress = 0, character_id = 0;
		string name;
		bool empty = 1, hover = 0, del_hover = 0, pressed = 0, del_pressed = 0, del = 0;
	};


	Text text, del;
    float scale = 1;
	save saves[3] = { {-130,0,1}, {0,0,2}, {130,0,3} };


	Sprite save_bg, savesBG, charactertex, back_arrow, arrow;
	Text Delete[3];
	bool del_presse[3] = { 0,0,0 };
	

	RectangleShape tint;
	
	float x_win = 0, y_win = 0;
	float x = 0, y = 0;
	float pos_x = 0, pos_y = 0, dis = 0;
	bool arrow_pressed = 0;

	//private functions:
	void initial_saves();
	void update_saves(RenderWindow*);
	void update_arrow(RenderWindow*, int*, deque<State*>*);
	void render_saves(RenderWindow*);

	void draw_text(RenderWindow*, string, int, int, int);

public:
	//constructors/destructors:
	SavesState();
	~SavesState();
	//public functions:
	void update(float, RenderWindow*, int*, deque<State*>*);
	void render(RenderWindow*);
	void pollevent(Event, RenderWindow*);
};
