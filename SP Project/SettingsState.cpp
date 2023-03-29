#include "SettingsState.h"

SettingsState::SettingsState()
{
	initial_fps();
	initial_textures("settings");
	tissue.setTexture(*textures.at(0));
	tissue.setTextureRect(buttons[3]);
	tissue.setOrigin(92 / 2, 92 / 2);
	tint.setSize({1920, 1080});
	tint.setFillColor(Color(0,0,0,154));
	
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

	if (fps_active)
		calc_fps(dt);
}



void SettingsState::render(RenderWindow* window)
{
	window->draw(tint);
	window->draw(tissue);
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
