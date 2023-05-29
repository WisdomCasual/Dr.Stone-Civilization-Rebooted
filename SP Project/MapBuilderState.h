#pragma once
#include"State.h"
#include "CreativeMode.h"
#include "PauseState.h"

#include"Global.h"

#define spread_chances_num 11
using namespace globalvar;

using namespace std;

struct MapBuilderState : public State
{
private:
	//variables:

	struct Tile {
		map <char, Vector3i> layer; // [layer] { x cords, y cords, texture sheet id }
		Vector3i last_vis{ -1,-1,-1 };
	};

	struct change {
		Vector2i start = { 0,0 }, end = { 0,0 };
		Tile* tiles = nullptr;
		int size = 0;
		~change(){
			if (tiles != nullptr)
				delete[] tiles;
		}
	};


	struct nod {
		change itm;
		struct nod* link, * back_link;
	};
	struct special_stack {
	private:
		nod* top, * back;
		
	public:
		int size = 0;
		/* insert function */
		special_stack() {
			top = NULL;
			back = NULL;
		}

		~special_stack() {
			clear();
		}

		void add(change inputed) {
			nod* tmp;
			
			tmp = new nod;

			tmp->itm = inputed;

			tmp->link = top;

			top = tmp;
			top->back_link = NULL;

			if (top->link != NULL) {

				top->link->back_link = top;

				if (top->link->link == NULL)
					back = top->link;

			}
			size++;
		}

		/* empty function */
		bool empty() {
			if (top == NULL)
				return 1;
			else return 0;
		}

		/* delet top function */
		void pop_front() {
			if (back != NULL && back->back_link != NULL) {
				nod* tmp;
				tmp = back->back_link;
				delete back;

				back = tmp;

				back->link = NULL;

				size--;
			}
			else { delete top;  top = NULL; size = 0; }

		}

		/* clear function */
		void clear() {
			while (!empty())
				pop_front();
			size = 0;
		}

		change* atBack() {
			return &top->itm;
		}
		void pop_back() {
			if (top->link != NULL) {
				nod* tmp = top->link;

				delete top;

				top = tmp;

				top->back_link = NULL;
			}
			else {
				delete top;
				top = NULL;
			}
			size--;
		}

		change* front() {
			if(top->link == NULL)
				return &top->itm;
			return &back->itm;
		}
	}changes, undid_changes;

	bool ctrl_pressed = 0, z_pressed = 0, y_pressed = 0, undid = 0, redone = 0, display_text = 1;

	string map_name = "default";
	bool picker = 1;
	CreativeMode* tex_picker;
	Sprite tile;
	tex_tile picked_tile = { 0,0 };
	int size_x, size_y;  //<-- map size
	int x = 0, y = 0;    //<-- location of upper left corner of the map
	float x_offset = 0, y_offset = 0; //<-- offset from upper left corner of the screen to upper left corner of the map
	int x_mid = 0, y_mid = 0;       //<-- index of the tile in the center of the screen
	int cam_speed = 150;
	bool active_grid = 1; //<-- grid active/inactive
	float scale = 10, global_scale = 1, text_scale = 1, x_scale = 1, y_scale = 1, scaling_speed;
	int layer = 0;
	bool blocked = 0, hitbox = 0, destroyable = 0, opaque = 0, view_layers = 0, drawn_selection = 0, drawn_map_selection = 0, selecting = 0, save_click = 1, loadmap = 0;
	int brush_size = 1, rand_spray = 1, spread_chance = 3;
	const int speed_list[5] = { 1, 2, 4, 8, 16 };
	bool layer_toggle[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
	Vector2f mouse_pos = { 0, 0 }, relative_mouse_pos = {0, 0}, hover_tile = { 0 , 0 };
	Vector2i selected_tile = { 0, 0 }, selection_start = { 0,0 }, selection_end = { 0,0 }, line_start = { 0, 0 }, point_on_line = { 0,0 };
	short active_highlight = 0;
	float slope = 0;
	bool click = 0;
	int line_length = 1, c = 0, base = 0, *dep = nullptr, *indep = nullptr, dx, dy;
	Vector2u prev_win = { 0, 0 };
	Text info, layer_info;
	Tile tiles[260][260];    //the level map
	RectangleShape hover_rect, highlight_rect, select_rect;
	Color highlight_color;
	int wdth = 0, hght = 0, start_x = 0, start_y = 0;
	const int spread_chances[spread_chances_num] = {2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000 };

	//private functions:
	void grid(int, int);
	void render_tiles(int, int, int);
	void update_info_text();
	void dash_cam();
	void hover();
	void selection();
	void draw_tools();
	void erase_tools();
	void undo();
	void mouse_cords();
	void create_mini_map();
	void save_map();
	void load_map();

public:
	//constructors/destructors:
	MapBuilderState(string map_name = "default", int a = 128, int b = 128);
	~MapBuilderState();

	//public functions:
	void update();
	void render();
	void pollevent();
};