#pragma once
#include"State.h"
#include "CreativeMode.h"
class SavesState :
    public State
{
private:
	//variables:
	float scale = 1;
	bool save_empty[3] = { 0,0,0 };
	Sprite save[3], arrow, savesBG;
	 Text Delete [3];
	 bool del_presse[3] = { 0,0,0 };
	 int x_win, y_win;
	 float pos_x, pos_y, dis;
	//private functions:
	void render_save(RenderWindow*,int,int,int,Font);
	void render_arrow(RenderWindow*);
	void render_savesBG(RenderWindow*,Font);
	void update_save(RenderWindow*, int,int,int);
	void update_arrow(RenderWindow*);
	void draw_text(RenderWindow* ,string, int, int, int,Font);
	void draw_level(RenderWindow*, string, int, int, int, Font);
public:
	//constructors/destructors:
	SavesState();
	~SavesState();
	//public functions:
	void update(float);
	void render(RenderWindow*);
	void pollevent(Event, RenderWindow*);
};

