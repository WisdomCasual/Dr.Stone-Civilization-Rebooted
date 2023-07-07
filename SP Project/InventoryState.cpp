#include "InventoryState.h"


void InventoryState::render_items()
{
	nod* it = iteration_start;

	if (inventory_order->empty()) {
		draw_text("Empty", panel.getPosition().x, panel.getPosition().y + 30 * scale, 50 * scale);
	}
	else{
		if (inventory_order->size < 10) {
			for (int i = -(inventory_order->size - 1) / 2.0 * 75 + 30; it != NULL && i < 750; i += 75) {

				item.setPosition(panel.getPosition().x - 130 * scale, panel.getPosition().y + i * scale);
				item.setTextureRect(IntRect(it->itm * 16, 0, 16, 16));
				window->draw(item);

				draw_text(item_names[it->itm], panel.getPosition().x, panel.getPosition().y + i * scale, 45 * scale);
				draw_text("x" + to_string(inventory_count[it->itm]), panel.getPosition().x + 130 * scale, panel.getPosition().y + i * scale, 45 * scale);

				it = it->link;
			}
		}
		else {
			if (item_offset) {
				item.setPosition(panel.getPosition().x - 130 * scale, (205 + scroll_offset) * scale);
				item.setTextureRect(IntRect(it->itm * 16, 0, 16, 16));
				window->draw(item);

				draw_text(item_names[it->itm], panel.getPosition().x, (205 + scroll_offset) * scale, 45 * scale);
				draw_text("x" + to_string(inventory_count[it->itm]), panel.getPosition().x + 130 * scale, (205 + scroll_offset) * scale, 45 * scale);
			}

			for (int i = 0; it != NULL && i < 750; i += 75) {

				item.setPosition(panel.getPosition().x - 130 * scale, (280 + i + scroll_offset) * scale);
				item.setTextureRect(IntRect(it->itm * 16, 0, 16, 16));
				window->draw(item);

				draw_text(item_names[it->itm], panel.getPosition().x, (280 + i + scroll_offset) * scale, 45 * scale);
				draw_text("x" + to_string(inventory_count[it->itm]), panel.getPosition().x + 130 * scale, (280 + i + scroll_offset) * scale, 45 * scale);

				it = it->link;
			}
		}
	}
}

InventoryState::InventoryState(in_order* inventory_order, unsigned short* inventory_count)
{
	this->inventory_count = inventory_count;
	this->inventory_order = inventory_order;

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

	velocity = 1500;

	iteration_start = inventory_order->first;
}

InventoryState::~InventoryState()
{

}

void InventoryState::update()
{
	active_cursor = false;

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

		item.setScale(scale * 3, scale * 3);
	}
	if (close) {
		if (panel_pos > -panel.getLocalBounds().width / 2) {
			panel_pos -= velocity * dt;
			velocity += 1500 * dt;
			panel.setPosition(panel_pos * scale * 1.5, y);
			tissue.setPosition(panel_pos * scale * 1.5, y);
			if (darkness > 0) {
				if (darkness - 154 * dt * 6 < 0)
					darkness = 0;
				else
					darkness -= 154 * dt * 6;
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
		velocity -= 1000 * dt;
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
			case Keyboard::Z:
				inventory_count[0] = 0;
				if (!inventory_count[0]) {
					inventory_order->erase(0);
					iteration_start = inventory_order->first;
				}
				break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			}
			break;
		case Event::MouseWheelMoved:
			if (event.type == Event::MouseWheelMoved) {

				if (inventory_order->size > 9) {
					scroll_offset += 20 * event.mouseWheel.delta;

					if (!item_offset && scroll_offset > 0)
						scroll_offset = 0;
					else if (inventory_order->size - item_offset < 10 && scroll_offset < 0)
						scroll_offset = 0;
					else {
						if (scroll_offset > 0) {
							while (item_offset && scroll_offset > 0) {
								iteration_start = iteration_start->back_link;
								scroll_offset -= 60;
								item_offset--;
							}
						}
						else if (scroll_offset <= -60) {
							while (inventory_order->size - item_offset >= 10 && scroll_offset <= -60) {
								iteration_start = iteration_start->link;
								scroll_offset += 60;
								item_offset++;
							}
						}
					}
				}
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
	render_items();

	window->draw(panel);

	text.setFillColor(Color::Black);
	draw_text("Science Backpack", panel_pos * scale * 1.5, y - 375 * scale, 48 * scale);
}
