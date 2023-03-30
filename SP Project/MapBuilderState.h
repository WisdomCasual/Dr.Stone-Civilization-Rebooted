#pragma once
#include"State.h"
#include "CreativeMode.h"

using namespace std;

class MapBuilderState : public State
{
private:
	//variables:
	struct Tile {
		Vector2i layers[4]{ {21,49}, {5,6}, {5,6}, {5,6} }; //<-- cords of texture in sprite sheet
		bool blocked = 0, hitbox = 0; char texture_id[4] = {1, 1, 1, 1};
	};


	bool picker = 1;
	CreativeMode* tex_picker;
	tex_tile picked_tile = { 0,0 };
	int size_x, size_y;  //<-- map size
	int x = 0, y = 0;    //<-- location of upper left corner of the map
	int x_offset = 0, y_offset = 0; //<-- offset from upper left corner of the screen to upper left corner of the map
	int x_mid = 0, y_mid = 0;       //<-- index of the tile in the center of the screen
	int cam_speed = 150;
	bool active_grid = 1; //<-- grid active/inactive
	float scale = 10;
	float dt = 0;
	int layer = 0;
	bool blocked = 0, hitbox = 0, drawn_selection = 0, drawn_map_selection = 0, selecting = 0;
	int brush_size = 1;
	Vector2f mouse_pos = { 0, 0 }, relative_mouse_pos = {0, 0}, hover_tile = { 0 , 0 };
	Vector2i selected_tile = { 0, 0 }, selection_start = { 0,0 }, selection_end = { 0,0 };;
	Text info;
	Tile tiles[256][256];    //the level map
	RectangleShape hover_rect, hitbox_rect, select_rect;
	int wdth = 0, hght = 0, start_x = 0, start_y = 0;

	//private functions:
	void grid(RenderWindow*, int, int);
	void render_tiles(RenderWindow*, int, int, int);
	void update_info_text();
	void dash_cam();
	void hover();
	void selection(RenderWindow*);
	void draw_tools(RenderWindow*);
	void erase_tools(RenderWindow*);
	void mouse_cords(RenderWindow*);

public:
	//constructors/destructors:
	MapBuilderState(int a = 128, int b = 256);
	~MapBuilderState();

	//public functions:
	void update(float, RenderWindow*, int*, deque<State*>*);
	void render(RenderWindow*);
	void pollevent(Event, RenderWindow*);
};