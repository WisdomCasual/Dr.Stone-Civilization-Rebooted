#include "PauseState.h"


void PauseState::update_buttons()
{
	for (int i = 0; i < 3; i++) {
		buttontex.setTextureRect(IntRect(0, buttons[i].pressed * 49, 190, 49));
		buttontex.setPosition(x + buttons[i].x * scale * 0.33, y + buttons[i].y * scale * 0.33);
		if (buttontex.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			if (Mouse::isButtonPressed(Mouse::Left) && buttontex.getGlobalBounds().contains(clicked_on))buttons[i].pressed = 1;
			else {
				if (buttons[i].pressed)
					*buttons[i].execute = 1;
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

void PauseState::render_buttons()
{
	buttontex.setScale(scale * 0.33, scale * 0.33);
	button_text.setCharacterSize(7.5 * scale);
	for (auto& button : buttons) {
		buttontex.setTextureRect(IntRect(0, button.pressed * 49, 190, 49));
		buttontex.setPosition(x + button.x * scale * 0.33, y + button.y * scale * 0.33);
		button_text.setString(button.txt);
		FloatRect bounds = button_text.getLocalBounds();
		button_text.setOrigin(bounds.width / 2.0, bounds.top + bounds.height / 2.0);
		button_text.setPosition(x + button.x * scale * 0.33, (button.pressed) ? y + button.y * scale * 0.33 + 2 * scale * 0.33 : y + button.y * scale * 0.33 - 2 * scale * 0.33);
		if (button.hover)button_text.setFillColor(Color::White);
		else button_text.setFillColor(Color::Color(226, 211, 195));
		window->draw(buttontex);
		window->draw(button_text);
	}
}

PauseState::PauseState()
{
	initial_textures("pause");

	panel.setTexture(*textures[3]);
	panel.setOrigin(600 / 2, 600 / 2);

	tissue.setTexture(*textures[2]);
	tissue.setOrigin(700 / 2, 700 / 2);

	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));

	buttontex.setTexture(*textures[0]);
	buttontex.setTextureRect(IntRect(0, 0, 190, 49));
	buttontex.setOrigin(190 / 2, 49 / 2);

	button_text.setFont(font);
	
	buttons[0].execute = &resume;
	buttons[1].execute = &settings;
	buttons[2].execute = &exit;
}

PauseState::~PauseState()
{
}

void PauseState::update()
{
	window->setMouseCursorVisible(true);

	mouse_pos = Mouse::getPosition(*window);

	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 120.0 < win_y / 120.0) scale = win_x / 120.0;
		else scale = win_y / 120.0;

		if (win_x > 1280) scale *= 0.75;
		/////////////////////

		tint.setSize({ win_x, win_y });

		panel.setPosition(x, y);
		panel.setScale(scale * 0.17, scale * 0.17);

		tissue.setPosition(x, y);
		tissue.setScale(scale * 0.13, scale * 0.13);
	}

	update_buttons();

	if (resume) {
		resume = 0;

		delete states->at(PauseID);
		states->erase(PauseID);
		return;
	}
	else if (settings) {
		settings = 0;

		states->insert(SettingsST);

		int exceptions[] = { SettingsID, BackgroundID, MapBuilderID, GameID, WorldMapID, NotificationID };
		game.erase_states(exceptions, 6);
		return;
	}
	else if (exit) {
		exit = 0; 

		states->insert(MainMenuST);

		if (states->find(BackgroundID) == states->end())
			states->insert(BackgroundST);

		int exceptions[] = { MainMenuID , BackgroundID};
		game.erase_states(exceptions, 2);
		return;
	}
}

void PauseState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				resume = 1; break;
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

void PauseState::render()
{
	window->draw(tint);
	window->draw(panel);
	window->draw(tissue);
	render_buttons();

	text.setFillColor(Color::Black);
	draw_text("Paused", x, y - 35 * scale, 6.5 * scale);
}
