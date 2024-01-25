#include "MainMenuState.h"


void MainMenuState::fade_in()
{
	if (transparency < 255) {
		if (transparency + 800 * dt > 255)
			transparency = 255;
		else
			transparency += 800 * dt;
		buttontex.setColor(Color(255, 255, 255, (int)transparency));
		splash.setFillColor(Color(splash.getFillColor().r, splash.getFillColor().g, splash.getFillColor().b, (int)transparency));
		splash.setOutlineColor(Color(splash.getOutlineColor().r, splash.getOutlineColor().g, splash.getOutlineColor().b, (int)transparency));
		logo.setColor(Color(255, 255, 255, (int)transparency));
	}
}

bool MainMenuState::fade_out()
{
	if (transparency > 0) {
		if (transparency - 1500 * dt < 0)
			transparency = 0;
		else
			transparency -= 1500 * dt;

		buttontex.setColor(Color(255, 255, 255, (int)transparency));
		splash.setFillColor(Color(splash.getFillColor().r, splash.getFillColor().g, splash.getFillColor().b, (int)transparency));
		splash.setOutlineColor(Color(splash.getOutlineColor().r, splash.getOutlineColor().g, splash.getOutlineColor().b, (int)transparency));
		logo.setColor(Color(255, 255, 255, (int)transparency));

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
	text.setCharacterSize(int(25.69f * scale));
	for (int i = 0; i < 3; i++) {
		buttontex.setTextureRect(IntRect(0, buttons[i].pressed * 49, 190, 49));
		buttontex.setPosition(x+buttons[i].x*scale, y+buttons[i].y * scale);
		text.setString(buttons[i].txt);
		FloatRect bounds=text.getLocalBounds();
		text.setOrigin(bounds.width / 2.f, bounds.top+bounds.height / 2.f);
		text.setPosition(x + buttons[i].x * scale, (buttons[i].pressed) ? y + buttons[i].y * scale + 2 * scale : y + buttons[i].y * scale - 2 * scale);
		if (buttons[i].hover)text.setFillColor(Color(255, 255, 255, (int)transparency));
		else text.setFillColor(Color(226,211,195, (int)transparency));
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
	logo.setOrigin(logo.getLocalBounds().width / 2, logo.getLocalBounds().height / 2);

	splash.setFont(font);
	splash.setString("Civilization Rebooted");
	splash.setRotation(345.f);
	splash.setFillColor(Color((Uint8)average_color.x, (Uint8)average_color.y, (Uint8)average_color.z));
	splash.setOutlineColor(Color((Uint8)average_outline_color.x, (Uint8)average_outline_color.y, (Uint8)average_outline_color.z));

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
		float win_x = (float)window->getSize().x, win_y = (float)window->getSize().y;
		x = win_x / 2.f, y = win_y / 2.f;
		if (win_x / 390.f < win_y / 390.f) scale = win_x / 390.f;
		else scale = win_y / 390.f;
		
		if (VideoMode::getDesktopMode().width < win_x * 1.7 || VideoMode::getDesktopMode().height < win_y * 1.7) {
			scale *= 0.8f;
			logo.setPosition((float)x, y - 135 * scale);
			splash.setPosition((float)x + 60 * scale, y - 115 * scale);
		}
		else {
			logo.setPosition((float)x, y - 110.f * scale);
			splash.setPosition(x + 60 * scale, y - 90 * scale);
		}

		logo.setScale(scale * 0.8f, scale * 0.8f);
		splash.setCharacterSize(int(40.f * scale));
		splash.setOutlineThickness(1.5f * scale);
	}


	update_buttons();

	if (splash_increment > 0 && splash_scale + splash_increment * dt > 1.03f) {
		splash_increment = -splash_increment;
	}
	else if (splash_increment < 0 && splash_scale + splash_increment * dt < 0.98f) {
		splash_increment = -splash_increment;
	}

	splash_scale += splash_increment * dt;

	splash.setScale(splash_scale, splash_scale);
	splash.setOrigin(splash.getLocalBounds().left + splash.getLocalBounds().width / 2.f, splash.getLocalBounds().top + splash.getLocalBounds().height / 2.f);

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
	window->draw(splash);
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
