#pragma once
#include"State.h"
#include "Game.h"
#include"Global.h"
using namespace globalvar;

using namespace sf;
using namespace std;
struct DialogueState : public State {

private:

	string output_strings[6], speaker_text;
	const string y_string = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	dialogue* dialogues;
	Text output_text;
	Vector2f position = { 0, 0 }, offset = { 0, 0 };
	Vector2u prev_win = { 0, 0 };
	RectangleShape tint;
	float x = 0, y = 0, transparency = 0, darkness = 0;
	float win_x = 0, win_y = 0, scale = -1, scale_const = 1, delay = 0, text_y_bound = 0, text_x_bound = 0, text_x_offset, text_y_offset;
	int dialogues_number = 1, char_idx = 0, dialogue_idx = 0, reminder_idx = 0, lines = 1, lim = 4, temp_idx = 0;
	bool typing = 1, empty_state = 0, skip = 0, test = 0, cursor = 0, add_idx = 1, close = 0;
	Sprite box, pic;
	RectangleShape black_box;
	FloatRect box_bounds;
	

private:
	void fade_in();
	bool fade_out();

public:

	DialogueState(dialogue* dialogues, Vector2f offset = { 0.0, 0.0 }, float scale = 1, int dialogues_number = 1);

	~DialogueState();

	void setDialogue(dialogue* dialogues, int dialogues_number = 1);

	void setTexture(Texture& box_texture);

	void setFont(Font font);

	void setPosition(const Vector2f new_position);

	void render_text();

	void word_in_new_line();

	void write_text();

	void set_expression(short id);

	void commands();


	void update();
	void render();
	void pollevent();

};