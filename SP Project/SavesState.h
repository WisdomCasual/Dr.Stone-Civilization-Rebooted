#pragma once
#include"State.h"
#include "CreativeMode.h"
class SavesState :
	public State
{
private:
	//variables:
	struct save {
		int x, y, no;
		bool& excute;
		string name;
		bool empty = 1, hover = 0, del_hover = 0, pressed = 0, del_pressed = 0, del = 0;
	};


	Text text, del;
    float scale = 1;
	bool save1 = 0, save2 = 0, save3 = 0;
	save saves[3] = { {-130,0,1,save1}, {0,0,2,save2}, {130,0,3,save3} };


	bool save_empty[3] = { 0,0,0 };
	Sprite save_bg, save[3], arrow, savesBG;
	Text Delete[3];
	bool del_presse[3] = { 0,0,0 };
	

	RectangleShape tint;
	
	float x_win = 0, y_win = 0;
	float x = 0, y = 0;
	float pos_x = 0, pos_y = 0, dis = 0;

	//private functions:
	void update_saves(RenderWindow*);
	void render_saves(RenderWindow*);



	void render_save(RenderWindow*, int, int, int, Font);
	void render_arrow(RenderWindow*);
	void render_savesBG(RenderWindow*, Font);
	void update_save(RenderWindow*, int);
	void update_arrow(RenderWindow*);
	void draw_text(RenderWindow*, string, int, int, int);
	void draw_level(RenderWindow*, string, int, int, int, Font);
public:
	//constructors/destructors:
	SavesState();
	~SavesState();
	//public functions:
	void update(float, RenderWindow*, int*, deque<State*>*);
	void render(RenderWindow*);
	void pollevent(Event, RenderWindow*);
};
