#include "SettingsState.h"

void SettingsState::update_arrow(RenderWindow* window, int* terminator, deque<State*>* states)
{
	back_arrow.setPosition(x - 35 * scale, y - 35 * scale);
	if (back_arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		back_arrow.setTextureRect(IntRect(22, 0, 22, 21));
		back_arrow.setScale(scale * 0.3, scale * 0.3);
		if (Mouse::isButtonPressed(Mouse::Left)) {
			arrow_pressed = 1;
			back_arrow.setTextureRect(IntRect(44, 0, 22, 21));
			back_arrow.setScale(scale * 0.27, scale * 0.27);
		}
		else {
			if (arrow_pressed) {
				arrow_pressed = 0;
				*terminator += 2;
				states->push_front(new MainMenuState);
				states->push_front(new Background);
			}
		}

	}
	else {
		arrow_pressed = 0;
		back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
		back_arrow.setScale(scale * 0.3, scale * 0.3);
	}
}

SettingsState::SettingsState()
{
	initial_fps();
	initial_textures("settings");
	tissue.setTexture(*textures.at(0));
	tissue.setTextureRect(buttons[3]);
	tissue.setOrigin(92 / 2, 92 / 2);
	tint.setSize({1920, 1080});
	tint.setFillColor(Color(0,0,0,154));

	back_arrow.setTexture(*textures[1]);
	back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
	back_arrow.setOrigin(22 / 2, 21 / 2);
}

SettingsState::~SettingsState()
{

}

void SettingsState::update(float dt, RenderWindow* window, int* terminator, deque<State*>* states)
{
	win_x = window->getSize().x, win_y = window->getSize().y;
	x = win_x / 2, y = win_y / 2;
	if (win_x / 120.0 < win_y / 120.0) scale = win_x / 120.0;
	else scale = win_y / 120.0;
	/////// Do Not touch;

	tint.setSize({ win_x, win_y });

	tissue.setPosition(x, y);
	tissue.setScale(scale, scale);


	update_arrow(window, terminator, states);
	if (fps_active)
		calc_fps(dt);
}



void SettingsState::render(RenderWindow* window)
{
	window->draw(tint);
	window->draw(tissue);
	window->draw(back_arrow);
	if (fps_active)
		window->draw(fps_text);
}

void SettingsState::pollevent(Event event, RenderWindow* window)
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			window->close(); break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				window->close(); break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			}
		}
	}

}
