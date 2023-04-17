#include "PasswordState.h"

void PasswordState::update_arrow()
{
	back_arrow.setPosition(x - 40 * scale, y - (float)8.5 * scale);
	if (back_arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		back_arrow.setTextureRect(IntRect(22, 0, 22, 21));
		back_arrow.setScale(scale * 0.4, scale * 0.4);
		if (Mouse::isButtonPressed(Mouse::Left) && back_arrow.getGlobalBounds().contains(clicked_on)) {
			arrow_pressed = 1;
			back_arrow.setTextureRect(IntRect(44, 0, 22, 21));
			back_arrow.setScale(scale * 0.36, scale * 0.36);
		}
		else {
			if (arrow_pressed) {
				arrow_pressed = 0;
				states->erase(PasswordID);
				destruct = 1;
				return;
			}
		}
	}
	else {
		arrow_pressed = 0;
		back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
		back_arrow.setScale(scale * 0.4, scale * 0.4);
	}
}

void PasswordState::update_buttons()
{
	if (!txt_box.empty()) {
		if (wrong_password)
			buttontex.setColor(Color(button_color.r, button_color.g - 100, button_color.b - 100));
		else
			buttontex.setColor(button_color);
		buttontex.setPosition(x + confirm.x * scale / 3.5, y + confirm.y * scale / 3.5);
		if (buttontex.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			if (Mouse::isButtonPressed(Mouse::Left) && buttontex.getGlobalBounds().contains(clicked_on))confirm.pressed = 1;
			else {
				if (confirm.pressed)
					confirm.execute = 1;
				confirm.pressed = 0;
			}
			confirm.hover = 1;
		}
		else {
			confirm.pressed = 0;
			confirm.hover = 0;
		}
	}
	else {
		buttontex.setColor(Color(button_color.r - 100, button_color.g - 100, button_color.b - 100));
		wrong_password = 0;
	}
}

void PasswordState::render_buttons()
{
	buttontex.setScale(scale / 3.5, scale / 3.5);
	button_text.setCharacterSize(8.69 * scale);
	buttontex.setTextureRect(IntRect(0, confirm.pressed * 49, 108, 49));
	buttontex.setPosition(x + confirm.x * scale / 3.5, y + confirm.y * scale / 3.5);
	button_text.setString(confirm.txt);
	FloatRect bounds = button_text.getLocalBounds();
	button_text.setOrigin(bounds.width / 2.0, bounds.top + bounds.height / 2.0);
	button_text.setPosition(x + confirm.x * scale / 3.5, (confirm.pressed) ? y + confirm.y * scale / 3.5 + 2 * scale / 3.5 : y + confirm.y * scale / 3.5 - 2 * scale / 3.5);
	if (confirm.hover && wrong_password) button_text.setFillColor(Color(235, 205, 205));
	else if (confirm.hover) button_text.setFillColor(Color::White);
	else if (txt_box.empty())
		button_text.setFillColor(Color(120, 120, 120));
	else if (wrong_password)
		button_text.setFillColor(Color(200, 170, 170));
	else
		button_text.setFillColor(Color(200, 200, 200));

	window->draw(buttontex);
	window->draw(button_text);
}

PasswordState::PasswordState()
{
	win_x = window->getSize().x, win_y = window->getSize().y;
	x = win_x / 2, y = win_y / 2;
	if (win_x / 150.0 < win_y / 150.0) scale = win_x / 150.0;
	else scale = win_y / 150.0;
	if (win_x > 1280) scale *= 0.75;

	initial_textures("password");

	tissue.setTexture(*textures[1]);
	tissue.setOrigin(148 / 2, 60 / 2);

	bg.setTexture(*textures[0]);
	bg.setOrigin(160 / 2, 72 / 2);

	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));

	back_arrow.setTexture(*textures[4]);
	back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
	back_arrow.setOrigin(22 / 2, 21 / 2);

	buttontex.setTexture(*textures[3]);
	buttontex.setTextureRect(IntRect(0, 0, 108, 49));
	buttontex.setOrigin(108 / 2, 49 / 2);
	button_color = buttontex.getColor();

	button_text.setFont(font);
	button_text.setCharacterSize(50);
	button_text.setFillColor(Color(200, 200, 200));

	txt_box.initializeTextBox(input_password, *textures[2], "Enter Password");
	txt_box.setMode(1);

}

PasswordState::~PasswordState()
{

}


void PasswordState::update()
{
	mouse_pos = window->mapPixelToCoords(Mouse::getPosition(*window));

	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 150.0 < win_y / 150.0) scale = win_x / 150.0;
		else scale = win_y / 150.0;

		if (win_x > 1280) scale *= 0.75;

		///////////////////////////////////

		tint.setSize({ win_x, win_y });

		tissue.setScale(scale * 0.73, scale * 0.73);
		tissue.setPosition(x, y);

		bg.setPosition(x, y);
		bg.setScale(scale * 0.75, scale * 0.75);

		txt_box.setPosition({ x, y - (float) 8.5 * scale });

		txt_box.setScale(scale * 0.2);
	}

	update_arrow();

	update_buttons();

	if (destruct)
		return;

	txt_box.update();

	if (confirmed) {
		confirmed = 0;
		if (input_password == "xdCy1N" || input_password == "O-FxJ<" || input_password == "rHU6F" || input_password == ")YPN6" 
			|| input_password == "ay=yr") {    //password: team7 - raheeq - amany - sobia
			states->insert(WorldMapST_admin);
			states->at(WorldMapID)->update();
			states->erase(SettingsID);
			states->erase(PasswordID);
			return;
		}
		else
			wrong_password = 1;
	}
}


void PasswordState::render()
{
	window->draw(tint);
	window->draw(bg);
	window->draw(tissue);

	window->draw(back_arrow);

	render_buttons();

	txt_box.drawTextBox(window);
}

void PasswordState::pollevent()
{
	while (window->pollEvent(event)) {
		txt_box.text_poll(event);
		switch (event.type) {
		case Event::Closed:
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				states->erase(PasswordID); return; break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			}
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case Mouse::Left:
				clicked_on = window->mapPixelToCoords(Mouse::getPosition(*window));
				break;
			}
		}
	}
}
