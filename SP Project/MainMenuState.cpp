#include "MainMenuState.h"


void MainMenuState::fade_in()
{
	if (transparency < 255) {
		if (transparency + 800 * dt > 255)
			transparency = 255;
		else
			transparency += 800 * dt;
		buttontex.setColor(Color(255, 255, 255, transparency));
		logo.setColor(Color(255, 255, 255, transparency));
	}
}

bool MainMenuState::fade_out()
{
	if (transparency > 0) {
		if (transparency - 1500 * dt < 0)
			transparency = 0;
		else
			transparency -= 1500 * dt;

		buttontex.setColor(Color(255, 255, 255, transparency));
		logo.setColor(Color(255, 255, 255, transparency));

		return false;
	}
	else
		return true;
}

void MainMenuState::update_buttons()
{
	for (int i = 0; i < 3; i++) {
		buttontex.setTextureRect(IntRect(0,buttons[i].pressed * 49, 190, 49));
		buttontex.setPosition(x+buttons[i].x * scale, y+buttons[i].y * scale);
		if (buttontex.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			clickable_cursor = true;
			if (Mouse::isButtonPressed(Mouse::Left) && buttontex.getGlobalBounds().contains(clicked_on)) {
				if(!buttons[i].pressed)
					game.play_sfx(0);
				buttons[i].pressed = 1;
			}
			else {
				if (buttons[i].pressed) {
					*buttons[i].execute = 1;
				}
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

void MainMenuState::render_buttons()
{
	buttontex.setScale(scale, scale);
	text.setCharacterSize(25.69*scale);
	for (int i = 0; i < 3; i++) {
		buttontex.setTextureRect(IntRect(0, buttons[i].pressed * 49, 190, 49));
		buttontex.setPosition(x+buttons[i].x*scale, y+buttons[i].y * scale);
		text.setString(buttons[i].txt);
		FloatRect bounds=text.getLocalBounds();
		text.setOrigin(bounds.width / 2.0, bounds.top+bounds.height / 2.0);
		text.setPosition(x + buttons[i].x * scale, (buttons[i].pressed) ? y + buttons[i].y * scale + 2 * scale : y + buttons[i].y * scale - 2 * scale);
		if (buttons[i].hover)text.setFillColor(Color(255, 255, 255, transparency));
		else text.setFillColor(Color(226,211,195, transparency));
		window->draw(buttontex);
		window->draw(text);
	}
}

MainMenuState::MainMenuState()
{
	active_cursor = true;

	initial_textures("mainmenu");

	buttontex.setTexture(*textures[0]);
	buttontex.setTextureRect(IntRect(0, 0, 190, 49));
	buttontex.setOrigin(190/2,49/2);

	logo.setTexture(*textures[1]);
	logo.setOrigin(2280/2,772/2);

	text.setFont(font);

	buttons[0].txt = "Play";
	buttons[0].execute = &play;

	buttons[1].txt = "Settings";
	buttons[1].y = 64;
	buttons[1].execute = &settings;

	buttons[2].txt = "Exit";
	buttons[2].y = 128;
	buttons[2].execute = &exit;
}

MainMenuState::~MainMenuState()
{

}

void MainMenuState::update()
{
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		float win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 390.0 < win_y / 390.0) scale = win_x / 390.0;
		else scale = win_y / 390.0;
		
		logo.setPosition(x, y - 110 * scale);

		if (win_x > 1280) scale *= 0.8;

		logo.setScale(scale * 0.13, scale * 0.13);
	}


	update_buttons();

	if (exit) {
		exit = 0;
		game.exit_prompt();
		return;
	}
	else if (play) {
		if(fade_out()){
			play = 0;
			states->insert(SavesST);
			states->at(SavesID)->update();

			if (states->find(BackgroundID) != states->end()) {
				states->insert(BackgroundST);
				states->at(BackgroundID)->update();
			}


			int exceptions[] = { SavesID , BackgroundID };
			game.erase_states(exceptions, 2);
			return;
		}
	}
	else if (settings) {
		if(fade_out()){
			settings = 0;
			states->insert(SettingsST);
			states->at(SettingsID)->update();

			if (states->find(BackgroundID) != states->end()) {
				states->insert(BackgroundST);
				states->at(BackgroundID)->update();
			}

			int exceptions[] = { SettingsID , BackgroundID };
			game.erase_states(exceptions, 2);
			return;
		}
	}
	else
		fade_in();
}

void MainMenuState::render()
{
	render_buttons();
	window->draw(logo);
}

void MainMenuState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				game.exit_prompt();
				return; break;
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
