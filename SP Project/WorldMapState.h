#pragma once
#include"State.h"
#include"PauseState.h"

#include"Global.h"
using namespace globalvar;

struct WorldMapState : public State
{
	private:
		//variables:

		struct Pin {
			int x = 0, y = 0;
			string name = "Antarctica";
			bool pressed = 0;
		}test;


		RectangleShape tint;
		float x = 0, y = 0;
		float win_x = 0, win_y = 0, scale = 1, previous_scale = 1, text_size = 40;
		bool admin = 0;
		Sprite worldmap, namebox, pin;
		Vector2f mouse_pos = { 0,0 }, clicked_on = { -1, -1 };;
		Vector2u prev_win = { 0, 0 };

		//private functions
		void update_pins();
		void render_pins();


	public:
		//constructors/destructors
		WorldMapState(bool admin = 0);
		~WorldMapState();


		//public functions:
		void update();
		void render();
		void pollevent();
};

