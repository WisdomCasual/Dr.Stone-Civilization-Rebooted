#pragma once
#include"State.h"
#include "Game.h"
#include"Global.h"
using namespace globalvar;

using namespace sf;
using namespace std;
struct DialogueState : public State {

private:
	string output_strings[6];
	const string y_string = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	dialogue* dialogues;
	Text output_text, speaker_text;
	Vector2f position = { 0, 0 };
	float scale = 1, delay = 0, text_y_bound = 0, text_x_bound = 0, text_x_offset;
	int dialogues_number = 1, char_idx = 0, dialogue_idx = 0, reminder_idx = 0, lines = 1, lim = 4, temp_idx = 0;
	bool typing = 1, empty_state = 0, skip = 0, test = 0, cursor = 0, add_idx = 1;
	Sprite box, pic;
	RectangleShape black_box;
	FloatRect box_bounds;
	

public:

	DialogueState(dialogue* dialogues, Vector2f pos = { 0.0, 0.0 }, float scale = 1, int dialogues_number = 1);

	~DialogueState();

	void setDialogue(dialogue* dialogues, int dialogues_number = 1);

	void setTexture(Texture& box_texture);

	void setFont(Font font);

	void setPosition(const Vector2f new_position);

	void setScale(const float new_scale);

	void render_text();

	void word_in_new_line();

	void write_text();

	void set_expression(short id);

	void commands();


	void update();
	void render();
	void pollevent();

};