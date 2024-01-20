#include "ConfirmationState.h"


void ConfirmationState::fade_in()
{
	if (transparency < 255) {
		if (transparency + 1500 * dt > 255)
			transparency = 255;
		else
			transparency += 1500 * dt;

		BG.setColor(Color(255, 255, 255, transparency));
		tex_bg.setColor(Color(255, 255, 255, transparency));
		buttontex.setColor(Color(255, 255, 255, transparency));

		if (darkness < 154) {
			if (darkness + 154 * dt * 6 > 154)
				darkness = 154;
			else
				darkness += 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, darkness));
		}
	}
}

bool ConfirmationState::fade_out()
{
	if (transparency > 0) {
		if (transparency - 1500 * dt < 0)
			transparency = 0;
		else
			transparency -= 1500 * dt;

		BG.setColor(Color(255, 255, 255, transparency));
		tex_bg.setColor(Color(255, 255, 255, transparency));
		buttontex.setColor(Color(255, 255, 255, transparency));

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

void ConfirmationState::update_buttons()
{
	for (int i = 0; i < 2; i++) {
		buttontex.setPosition(x + buttons[i].x * scale, y + buttons[i].y * scale);
		if (buttontex.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			clickable_cursor = true;
			if (Mouse::isButtonPressed(Mouse::Left) && buttontex.getGlobalBounds().contains(clicked_on)) {
				if (!buttons[i].pressed) {
					if(i)
						game.play_sfx(0);
					else
						game.play_sfx(1);
				}
				buttons[i].pressed = 1;
			}
			else {
				if (buttons[i].pressed)
					buttons[i].execute = 1;
				buttons[i].pressed = 0;
			}
			buttons[i].hover = 1;
		}
		else {
			buttons[i].pressed = 0;
			buttons[i].hover = 0;
		}
	}
}

void ConfirmationState::render_buttons()
{
	buttontex.setScale(scale * .73, scale * .73);
	button_text.setCharacterSize(20 * scale);
	for (int i = 0; i < 2; i++) {
		buttontex.setTextureRect(IntRect(0, buttons[i].pressed * 49, 108, 49));
		buttontex.setPosition(x + buttons[i].x * scale, y + buttons[i].y * scale);
		button_text.setString(buttons[i].txt);
		FloatRect bounds = button_text.getLocalBounds();
		button_text.setOrigin(bounds.width / 2.0, bounds.top + bounds.height / 2.0);
		button_text.setPosition(x + buttons[i].x * scale, (buttons[i].pressed) ? y + buttons[i].y * scale + 2 * scale : y + buttons[i].y * scale - 2 * scale);
		if (buttons[i].hover)
			button_text.setFillColor(Color(255, 255, 255, transparency));
		else
			button_text.setFillColor(Color(200, 200, 200, transparency));

		window->draw(buttontex);
		window->draw(button_text);
	}
}

void ConfirmationState::render_strings()
{
	float i = (lines / 2.0 - 2) * -1, dis = (92 / lines) * scale * 1.3;
	text.setFillColor(Color(0, 0, 0, transparency));
	for (int j = 0; j < lines - 3; j++, i++) {
		draw_text(text_strings[j], x, y + i * dis, (5 + 115 / lines) * scale);
	}

}

ConfirmationState::ConfirmationState(string text_strings[], int lines, bool& fn) : function(fn)
{
	for (int i = 0; i < lines; i++) {
		this->text_strings[i] = text_strings[i];
	}
	this->lines = lines + 3;
	initial_textures("confirm");
	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));
	buttontex.setTexture(*textures[2]);
	buttontex.setTextureRect(IntRect(0, 0, 108, 48));
	buttontex.setOrigin(108 / 2, 48 / 2);
	BG.setTexture(*textures[0]);
	BG.setOrigin(300, 300);
	tex_bg.setTexture(*textures[1]);
	tex_bg.setOrigin(148 / 2, 92 / 2);
	button_text.setFont(font);
	button_text.setCharacterSize(50);
	button_text.setFillColor(Color(200, 200, 200));
}

ConfirmationState::~ConfirmationState()
{

}

void ConfirmationState::update()
{	
	active_cursor = true;
	mouse_pos = window->mapPixelToCoords(Mouse::getPosition(*window));

	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 350.0 < win_y / 350.0) scale = win_x / 350.0;
		else scale = win_y / 350.0;

		if (VideoMode::getDesktopMode().width < win_x * 1.7 || VideoMode::getDesktopMode().height < win_y * 1.7) scale *= 0.75;

		///////////////////////////////////

		tint.setSize({ win_x, win_y });
		BG.setPosition(x, y + 20 * scale);
		BG.setScale(scale * .35, scale * .35);
		tex_bg.setPosition(x, y);
		tex_bg.setScale(scale * 1.3, scale * 1.3);
	}

	if (no) {
		if(fade_out()){
		no = 0;
		delete states->at(ConfirmationID);
		states->erase(ConfirmationID);
		return;
		}
	}
	else if (yes) {
		if (fade_out()) {
			function = 1, yes = 0;
			delete states->at(ConfirmationID);
			states->erase(ConfirmationID);
			return;
		}
	}
	else
		fade_in();
	update_buttons();
	//dtclock.restart();
}

void ConfirmationState::render()
{
	window->draw(tint);
	window->draw(BG);
	window->draw(tex_bg);
	render_strings();
	render_buttons();
}

void ConfirmationState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				no = true; break;
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
