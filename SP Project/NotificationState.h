#pragma once
#include "State.h"
#include"Game.h"
#include"Global.h"
using namespace globalvar;
struct NotificationState :public State
{
private:
	//variables:
	int x = 0, y = 0,text_x=0,text_y=0;
	float win_x = 0, win_y = 0, scale = 1;
	Vector2u prev_win = { 0,0 };
	Vector2f velocity = { 0,-4 * dt };
	float g = .5;
	Sprite notification_BG;
	//private functions;
	void render_strings();
	void update_velocity();

public:
	//constructors/destructors:
	NotificationState();
	~NotificationState();

	//public functions:
	void pollevent();
	void update();
	void render();
};


