#include "WorldMapState.h"


void WorldMapState::update_pins()
{
	namebox.setPosition(x + test.x * scale, y + (test.y - 90) * scale);
	pin.setPosition(x + test.x * scale,y + test.y * scale);

	if (pin.getGlobalBounds().contains(mouse_pos) || namebox.getGlobalBounds().contains(mouse_pos)) {
		if (Mouse::isButtonPressed(Mouse::Left) && (namebox.getGlobalBounds().contains(clicked_on)|| pin.getGlobalBounds().contains(clicked_on))) {
			pin.setScale(scale * 2.9, scale * 2.9);
			namebox.setScale(scale * 1.44, scale * 1.44);
			text_size = 50 * scale;
			test.pressed = 1;
		}
		else {
			pin.setScale(scale * 3.1, scale * 3.1);
			namebox.setScale(scale * 1.5, scale * 1.5);
			text_size = 54 * scale;
		}
		namebox.setTextureRect(IntRect(0, 55, 150, 55));
		pin.setTextureRect(IntRect(21, 0, 21, 22));
	}
	else {
		if (test.pressed) {
			test.pressed = 0;

		}
		pin.setScale(scale * 3.1, scale * 3.1);
		namebox.setScale(scale * 1.5, scale * 1.5);
		namebox.setTextureRect(IntRect(0, 0, 150, 55));
		pin.setTextureRect(IntRect(0, 0, 21, 22));
		text_size = 54 * scale;
	}
}

void WorldMapState::render_pins()
{

	pin.setPosition(x + test.x * scale, y + test.y * scale);
	namebox.setPosition(x + test.x * scale, y + (test.y - 90) * scale);
	window->draw(pin);
	window->draw(namebox);
	draw_text(test.name, x + test.x * scale, y + (test.y - 90) * scale, text_size);

}

WorldMapState::WorldMapState(bool admin)
{
	this->admin = admin;

	initial_textures("worldmap");

	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));

	worldmap.setTexture(*textures[0]);
	worldmap.setOrigin(2150/2, 1150/2);

	namebox.setTexture(*textures[1]);
	namebox.setTextureRect(IntRect(0, 0, 150, 55));
	namebox.setOrigin(150 / 2, 55 / 2);

	pin.setTexture(*textures[2]);
	pin.setTextureRect(IntRect(0, 0, 21, 22));
	pin.setOrigin(21 / 2, 22 / 2);
}

WorldMapState::~WorldMapState()
{
}

void WorldMapState::update()
{
	mouse_pos = window->mapPixelToCoords(Mouse::getPosition(*window));

	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 2350.0 < win_y / 1350.0) scale = win_x / 2350.0;
		else scale = win_y / 1350.0;
		////////////////////

		tint.setSize({ win_x, win_y });
		worldmap.setPosition(x, y);
		worldmap.setScale(scale, scale);
		pin.setScale(scale * 3.1, scale * 3.1);
		namebox.setScale(scale*1.5, scale*1.5);
	}
	update_pins();
}

void WorldMapState::render()
{
	window->draw(tint);
	window->draw(worldmap);
	render_pins();
}

void WorldMapState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			window->close(); break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				states->insert(PauseST); break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
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
