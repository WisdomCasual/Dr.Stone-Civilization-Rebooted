#include "InventoryState.h"


InventoryState::InventoryState()
{
	initial_textures("inventory");


	panel.setTexture(*textures[0]);
	panel.setOrigin(panel.getLocalBounds().width / 2, panel.getLocalBounds().height / 2);
	panel_pos = -panel.getLocalBounds().width / 2;

	tissue.setTexture(*textures[1]);
	tissue.setOrigin(tissue.getLocalBounds().width / 2, tissue.getLocalBounds().height / 2);

	item.setTexture(*textures[2]);
	item.setOrigin(8, 8);

	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 0));

	velocity = 1000;
}

InventoryState::~InventoryState()
{

}

void InventoryState::update()
{
	window->setMouseCursorVisible(false);

	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 1920.0 > win_y / 1080.0) scale = win_x / 1920.0;
		else scale = win_y / 1080.0;

		/////////////////////

		tint.setSize({ win_x, win_y });

		panel.setPosition(panel_pos * scale * 1.5, y);
		panel.setScale(scale * 1.5, scale * 1.5);

		tissue.setPosition(panel_pos * scale * 1.5, y);
		tissue.setScale(scale * 1.5, scale * 1.5);

	}
	if (close) {
		if (panel_pos > -panel.getLocalBounds().width / 2) {
			panel_pos -= velocity * dt;
			velocity += 1000 * dt;
			panel.setPosition(panel_pos * scale * 1.5, y);
			tissue.setPosition(panel_pos * scale * 1.5, y);
			if (darkness > 0) {
				if (darkness - 154 * dt * 4 < 0)
					darkness = 0;
				else
					darkness -= 154 * dt * 4;
				tint.setFillColor(Color(0, 0, 0, darkness));
			}
		}
		else {
			delete states->at(InventoryID);
			states->erase(InventoryID);
			return;
		}
	}
	else if (panel_pos < panel.getLocalBounds().width / 2 + 12) {
		if (panel_pos + velocity * dt > panel.getLocalBounds().width / 2 + 12)
			panel_pos = panel.getLocalBounds().width / 2 + 12;
		else
			panel_pos += velocity * dt;
		velocity -= 500 * dt;
		panel.setPosition(panel_pos * scale * 1.5, y);
		tissue.setPosition(panel_pos * scale * 1.5, y);
		if (darkness < 154) {
			if (darkness + 154 * dt * 4 > 154)
				darkness = 154;
			else
				darkness += 154 * dt * 4;
			tint.setFillColor(Color(0, 0, 0, darkness));
		}
	}

}

void InventoryState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
			case Keyboard::E:
				velocity = 700;
				close = true; break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			}
		case Event::MouseWheelMoved:
			if (event.type == Event::MouseWheelMoved) {


			}
			break;
		}
	}
}

void InventoryState::render()
{
	window->draw(tint);
	window->draw(tissue);

	////////////

	window->draw(panel);

	text.setFillColor(Color::Black);
	draw_text("Science Backpack", panel_pos * scale * 1.5, y - 375 * scale, 48 * scale);
}
