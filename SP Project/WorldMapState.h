#pragma once
#include"State.h"
#include"PauseState.h"

#include"Global.h"
using namespace globalvar;

struct Pin {
			int x = 0, y = 0, x_size = 128, y_size = 128;
			float pin_scale = 2, namebox_scale = 1, text_size = 40;
			bool pressed = 0, hover = 0;
		};
inline map<string, Pin> pins;

inline void load_maps()
{
	pins.clear();

	ifstream ifs("Maps/maps.ini");
	if (ifs.is_open()) {
		string mp_name; int a, b, c, d;
		while (!ifs.eof()) {
			getline(ifs, mp_name, '|');
			ifs >> a >> b >> c >> d;
			ifs.ignore();
			pins.insert({ mp_name , {a,b,c,d} });
		}
	}
	ifs.close();
}

struct WorldMapState : public State
{
	private:
		//variables:

		RectangleShape tint;
		float x = 0, y = 0;
		float win_x = 0, win_y = 0, scale = 1, previous_scale = 1;
		bool admin = 0, new_map = 0, map_added = 0, del = 0, move = 0, selected = 0, destruct = 0, loadmap = 0, delete_map = 0;
		string moving, map_to_delete;
		Sprite worldmap, namebox, pin;
		Vector2f mouse_pos = { 0,0 }, clicked_on = { -1, -1 };
		Vector2u prev_win = { 0, 0 };

		//private functions
		void update_pins();
		void render_pins();
		void save_maps();
		


	public:
		//public functions;

		//constructors/destructors
		WorldMapState(bool admin = 0);
		~WorldMapState();


		//public functions:
		void update();
		void render();
		void pollevent();
};