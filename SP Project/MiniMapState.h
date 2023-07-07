#pragma once
#include "State.h"
#include "game.h"

#include"Global.h"
using namespace globalvar;


struct MiniMapState
	: State
{
private:
	Sprite minimap;
	float x_pic = 0, y_pic = 0, map_scale = 1, scale = 1;
	float win_x = 0, win_y = 0, x = 0, y = 0, transparency = 0, darkness = 0;
	RectangleShape tint;
	CircleShape player_pointer, waypoint_pointer, quest_pointer;
	Vector2i mouse_pos = { 0,0 };
	Vector2u prev_win = { 0, 0 };
	bool exit = 0;
	Vector2f clicked_on = { -1, -1 }, player_position = { -1, -1 }, &waypoint, quest_location;

	// private functions
	void fade_in();
	bool fade_out();

public:
	MiniMapState(Texture&, Vector2f, Vector2f&, Vector2f&);
	~MiniMapState();
	void update();
	void pollevent();
	void render();

};

