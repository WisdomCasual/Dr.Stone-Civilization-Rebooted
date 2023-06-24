#pragma once
#include "State.h"
#include"Game.h"

#include"Global.h"
using namespace globalvar;

struct ConfirmationState : public State
{
	private:
		//variables:
		int x = 0, y = 0, lines = 0;
		float win_x = 0, win_y = 0, scale = 1, transparency = 0, darkness = 0;
		bool yes = 0, no = 0, &function;
		Vector2f mouse_pos = { 0,0 };
		Vector2u prev_win = { 0,0 };
		Sprite buttontex, BG, tex_bg;
		RectangleShape tint;
		string text_strings[10];
		Vector2f clicked_on = { -1, -1 };
		Text button_text;

		struct button {
			string txt;
			int x = 0, y = 0;
			bool& execute;
			int pressed = 0;
			bool hover = 0;
		} buttons[2] = { { "NO",-45,90, no }, { "YES",45,90, yes } };

		//private functions;
		void fade_in();
		bool fade_out();
		void update_buttons();
		void render_buttons();
		void render_strings();

	public:
		//constructors/destructors
		ConfirmationState(string[], int line, bool&);
		~ConfirmationState();

		//public functions:
		void update();
		void render();
		void pollevent();

};
