#include "ConfirmationState.h"

void ConfirmationState::update_buttons()
{
	for (int i = 0; i < 2; i++) {
		buttontex.setPosition(x + buttons[i].x * scale, y + buttons[i].y * scale);
		if (buttontex.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			if (Mouse::isButtonPressed(Mouse::Left) && buttontex.getGlobalBounds().contains(clicked_on))buttons[i].pressed = 1;
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
			button_text.setFillColor(Color::White);
		else
			button_text.setFillColor(Color(200, 200, 200));

		window->draw(buttontex);
		window->draw(button_text);
	}
}

void ConfirmationState::render_strings()
{
	float i = (lines / 2.0 - 2) * -1, dis = (92 / lines) * scale * 1.3;
	text.setFillColor(Color::Black);
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
	mouse_pos = window->mapPixelToCoords(Mouse::getPosition(*window));

	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 350.0 < win_y / 350.0) scale = win_x / 350.0;
		else scale = win_y / 350.0;

		if (win_x > 1280) scale *= 0.75;

		///////////////////////////////////

		tint.setSize({ win_x, win_y });
		BG.setPosition(x, y + 20 * scale);
		BG.setScale(scale * .35, scale * .35);
		tex_bg.setPosition(x, y);
		tex_bg.setScale(scale * 1.3, scale * 1.3);
	}

	if (no){
		no = 0;
		delete states->at(ConfirmationID);
		states->erase(ConfirmationID);
		return;
	}
	else if (yes){
		function = 1, yes = 1;
		delete states->at(ConfirmationID);
		states->erase(ConfirmationID);
		return;
	}

	update_buttons();
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
		case Event::Closed:
			window->close(); break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				delete states->at(ConfirmationID);
				states->erase(ConfirmationID);
				return; break;
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
