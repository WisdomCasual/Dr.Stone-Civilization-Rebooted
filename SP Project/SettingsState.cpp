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

void SettingsState::dev_button(RenderWindow* window, int* terminator, deque<State*>* states)
{
	if (scale != previous_scale) {
		previous_scale = scale;
		devbutton.setScale(scale * 0.2, scale * 0.2);
		devbutton.setPosition(x + 35 * scale, y + 35 * scale);
		devtext.setCharacterSize(4.45 * scale);
		devtext.setOrigin(devtext.getLocalBounds().width / 2.0, devtext.getLocalBounds().top + devtext.getLocalBounds().height / 2.0);
	}
	devtext.setPosition(x + 35 * scale, y + 35 * scale - 2 * 0.2 * scale);
	devbutton.setTextureRect(IntRect(0, 0, 45, 49));
	if (devbutton.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		if (Mouse::isButtonPressed(Mouse::Left)) {
			button_pressed = 1;
			devtext.setPosition(x + 35 * scale, y + 35.4 * scale);
			devbutton.setTextureRect(IntRect(45, 0, 45, 49));
		}
		else {
			if (button_pressed) {
				button_pressed = 0;
				*terminator += 2;
				states->push_front(new MapBuilderState);
			}
			devbutton.setTextureRect(IntRect(0, 0, 45, 49));
			devtext.setPosition(x + 35 * scale, y + 34.6 * scale);
		}
		devtext.setFillColor(Color::White);
	}
	else {
		button_pressed = 0;
		devtext.setFillColor(Color(200, 200, 200));
	}
}


void SettingsState::update_slider(RenderWindow* window, int color, int pos_x, int pos_y, string name)
{
	tip.setTextureRect(tipsleft[color]);
	tip.setTextureRect(mids[color]);
	tip.setTextureRect(tipsright[color]);
	
	tip.setOrigin(tip.getLocalBounds().width / 2.0, tip.getLocalBounds().width / 2.0);

	if (Mouse::isButtonPressed(Mouse::Left)) {
		if (tip.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			presssed = 1;
		}
	}
	else
		presssed = 0;
	if(presssed)
		tip.setPosition( round(max(min(mouse_pos.x, (int)(x + 20 * scale)), (int)(x - 20 * scale))/64.0)  * 64, y);
	
tip.setScale(3, 3);

	tip.setTexture(*textures[0]);
	window->draw(tip);
}

void SettingsState::render_slider(RenderWindow* window)
{
	tip.setPosition(x, y);

}

SettingsState::SettingsState()
{
	initial_fps();
	initial_textures("settings");
	tissue.setTexture(*textures.at(0));
	tissue.setTextureRect(buttons[3]);
	tissue.setOrigin(92 / 2, 92 / 2);
	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));

	back_arrow.setTexture(*textures[1]);
	back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
	back_arrow.setOrigin(22 / 2, 21 / 2);

	devbutton.setTexture(*textures[2]);
	devbutton.setTextureRect(IntRect(0, 0, 45, 49));
	devbutton.setOrigin(45 / 2, 49 / 2);

	devtext.setFont(font);
	devtext.setString("Dev");
	devtext.setCharacterSize(40);
	devtext.setFillColor(Color(200, 200, 200));

	tip.setTexture(*textures[0]);

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
	mouse_pos = Mouse::getPosition(*window);
	/////// Do Not touch;

	tint.setSize({ win_x, win_y });

	tissue.setPosition(x, y);
	tissue.setScale(scale, scale);

	dev_button(window, terminator, states);
	update_arrow(window, terminator, states);
	if (fps_active)
		calc_fps(dt);


	update_slider(window, 0, x, y, "Slider1");
}

void SettingsState::render(RenderWindow* window)
{
	window->draw(tint);
	window->draw(tissue);
	window->draw(devbutton);
	window->draw(devtext);
	window->draw(back_arrow);

	text.setFillColor(Color::Black);
	draw_text(window, "Settings", x, y - 35 * scale, 6.5 * scale);



	window->draw(tip); ///////delete, just for testing


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