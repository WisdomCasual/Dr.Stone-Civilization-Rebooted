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
			if (mp_name == "")
				continue;
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

		RectangleShape tint, blackscreen;
		bool blacking_out = 0, change_map = 0;
		float x = 0, y = 0;
		float win_x = 0, win_y = 0, scale = 1, previous_scale = 1, transparency = 0, darkness = 0;
		bool admin = 0, new_map = 0, map_added = 0, del = 0, move = 0, selected = 0, destruct = 0, loadmap = 0, delete_map = 0, tint_fade = 1, back = 0;
		string moving, map_to_delete;
		Sprite worldmap, namebox, pin;
		Vector2f mouse_pos = { 0,0 }, clicked_on = { -1, -1 };
		Vector2u prev_win = { 0, 0 };
		string& selected_map;

		//private functions
		void fade_in();
		bool fade_out();
		bool black_out();
		void update_pins();
		void render_pins();

	public:
		//public functions;

		//constructors/destructors
		WorldMapState(string& selected_map, bool admin = false, bool tint_fade = true );
		~WorldMapState();


		//public functions:
		void save();
		void update();
		void render();
		void pollevent();
};