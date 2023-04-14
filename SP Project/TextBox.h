#pragma once
#include"State.h"

#include"Global.h"
using namespace globalvar;

using namespace sf;
using namespace std;
struct TextBox {

private:
	string* target_string = nullptr;
	int character_limit = 16;
	Vector2f position = { 0, 0 };
	float scale = 1, bound_y = 0, delay = 0;
	string input_string = "", placeholder = "", clipboard = "";
	bool isActive = 0, selected = 0, cursor = 0;
	Text inputted_text, placeholder_text;
	Sprite box;
	Vector2f clicked_on = { 0, 0 };



public:

	TextBox();


	void setTargetString(string& target_string);

	void setTexture(Texture& box_texture);

	void setFont(Font font);

	void setPosition(const Vector2f new_position);

	void setScale(const float new_scale);

	void setPlaceholderText(const string placeholder);


	void text_poll(Event event);

	void initializeTextBox(std::string& targ_string, Texture& texture, const string placeholder = "Write Here", Vector2f pos = {0.0, 0.0}, float scale = 1);

	void drawTextBox(RenderWindow* window);

	void update();

};