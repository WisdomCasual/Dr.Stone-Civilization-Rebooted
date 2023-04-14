#include "TextBox.h"


TextBox::TextBox()
{
	//text
	inputted_text.setFont(font);
	inputted_text.setCharacterSize(40*scale);
	inputted_text.setString("abcdefghijklmnopqrstuvwxyz");
	bound_y = inputted_text.getGlobalBounds().top;
	inputted_text.setString("");
	inputted_text.setPosition(position);
}

void TextBox::setTargetString(string& target_string)
{
	this->target_string = &target_string;
}

void TextBox::setTexture(Texture& box_texture)
{
	box.setTexture(box_texture);
	box.setOrigin(box.getGlobalBounds().width / 2.0,box.getGlobalBounds().top + box.getGlobalBounds().height / 2.0);
	box.setScale(scale, scale);
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
	box.setScale(scale, scale);
	inputted_text.setCharacterSize(40 * scale);
	placeholder_text.setCharacterSize(40 * scale);
}

void TextBox::setPlaceholderText(const string placeholder)
{
	placeholder_text.setString(placeholder);
	placeholder_text.setOrigin(placeholder_text.getLocalBounds().left + placeholder_text.getLocalBounds().width / 2.0, bound_y/1.25);
	placeholder_text.setPosition(position);
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
		}

		break;
	case Event::TextEntered:
		if (isActive) {
			if (event.text.unicode == 10) {
				*target_string = input_string;
				input_string.clear();
			}

			if (event.text.unicode == 8 && !input_string.empty()) {
				input_string.pop_back();
			}
			if (event.text.unicode > 31 && event.text.unicode < 127 && event.text.unicode != 96 && input_string.size() <= character_limit) {
				input_string += event.text.unicode;
			}
				inputted_text.setString(input_string);
				inputted_text.setOrigin(inputted_text.getGlobalBounds().width / 2.0, bound_y/1.25);
				inputted_text.setPosition(position);
		} break;

	}
}

void TextBox::initializeTextBox(string& targ_string, Texture& texture, const string placeholder, Vector2f pos, float scale)
{
	position = pos;
	this->scale = scale;
	target_string = &targ_string;
	placeholder_text.setFont(font);
	setScale(scale);
	placeholder_text.setFillColor(Color(177, 146, 114, 255));
	setPlaceholderText(placeholder);
	setTexture(texture);
	setPosition(pos);
}

void TextBox::drawTextBox(RenderWindow* window)
{
	window->draw(box);
	if (input_string.empty() && !isActive)
		window->draw(placeholder_text);
	window->draw(inputted_text);
}


