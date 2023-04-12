#pragma once
#include"State.h"
#include "CreativeMode.h"
#include "PauseState.h"


using namespace std;

struct MapBuilderState : public State
{
private:
	//variables:
	struct Tile {
		map <char, Vector3i> layer[2]; //Render_Priority [ back/front ] [layer] { x cords, y cords, texture sheet id }
		bool hitbox = 0, blocked = 0;
	};

	struct change {
		Vector2i start = { 0,0 }, end = { 0,0 };
		vector <Tile> tiles;
	};

	bool ctrl_pressed = 0, z_pressed = 0, y_pressed = 0, undid = 0, redone = 0;

	deque<change> changes;
	deque<change> undid_changes;

	bool picker = 1;
	bool Render_Priority = 0;
	string Render_in[2] = { "Back", "Front" };
	CreativeMode* tex_picker;
	tex_tile picked_tile = { 0,0 };
	int size_x, size_y;  //<-- map size
	int x = 0, y = 0;    //<-- location of upper left corner of the map
	float x_offset = 0, y_offset = 0; //<-- offset from upper left corner of the screen to upper left corner of the map
	int x_mid = 0, y_mid = 0;       //<-- index of the tile in the center of the screen
	int cam_speed = 150;
	bool active_grid = 1; //<-- grid active/inactive
	float scale = 10, global_scale = 1, text_scale = 1, x_scale = 1, y_scale = 1, scaling_speed;
	float dt = 0;
	int layer = 0;
	bool blocked = 0, hitbox = 0, drawn_selection = 0, drawn_map_selection = 0, selecting = 0;
	int brush_size = 1, rand_spray = 1;
	const int speed_list[5] = { 1, 2, 4, 8, 16 };
	bool layer_toggle[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
	Vector2f mouse_pos = { 0, 0 }, relative_mouse_pos = {0, 0}, hover_tile = { 0 , 0 };
	Vector2i selected_tile = { 0, 0 }, selection_start = { 0,0 }, selection_end = { 0,0 };
	Vector2u prev_win = { 1920, 1080 };
	Text info, layer_info;
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
	void undo(RenderWindow*);
	void mouse_cords(RenderWindow*);

public:
	//constructors/destructors:
	MapBuilderState(int a = 128, int b = 256, string map_name = "unnamed");
	~MapBuilderState();

	//public functions:
	void update(float, RenderWindow*, int*, map<int, State*>*);
	void render(RenderWindow*);
	void pollevent(Event, RenderWindow*, int*, map<int, State*>*);
};