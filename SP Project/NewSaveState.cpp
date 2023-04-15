#include "NewSaveState.h"

NewSaveState::NewSaveState()
{
	State::initial_textures("newsave");
	black_box.setSize(Vector2f(window->getSize().x, window->getSize().y));
	black_box.setPosition(0.0, 0.0);
	black_box.setFillColor(Color(0, 0, 0, 100));
	win_x = window->getSize().x, win_y = window->getSize().y;
	scale = min(win_x / 1920.0, win_y / 1080.0);
	txt_box.initializeTextBox(test_str, *textures[2], "Write Here", Vector2f(win_x/2.0, win_y / 2.0), scale);
}

NewSaveState::~NewSaveState()
{
}

void NewSaveState::update()
{
	txt_box.update();
}

void NewSaveState::render()
{
	window->draw(black_box);
	txt_box.drawTextBox(window);
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
					delete states->rbegin()->second;
					states->erase(states->rbegin()->first);
					return;
					break;
				case Keyboard::F3:
					fps_active = !fps_active; break;
				case Keyboard::F11:
					fullscreen = !fullscreen;
					game.update_window();
					break;
				}
		}
	}
}
