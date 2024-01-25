#include "PasswordState.h"


void PasswordState::fade_in()
{
	if (transparency < 255) {
		if (transparency + 1500 * dt > 255)
			transparency = 255;
		else
			transparency += 1500 * dt;

		bg.setColor(Color(255, 255, 255, (int)transparency));
		tissue.setColor(Color(255, 255, 255, (int)transparency));
		back_arrow.setColor(Color(255, 255, 255, (int)transparency));
		txt_box.setColor(Color(255, 255, 255, (int)transparency));

		if (darkness < 154) {
			if (darkness + 154 * dt * 6 > 154)
				darkness = 154;
			else
				darkness += 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, (int)darkness));
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

		bg.setColor(Color(255, 255, 255, (int)transparency));
		tissue.setColor(Color(255, 255, 255, (int)transparency));
		back_arrow.setColor(Color(255, 255, 255, (int)transparency));
		txt_box.setColor(Color(255, 255, 255, (int)transparency));

		if (darkness > 0) {
			if (darkness - 154 * dt * 6 < 0)
				darkness = 0;
			else
				darkness -= 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, (int)darkness));
		}
		return false;
	}
	else
		return true;
}

void PasswordState::update_arrow()
{

	// giving positon to the back arrow and detecting for clicks

	back_arrow.setPosition(x - 40 * scale, y - 8.5f * scale);
	back_arrow.setScale(scale * 0.4f, scale * 0.4f);
	if (back_arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		clickable_cursor = true;
		back_arrow.setTextureRect(IntRect(22, 0, 22, 21));
		if (Mouse::isButtonPressed(Mouse::Left) && back_arrow.getGlobalBounds().contains(clicked_on)) {
			if(!arrow_pressed)
				game.play_sfx(1);
			arrow_pressed = 1;
			back_arrow.setTextureRect(IntRect(44, 0, 22, 21));
			back_arrow.setScale(scale * 0.36f, scale * 0.36f);
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
			buttontex.setColor(Color(255, 155, 155, (int)transparency));
		else
			// set the button color to it's original color
			buttontex.setColor(Color(255, 255, 255, (int)transparency));
		buttontex.setPosition(x + confirm.x * scale / 3.5f, y + confirm.y * scale / 3.5f);
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
		buttontex.setColor(Color(155, 155, 155, (int)transparency));
		wrong_password = 0;
	}
}

void PasswordState::render_buttons()
{
	buttontex.setScale(scale / 3.5f, scale / 3.5f);
	button_text.setCharacterSize(int(8.69f * scale));
	buttontex.setTextureRect(IntRect(0, confirm.pressed * 49, 108, 49));
	buttontex.setPosition(x + confirm.x * scale / 3.5f, y + confirm.y * scale / 3.5f);
	button_text.setString(confirm.txt);
	FloatRect bounds = button_text.getLocalBounds();
	button_text.setOrigin(bounds.width / 2.f, bounds.top + bounds.height / 2.f);
	button_text.setPosition(x + confirm.x * scale / 3.5f, (confirm.pressed) ? y + confirm.y * scale / 3.5f + 2 * scale / 3.5f : y + confirm.y * scale / 3.5f - 2 * scale / 3.5f);
	if (confirm.hover && wrong_password) button_text.setFillColor(Color(235, 205, 205));
	else if (confirm.hover) button_text.setFillColor(Color(255, 255, 255, (int)transparency));
	else if (txt_box.empty())
		button_text.setFillColor(Color(120, 120, 120, (int)transparency));
	else if (wrong_password)
		button_text.setFillColor(Color(200, 170, 170, (int)transparency));
	else
		button_text.setFillColor(Color(200, 200, 200, (int)transparency));

	window->draw(buttontex);
	window->draw(button_text);
}

PasswordState::PasswordState()
{

	win_x = (float)window->getSize().x, win_y = (float)window->getSize().y;
	x = win_x / 2.f, y = win_y / 2.f;
	if (win_x / 150.f < win_y / 150.f) scale = win_x / 150.f;
	else scale = win_y / 150.f;
	if (VideoMode::getDesktopMode().width < win_x * 1.7 || VideoMode::getDesktopMode().height < win_y * 1.7) scale *= 0.75;

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
		win_x = (float)window->getSize().x, win_y = (float)window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 150.f < win_y / 150.f) scale = win_x / 150.f;
		else scale = win_y / 150.f;

		if (VideoMode::getDesktopMode().width < win_x * 1.7 || VideoMode::getDesktopMode().height < win_y * 1.7f) scale *= 0.75f;

		///////////////////////////////////

		tint.setSize({ win_x, win_y });

		tissue.setScale(scale * 0.73f, scale * 0.73f);
		tissue.setPosition(x, y);

		bg.setPosition(x, y);
		bg.setScale(scale * 0.75f, scale * 0.75f);

		txt_box.setPosition({ x, y - (float) 8.5f * scale });

		txt_box.setScale(scale * 0.2f);
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
