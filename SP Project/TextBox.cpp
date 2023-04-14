#include "TextBox.h"


TextBox::TextBox()
{
	//text
	inputted_text.setFont(font);
	inputted_text.setCharacterSize(40*scale);
	inputted_text.setPosition(position);
}

void TextBox::setTargetString(string& target_string)
{
	this->target_string = &target_string;
}

void TextBox::setTexture(Texture& box_texture)
{
	box.setTexture(box_texture);
	box.setScale(scale * 1.5, scale * 1.5);
	box.setPosition(position);
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
			if (box.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window))))
				isActive = 1;
			else
				isActive = 0;
			selected = 0;
		}

		break;
	case Event::TextEntered:
		if (isActive) {
			if (event.text.unicode == 13) {
				*target_string = input_string;
				input_string.clear();
			}

			if (event.text.unicode == 8 && !input_string.empty()) {
				if(selected) { 
					input_string.clear();
					selected = 0;
				}
				else
					input_string.pop_back();
			}
			if (event.text.unicode > 31 && event.text.unicode < 127 && event.text.unicode != 96 && input_string.size() < character_limit && text_x_bound < box_x_bound) {
				if (selected) {
					input_string.clear();
					selected = 0;
				}
				input_string += event.text.unicode;
			}
				inputted_text.setString(input_string);
		} break;
	case Event::KeyPressed:
		if (isActive && event.key.control) {
			if (event.key.code == Keyboard::V) {
				clipboard = Clipboard::getString();
				if (selected)
					input_string.clear();
				for (int i = 0; i < clipboard.size() && input_string.size() < character_limit && text_x_bound < box_x_bound; i++) {
					if (clipboard[i] > 31 && clipboard[i] < 127 && clipboard[i] != 96) {
						input_string += clipboard[i];
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
				Clipboard::setString(input_string);
			}
		}
		break;
	}
}

void TextBox::initializeTextBox(string& targ_string, Texture& texture, const string placeholder, Vector2f pos, float scale)
{
	position = pos;
	this->scale = scale;
	target_string = &targ_string;
	placeholder_text.setFont(font);
	placeholder_text.setFillColor(Color(177, 146, 114, 255));
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

void TextBox::update()
{
	inputted_text.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	bound_y = inputted_text.getLocalBounds().top + inputted_text.getLocalBounds().height / 2.0;
	inputted_text.setString(input_string);
	placeholder_text.setOrigin(placeholder_text.getLocalBounds().left + placeholder_text.getLocalBounds().width / 2.0, placeholder_text.getLocalBounds().top + placeholder_text.getLocalBounds().height / 2.0);
	box_x_bound = box.getGlobalBounds().width * 0.8, text_x_bound = inputted_text.getGlobalBounds().width;
	box.setOrigin(box.getLocalBounds().left + box.getLocalBounds().width / 2.0, box.getLocalBounds().top + box.getLocalBounds().height / 2.0);
	inputted_text.setOrigin(inputted_text.getLocalBounds().left + inputted_text.getLocalBounds().width / 2.0, bound_y);
	setPosition(position);
	if (selected)
		inputted_text.setFillColor(Color::Blue);
	else
		inputted_text.setFillColor(Color::White);

	string output_string;
	if (cursor && isActive) {
		output_string = input_string + ((cursor) ? "|" : "");
		inputted_text.setString(output_string);
	}
	if (delay > 1.0 && isActive) {
		delay = 0; cursor = !cursor;
	}
	else if (isActive)
		delay += dt;
	else
		delay = 0;

}


