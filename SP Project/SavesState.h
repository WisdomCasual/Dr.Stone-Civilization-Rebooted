#pragma once
#include"State.h"
#include "CreativeMode.h"
class SavesState :
    public State
{
private:
	//variables:
	float scale = 1;
	bool save_intersection[3] = {};
	Sprite save[3], arrow, savesBG;
	//private functions:
	void render_save(RenderWindow*,int,int,int,Font);
	void render_arrow(RenderWindow*);
	void render_savesBG(RenderWindow*,Font);

public:
	//constructors/destructors:
	SavesState();
	~SavesState();
	//public functions:
	void update(float);
	void render(RenderWindow*);
	void pollevent(Event, RenderWindow*);
};

