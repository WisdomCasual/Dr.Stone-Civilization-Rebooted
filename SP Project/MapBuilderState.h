#pragma once
#include"State.h"
#include "CreativeMode.h"

using namespace std;

class MapBuilderState : public State
{
private:
	//variables:
	struct Tile {
		Vector2i layers[3]{ {21,49}, {5,6}, {5,6} }; //<-- cords of texture in sprite sheet
		bool blocked = 0, hitbox = 0; char texture_id = 0;
	};
	struct tex_props {
		bool blocked = 0, hitbox = 0; int layer = 0;
	};

	bool picker = 1;
	CreativeMode* tex_picker;
	Vector2i picked_tile = { 0,0 };
	int size_x, size_y;  //<-- map size
	int x = 0, y = 0;    //<-- location of upper left corner of the map
	int x_offset = 0, y_offset = 0; //<-- offset from upper left corner of the screen to upper left corner of the map
	int x_mid = 0, y_mid = 0;       //<-- index of the tile in the center of the screen
	int cam_speed = 150;
	bool active_grid = 1; //<-- grid active/inactive
	float scale = 10;
	float dt = 0;
	Tile tiles[256][256];    //the level map
	tex_props props[96][96];

	//private functions:
	void grid(RenderWindow*, int, int);
	void render_tiles(string, RenderWindow*, int, int, int);

public:
	//constructors/destructors:
	MapBuilderState(int a = 64, int b = 64);
	~MapBuilderState();

	//public functions:
	void update(float);
	void render(RenderWindow*);
	void pollevent(Event, RenderWindow*);
};

