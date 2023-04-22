#pragma once
#include"State.h"
#include"Game.h"

#include"Global.h"
using namespace globalvar;

struct GameState : public State
{
private:
	//variables:
	Vector2f clicked_on = { -1, -1 };

	struct render_tile {
		Vector3i* layer[2];
		short size[2]{}, tile_props = 0;
	};
	render_tile** static_map;

	int size_x = 0, size_y = 0;  //<-- map size
	int x = 0, y = 0;    //<-- location of upper left corner of the map
	float x_offset = 0, y_offset = 0; //<-- offset from upper left corner of the screen to upper left corner of the map
	float map_x = 0, map_y = 0;
	float scale = 1, x_scale = 1, y_scale = 1, win_x = 0, win_y = 0;
	Vector2u prev_win = { 0, 0 };
	Sprite tile;


	//private functions:
	void load_map(string);
	void deload_map();
	void initial_game();
	void render_map(int);

public:
	//constructors/destructors
	GameState();
	~GameState();
	
	//public functions:
	void update();
	void render();
	void pollevent();
};
