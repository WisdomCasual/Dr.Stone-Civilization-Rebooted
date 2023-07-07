#include "PasswordState.h"


void PasswordState::fade_in()
{
	if (transparency < 255) {
		if (transparency + 1500 * dt > 255)
			transparency = 255;
		else
			transparency += 1500 * dt;

		bg.setColor(Color(255, 255, 255, transparency));
		tissue.setColor(Color(255, 255, 255, transparency));
		back_arrow.setColor(Color(255, 255, 255, transparency));
		txt_box.setColor(Color(255, 255, 255, transparency));

		if (darkness < 154) {
			if (darkness + 154 * dt * 6 > 154)
				darkness = 154;
			else
				darkness += 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, darkness));
		}
	}
}

bool PasswordState::fade_out()
{
	if (transparency > 0) {
		if (transparency - 1500 * dt < 0)
			transparency = 0;
		else
			transparency -= 1500 * dt;

		bg.setColor(Color(255, 255, 255, transparency));
		tissue.setColor(Color(255, 255, 255, transparency));
		back_arrow.setColor(Color(255, 255, 255, transparency));
		txt_box.setColor(Color(255, 255, 255, transparency));

		if (darkness > 0) {
			if (darkness - 154 * dt * 6 < 0)
				darkness = 0;
			else
				darkness -= 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, darkness));
		}
		return false;
	}
	else
		return true;
}

void PasswordState::update_arrow()
{

	// giving positon to the back arrow and detecting for clicks

	back_arrow.setPosition(x - 40 * scale, y - (float)8.5 * scale);
	back_arrow.setScale(scale * 0.4, scale * 0.4);
	if (back_arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		clickable_cursor = true;
		back_arrow.setTextureRect(IntRect(22, 0, 22, 21));
		if (Mouse::isButtonPressed(Mouse::Left) && back_arrow.getGlobalBounds().contains(clicked_on)) {
			if(!arrow_pressed)
				game.play_sfx(1);
			arrow_pressed = 1;
			back_arrow.setTextureRect(IntRect(44, 0, 22, 21));
			back_arrow.setScale(scale * 0.36, scale * 0.36);
		}
		else {
			if (arrow_pressed) {
				arrow_pressed = 0;
				back = true;
				return;
			}
		}
	}
	else {
		arrow_pressed = 0;
		back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
	}
}

void PasswordState::update_buttons()
{

	if (!txt_box.empty()) {
		if (wrong_password)
			// if the password is wrong the and clicked on the button turns a bit red
			buttontex.setColor(Color(255, 155, 155, transparency));
		else
			// set the button color to it's original color
			buttontex.setColor(Color(255, 255, 255, transparency));
		buttontex.setPosition(x + confirm.x * scale / 3.5, y + confirm.y * scale / 3.5);
		if (buttontex.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			clickable_cursor = true;
			if (Mouse::isButtonPressed(Mouse::Left) && buttontex.getGlobalBounds().contains(clicked_on)) {
				if(!confirm.pressed)
					game.play_sfx(0);
				confirm.pressed = 1;
			}
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
		// when nothing is typed the button's color to a dim color
		buttontex.setColor(Color(155, 155, 155, transparency));
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
	else if (confirm.hover) button_text.setFillColor(Color(255, 255, 255, transparency));
	else if (txt_box.empty())
		button_text.setFillColor(Color(120, 120, 120, transparency));
	else if (wrong_password)
		button_text.setFillColor(Color(200, 170, 170, transparency));
	else
		button_text.setFillColor(Color(200, 200, 200, transparency));

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

	button_text.setFont(font);
	button_text.setCharacterSize(50);
	button_text.setFillColor(Color(200, 200, 200));

	txt_box.initializeTextBox(input_password, *textures[2], "Enter Password");
	txt_box.setMode(1);
	txt_box.setSubmitBool(&confirmed);

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

	if (back) {
		if (fade_out()) {
			back = false;
			states->erase(PasswordID);
			return;
		}
	}
	else if (proceed) {
		game.music.stop();
		if (fade_out()) {
			proceed = false;
			string placeholer;
			states->insert({ WorldMapID, new WorldMapState(placeholer, 1, 0) });
			states->at(WorldMapID)->update();
			states->erase(SettingsID);
			states->erase(PasswordID);
			return;
		}
	}
	else
		fade_in();

	txt_box.update();

	if (confirmed) {
		confirmed = 0;
		if (input_password == "xdCy1N" || input_password == "O-FxJ<" || input_password == "rHU6F" || input_password == ")YPN6" 
			|| input_password == "ay=yr") {    //password: team7 - raheeq - amany - sobia
			
			// when the password is confirmed we remove the settings state and the password state and add the worldmap state
			// which we can access map builder state from
			proceed = true;
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
				back = true; break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			}
			break;
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case Mouse::Left:
				clicked_on = window->mapPixelToCoords(Mouse::getPosition(*window));
				break;
			}
		}
	}
}
