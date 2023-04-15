#include "NewSaveState.h"



void NewSaveState::update_arrow()
{
	back_arrow.setPosition(x - 28  * scale, y - 26 * scale);
	if (back_arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		back_arrow.setTextureRect(IntRect(22, 0, 22, 21));
		back_arrow.setScale(scale * 0.3, scale * 0.3);
		if (Mouse::isButtonPressed(Mouse::Left) && back_arrow.getGlobalBounds().contains(clicked_on)) {
			arrow_pressed = 1;
			back_arrow.setTextureRect(IntRect(44, 0, 22, 21));
			back_arrow.setScale(scale * 0.27, scale * 0.27);
		}
		else {
			if (arrow_pressed) {
				arrow_pressed = 0;
				states->insert(SavesST);
				states->at(SavesID)->update();
				
				if (states->find(BackgroundID) == states->end())
					states->insert(BackgroundST);

				int exceptions[] = { SavesID , BackgroundID };
				game.erase_states(exceptions, 2);
				destruct = 1;
				return;
			}
		}

	}
	else {
		arrow_pressed = 0;
		back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
		back_arrow.setScale(scale * 0.3, scale * 0.3);
	}
}

NewSaveState::NewSaveState()
{
	State::initial_textures("newsave");

	tissue.setTexture(*textures[0]);
	tissue.setOrigin(700 / 2, 700 / 2);

	panel.setTexture(*textures[6]);
	panel.setOrigin(600 / 2, 600 / 2);

	back_arrow.setTexture(*textures[1]);
	back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
	back_arrow.setOrigin(22 / 2, 21 / 2);

	black_box.setSize(Vector2f(window->getSize().x, window->getSize().y));
	black_box.setPosition(0.0, 0.0);
	black_box.setFillColor(Color(0, 0, 0, 100));

	win_x = window->getSize().x, win_y = window->getSize().y;
	scale = min(win_x / 1920.0, win_y / 1080.0);
	txt_box.initializeTextBox(test_str, *textures[2], "Enter name", Vector2f(win_x / 2.0, (win_y / 2.5)), scale * 1.2);
}

NewSaveState::~NewSaveState()
{
}

void NewSaveState::update()
{
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 120.0 < win_y / 120.0) scale = win_x / 120.0;
		else scale = win_y / 120.0;
		////////////////////

		tissue.setScale(scale * 0.1, scale * 0.1); 
		tissue.setPosition(x, y);
		panel.setScale(scale * 0.13, scale * 0.13);
		panel.setPosition(x, y);
		tint.setSize({ win_x, win_y });
	}
	tissue.setPosition(x, y);

	update_arrow();

	if (destruct)
		return;

	txt_box.update();
}

void NewSaveState::render()
{
	window->draw(black_box);
	window->draw(panel);
	window->draw(tissue);
	txt_box.drawTextBox(window);
	window->draw(back_arrow);
	draw_text("Choose name and", x, y - 28 * scale, 6.5 * scale);
	draw_text("character", x, y - 28 * scale+(5*scale), 6.5 * scale);
}

void NewSaveState::pollevent()
{
	while (window->pollEvent(event)) {
		txt_box.text_poll(event);
		switch (event.type) {
		case Event::Closed:
			window->close(); break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				states->insert(SavesST);
				states->at(SavesID)->update();

				if (states->find(BackgroundID) == states->end())
					states->insert(BackgroundST);
				{
					int exceptions[] = { SavesID , BackgroundID };
					game.erase_states(exceptions, 2);
					return; 
				}
				break;
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
