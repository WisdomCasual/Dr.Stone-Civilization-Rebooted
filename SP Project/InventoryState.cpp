#include "InventoryState.h"


void InventoryState::render_items()
{
	nod* it = iteration_start;

	if (inventory_order->empty()) {
		draw_text("Empty", panel.getPosition().x, panel.getPosition().y + 30 * scale, 50 * scale);
	}
	else{
		if (inventory_order->size < 10) {
			for (int i = -(inventory_order->size - 1) / 2.0 * 75 + 30, c = 0; it != NULL && i < 750; i += 75, c++) {

				item.setPosition(panel.getPosition().x - 160 * scale, panel.getPosition().y + i * scale);
				item.setTextureRect(IntRect(it->itm * 16, 0, 16, 16));
				window->draw(item);

				draw_text(item_names[it->itm], panel.getPosition().x - (healing_value[it->itm] ? 20 * scale : 0), panel.getPosition().y + i * scale, 45 * scale);
				draw_text("x" + to_string(inventory_count[it->itm]), panel.getPosition().x + (healing_value[it->itm] ? 120 : 140) * scale, panel.getPosition().y + i * scale, 45 * scale);

				buttons[c].y = panel.getPosition().y + i * scale;
				buttons[c].item_id = it->itm;

				it = it->link;
			}
		}
		else {
			if (item_offset) {
				item.setPosition(panel.getPosition().x - 160 * scale, (205 + scroll_offset) * scale);
				item.setTextureRect(IntRect(it->back_link->itm * 16, 0, 16, 16));
				window->draw(item);

				draw_text(item_names[it->back_link->itm], panel.getPosition().x - (healing_value[it->itm] ? 20 * scale : 0), (205 + scroll_offset) * scale, 45 * scale);
				draw_text("x" + to_string(inventory_count[it->back_link->itm]), panel.getPosition().x + (healing_value[it->itm] ? 120 : 140) * scale, (205 + scroll_offset) * scale, 45 * scale);

				buttons[item_offset - 1].y = (205 + scroll_offset) * scale;
				buttons[item_offset - 1].item_id = it->back_link->itm;
			}

			for (int i = 0, c = item_offset; it != NULL && i < 750; i += 75, c++) {

				item.setPosition(panel.getPosition().x - 160 * scale, (280 + i + scroll_offset) * scale);
				item.setTextureRect(IntRect(it->itm * 16, 0, 16, 16));
				window->draw(item);

				draw_text(item_names[it->itm], panel.getPosition().x - (healing_value[it->itm] ? 20 * scale : 0), (280 + i + scroll_offset) * scale, 45 * scale);
				draw_text("x" + to_string(inventory_count[it->itm]), panel.getPosition().x + (healing_value[it->itm] ? 120 : 140) * scale, (280 + i + scroll_offset) * scale, 45 * scale);

				buttons[c].y = (280 + i + scroll_offset) * scale;
				buttons[c].item_id = it->itm;

				it = it->link;
			}
		}
	}
}

void InventoryState::update_buttons()
{
	for (int i = (item_offset? item_offset - 1 : item_offset); i < inventory_order->size && i < item_offset + 10; i++) {
		if (healing_value[buttons[i].item_id]) {
			consumebutton.setTextureRect(IntRect(buttons[i].pressed * 45, 0, 45, 49));
			consumebutton.setPosition(panel.getPosition().x + 180 * scale, buttons[i].y);
			if (consumebutton.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window))) && Mouse::getPosition(*window).y > panel.getPosition().y - 320 * scale && Mouse::getPosition(*window).y < panel.getPosition().y + 400 * scale) {
				clickable_cursor = true;
				if (Mouse::isButtonPressed(Mouse::Left) && consumebutton.getGlobalBounds().contains(clicked_on)) {
					if (!buttons[i].pressed)
						game.play_sfx(0);
					buttons[i].pressed = 1;
				}
				else {
					if (buttons[i].pressed) {
						inventory_count[buttons[i].item_id]--;
							if (*player_health + player_max_health / 5.f * healing_value[buttons[i].item_id] < player_max_health)
								*player_health += player_max_health / 5.f * healing_value[buttons[i].item_id];
							else
								*player_health = player_max_health;
						if (!inventory_count[buttons[i].item_id]) {
							inventory_order->erase(buttons[i].item_id);
							if (i <= item_offset)
								iteration_start = iteration_start->link;
						}
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
}

void InventoryState::render_buttons()
{
	consumebutton.setScale(scale, scale);
	text.setCharacterSize(25.69 * scale);
	for (int i = (item_offset ? item_offset - 1 : item_offset); i < inventory_order->size && i < item_offset + 10; i++) {
		if (healing_value[buttons[i].item_id]) {
			consumebutton.setTextureRect(IntRect(buttons[i].pressed * 45, 0, 45, 49));
			consumebutton.setPosition(panel.getPosition().x + 180 * scale, buttons[i].y);


			if (buttons[i].hover) text.setFillColor(Color(255, 255, 255));
			else text.setFillColor(Color(226, 211, 195));
			window->draw(consumebutton);

			if (buttons[i].hover)
				heart.setColor(Color(255, 255, 255));
			else
				heart.setColor(Color(200, 200, 200));

			if (healing_value[buttons[i].item_id] <= 0.5)
				heart.setTextureRect(IntRect(0, 0, 22, 20));
			else
				heart.setTextureRect(IntRect(22, 0, 22, 20));

			heart.setPosition(panel.getPosition().x + 180 * scale, (buttons[i].pressed) ? buttons[i].y + 2 * scale : buttons[i].y - 2 * scale);
			window->draw(heart);
		}
	}
}

InventoryState::InventoryState(in_order* inventory_order, unsigned short* inventory_count, short* player_health, short player_max_health, float* combat_tag)
{
	this->inventory_count = inventory_count;
	this->inventory_order = inventory_order;
	this->player_health = player_health;
	this->player_max_health = player_max_health;
	this->combat_tag = combat_tag;

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

	consumebutton.setTexture(*textures[3]);
	consumebutton.setTextureRect(IntRect(0, 0, 45, 49));
	consumebutton.setOrigin(45 / 2, 49 / 2);

	heart.setTexture(*textures[4]);
	heart.setTextureRect(IntRect(0, 0, 22, 20));
	heart.setOrigin(11, 10);

	velocity = 1500;

	if(inventory_order->size)
		buttons = new button[inventory_order->size];

	iteration_start = inventory_order->first;
}

InventoryState::~InventoryState()
{
	active_cursor = false;

	if (buttons != nullptr)
		delete[] buttons;
}

void InventoryState::update()
{
	active_cursor = true;

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

		consumebutton.setScale(scale, scale);
		heart.setScale(scale * 1.25f, scale * 1.25f);
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
	if (*combat_tag <= 0)
		update_buttons();
	else {
		consumebutton.setColor(Color(155, 155, 155));
		heart.setColor(Color(100, 100, 100));
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
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case Mouse::Left:
				// clicked on is a vector2f that stores the position of the mouse when the left mouse button is clicked
				clicked_on = window->mapPixelToCoords(Mouse::getPosition(*window));
				break;
			}
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

	render_buttons();

	window->draw(panel);

	text.setFillColor(Color::Black);
	draw_text("Science Backpack", panel_pos * scale * 1.5, y - 375 * scale, 48 * scale);
}
