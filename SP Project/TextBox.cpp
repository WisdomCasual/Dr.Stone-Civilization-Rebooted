#include "TextBox.h"
#include "Game.h"

TextBox::TextBox()
{
	//text
	inputted_text.setFont(font);
	inputted_text.setCharacterSize(40*scale);
	inputted_text.setPosition(position);
	keypress_buff.loadFromFile("Audio/UI/keypress.ogg");
	keypress.setBuffer(keypress_buff);
	enterpress_buff.loadFromFile("Audio/UI/ENTER.ogg");
	enterpress.setBuffer(enterpress_buff);
}

void TextBox::setTargetString(string& target_string)
{
	this->target_string = &target_string;
}

void TextBox::setTexture(Texture& box_texture)
{
	box.setTexture(box_texture);
	box.setScale(scale * 1.5, scale * 1.5);
	box.setOrigin(box.getLocalBounds().left + box.getLocalBounds().width / 2.0, box.getLocalBounds().top + box.getLocalBounds().height / 2.0);
	box.setPosition(position);
}

void TextBox::setColor(Color color)
{
	box.setColor(color);
	transparency = color.a;
}

void TextBox::setFont(Font font)
{
	inputted_text.setFont(font);
}

void TextBox::setPosition(const Vector2f new_position)
{
	position = new_position;
	box.setPosition(position);
	inputted_text.setPosition(position);
	placeholder_text.setPosition(position);
}

void TextBox::setLimit(float limit)
{
	this->limit = limit;
}

void TextBox::setScale(const float new_scale)
{
	scale = new_scale;
	box.setScale(scale * 1.5, scale * 1.5);
	inputted_text.setCharacterSize(40 * scale);
	placeholder_text.setCharacterSize(40 * scale);
}

void TextBox::setPlaceholderText(const string placeholder)
{
	placeholder_text.setString(placeholder);
	this->placeholder = placeholder;
}

void TextBox::text_poll(Event event)
{
	switch (event.type) {
	case Event::MouseButtonPressed:
		if (event.mouseButton.button == Mouse::Left) {
			if (box.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
				if (!isActive)
					game.play_sfx(0);
				isActive = 1;
			}
			else {
				isActive = 0;
				save_text();
			}
			selected = 0;
		}

		break;
	case Event::TextEntered:
		if (isActive) {
			if (event.text.unicode == 13) {
				enterpress.setVolume(game_volume);
				enterpress.setPitch(generate_random_f(0.8f, 0.9f));
				enterpress.play();
				submit();
				isActive = 0;
			}

			if (event.text.unicode == 8 && !input_string.empty()) {
				keypress.setVolume(game_volume);
				keypress.setPitch(generate_random_f(0.8f, 0.9f));
				keypress.play();
				if(selected) { 
					input_string.clear();
					pw_string.clear();
					selected = 0;
				}
				else {
					input_string.pop_back();
					pw_string.pop_back();
				}
			}
			if (event.text.unicode > 31 && event.text.unicode < 127 && event.text.unicode != 96 && input_string.size() < character_limit && text_x_bound < box_x_bound && text_x_bound < limit * scale) {
				if (selected) {
					input_string.clear();
					pw_string.clear();
					selected = 0;
				}
				keypress.setVolume(game_volume);
				keypress.setPitch(generate_random_f(0.9f, 1.f));
				keypress.play();
				input_string += event.text.unicode;
				pw_string += '*';
			}
		} break;
	case Event::KeyPressed:
		if (isActive && event.key.control) {
			if (event.key.code == Keyboard::V) {
				clipboard = Clipboard::getString();
				if (selected) {
					input_string.clear();
					pw_string.clear();
				}
				for (int i = 0; i < clipboard.size() && input_string.size() < character_limit && text_x_bound < box_x_bound && text_x_bound < limit * scale; i++) {
					if (clipboard[i] > 31 && clipboard[i] < 127 && clipboard[i] != 96) {
						input_string += clipboard[i];
						pw_string += '*';
					}
				}
				selected = 0;

				inputted_text.setString(input_string);
				inputted_text.setOrigin(inputted_text.getGlobalBounds().width / 2.0, bound_y);
				inputted_text.setPosition(position);
			}

			if (event.key.code == Keyboard::A) {
				selected = 1;
			}
			if (event.key.code == Keyboard::C && selected) {
				Clipboard::setString(*display_string);
			}
		}
		break;
	}
}

void TextBox::initializeTextBox(string& targ_string, Texture& texture, const string placeholder, Vector2f pos, float scale, bool mode, bool* submit_bool)
{
	position = pos;
	this->scale = scale;
	this->submit_bool = submit_bool;
	target_string = &targ_string;
	placeholder_text.setFont(font);
	placeholder_text.setFillColor(Color(177, 146, 114));
	setMode(mode);
	setScale(scale);
	setPlaceholderText(placeholder);
	setTexture(texture);
}

void TextBox::drawTextBox(RenderWindow* window)
{
	window->draw(box);
	if (input_string.empty() && !isActive)
		window->draw(placeholder_text);
	window->draw(inputted_text);
}

bool TextBox::empty()
{
	return input_string.empty();
}

void TextBox::setMode(bool mode)
{
	this->mode = mode;
	display_string = (mode) ? &pw_string : &input_string;
}

void TextBox::setSubmitBool(bool* submit_bool)
{
	this->submit_bool = submit_bool;
}

void TextBox::save_text()
{
	if (mode) {
		for (int i = 0; i < input_string.size(); i++) {
			*target_string = input_string;
			encrypt(*target_string, 69);
		}
	}
	else
		*target_string = input_string;
}

void TextBox::submit()
{
	save_text();
	if (submit_bool != nullptr)
		*submit_bool = 1;
}

void TextBox::update()
{
	inputted_text.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	bound_y = inputted_text.getLocalBounds().top + inputted_text.getLocalBounds().height / 2.0;
	inputted_text.setString(*display_string);
	placeholder_text.setOrigin(placeholder_text.getLocalBounds().left + placeholder_text.getLocalBounds().width / 2.0, placeholder_text.getLocalBounds().top + placeholder_text.getLocalBounds().height / 2.0);
	box_x_bound = box.getGlobalBounds().width * 0.8, text_x_bound = inputted_text.getGlobalBounds().width;
	box.setOrigin(box.getLocalBounds().left + box.getLocalBounds().width / 2.0, box.getLocalBounds().top + box.getLocalBounds().height / 2.0);
	inputted_text.setOrigin(inputted_text.getLocalBounds().left + inputted_text.getLocalBounds().width / 2.0, bound_y);
	setPosition(position);
	if (selected)
		inputted_text.setFillColor(Color(0, 0, 255, transparency));
	else if(isActive)
		inputted_text.setFillColor(Color(255, 255, 255, transparency));
	else
		inputted_text.setFillColor(Color(200, 200, 200, transparency));
	placeholder_text.setFillColor(Color(177, 146, 114, transparency));
	string output_string;
	if (cursor && isActive) {
		output_string = *display_string + ((cursor) ? "|" : "");
		inputted_text.setString(output_string);
	}
	if (delay > 1.0 && isActive) {
		delay = 0; cursor = !cursor;
	}
	else if (isActive)
		delay += dt;
	else
		delay = 0;

	if (box.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window))))
			clickable_cursor = true;
}