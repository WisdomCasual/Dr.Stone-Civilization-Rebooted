#include "TradingState.h"
#include"Game.h"

void TradingState::fade_in()
{
	if (transparency < 255) {
		if (transparency + 1500 * dt > 255)
			transparency = 255;
		else
			transparency += 1500 * dt;

		panel.setColor(Color(255, 255, 255, transparency));
		tissue.setColor(Color(255, 255, 255, transparency));
		items.setColor(Color(255, 255, 255, transparency));
		get_button.setColor(Color(255, 255, 255, transparency));
		pic.setColor(Color(255, 255, 255, transparency));

		if (darkness < 154) {
			if (darkness + 154 * dt * 6 > 154)
				darkness = 154;
			else
				darkness += 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, darkness));
		}
	}
}

bool TradingState::fade_out()
{
	if (transparency > 0) {
		if (transparency - 1500 * dt < 0)
			transparency = 0;
		else
			transparency -= 1500 * dt;

		panel.setColor(Color(255, 255, 255, transparency));
		tissue.setColor(Color(255, 255, 255, transparency));
		items.setColor(Color(255, 255, 255, transparency));
		get_button.setColor(Color(255, 255, 255, transparency));
		pic.setColor(Color(255, 255, 255, transparency));

		if (darkness > 0) {
			if (darkness - 154 * dt * 6 < 0)
				darkness = 0;
			else
				darkness -= 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, darkness));
		}
		return false;
	}
	else
		return true;
}

void TradingState::update_buttons()
{
	for (int i = 0; i < trading_info.trades_count; i++) {
		if (!buttons[i].disabled) {
			get_button.setTextureRect(IntRect(buttons[i].pressed * 45, 0, 45, 49));
			get_button.setPosition(x + 425 * scale, buttons[i].y);

			if (get_button.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window))) && in_bounds(Mouse::getPosition(*window).y)) {
				clickable_cursor = true;
				if (Mouse::isButtonPressed(Mouse::Left) && get_button.getGlobalBounds().contains(clicked_on)) {
					if (!buttons[i].pressed)
						game.play_sfx(0);
					buttons[i].pressed = 1;
				}
				else {
					if (buttons[i].pressed) {
						for (int j = 0; j < trading_info.trading_menu[i].ingredients_count; j++) {
							inventory_count[trading_info.trading_menu[i].ingredients[j].ID] -= trading_info.trading_menu[i].ingredients[j].count;
							if (!inventory_count[trading_info.trading_menu[i].ingredients[j].ID])
								inventory_order->erase(trading_info.trading_menu[i].ingredients[j].ID);
						}
						if (!inventory_count[trading_info.trading_menu[i].result.ID])
							inventory_order->add(trading_info.trading_menu[i].result.ID);
						inventory_count[trading_info.trading_menu[i].result.ID]+= trading_info.trading_menu[i].result.count;
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

void TradingState::render_buttons()
{
	for (int i = 0; i < trading_info.trades_count; i++) {
		if (!in_bounds(buttons[i].y))
			continue;
		button_text.setFillColor(Color(226, 211, 195, transparency));
		if (!buttons[i].disabled) {
			get_button.setColor(Color(255, 255, 255, transparency));

			if (buttons[i].hover)
				button_text.setFillColor(Color(255, 255, 255, transparency));
		}
		else {
			get_button.setColor(Color(155, 155, 155, transparency));
			button_text.setFillColor(Color(126, 111, 95, transparency));
		}
		button_text.setPosition(x + 425 * scale, (buttons[i].pressed) ? buttons[i].y + 2 * scale : buttons[i].y - 2 * scale);
		get_button.setTextureRect(IntRect(buttons[i].pressed * 45, 0, 45, 49));
		get_button.setPosition(x + 425 * scale, buttons[i].y);
		window->draw(get_button);
		window->draw(button_text);
	}
}

void TradingState::render_trades()
{
	float x_stride = 100.f;
	
	if (trading_info.trades_count < 4) {
		for (int i = 0; i < trading_info.trades_count; i++) {
			buttons[i].disabled = false;
			float x_start = x + 190 * scale - trading_info.trading_menu[i].ingredients_count / 2.f * x_stride * scale;
			float y_pos = y - (trading_info.trades_count - 1) * 100 * scale + i * 200 * scale + 25 * scale;
			buttons[i].y = y_pos;

			text.setFillColor(Color(0, 0, 0, transparency));
			draw_text(trading_info.trading_menu[i].item_name, x + 240 * scale, y_pos - 80 * scale, 50 * scale);

			items.setTextureRect(IntRect(trading_info.trading_menu[i].result.ID * 16, 0, 16, 16));
			items.setPosition(x_start + trading_info.trading_menu[i].ingredients_count * x_stride * scale, y_pos - 25 * scale);
			window->draw(items);
			draw_text(to_string(trading_info.trading_menu[i].result.count), x_start + trading_info.trading_menu[i].ingredients_count * x_stride * scale, y_pos + 25 * scale, 45 * scale);

			for (int j = 0; j < trading_info.trading_menu[i].ingredients_count; j++) {

				items.setTextureRect(IntRect(trading_info.trading_menu[i].ingredients[j].ID * 16, 0, 16, 16));
				items.setPosition(x_start + j * x_stride * scale, y_pos - 25 * scale);
				window->draw(items);

				draw_text((j == trading_info.trading_menu[i].ingredients_count - 1) ? "=" : "+", x_start + (j + 0.5f) * x_stride * scale, y_pos, 55 * scale);

				if (inventory_count[trading_info.trading_menu[i].ingredients[j].ID] < trading_info.trading_menu[i].ingredients[j].count) {
					text.setFillColor(Color(164, 0, 0, transparency));
					buttons[i].disabled = true;
				}
				draw_text(to_string(inventory_count[trading_info.trading_menu[i].ingredients[j].ID]) + '/' + to_string(trading_info.trading_menu[i].ingredients[j].count), x_start + j * x_stride * scale, y_pos + 25 * scale, 45 * scale);
			}
		}
	}
	else {
		for (int i = 0; i < trading_info.trades_count; i++) {
			float y_pos = y - (225 - i * 200 - scroll_offset) * scale;
			buttons[i].disabled = false;
			float x_start = x + 190 * scale - trading_info.trading_menu[i].ingredients_count / 2.f * x_stride * scale;
			buttons[i].y = y_pos;

			text.setFillColor(Color(0, 0, 0, transparency));
			if (in_bounds(y_pos - 80 * scale))
				draw_text(trading_info.trading_menu[i].item_name, x + 240 * scale, y_pos - 80 * scale, 50 * scale);

			if (in_bounds(y_pos - 25 * scale)) {
				items.setTextureRect(IntRect(trading_info.trading_menu[i].result.ID * 16, 0, 16, 16));
				items.setPosition(x_start + trading_info.trading_menu[i].ingredients_count * x_stride * scale, y_pos - 25 * scale);
				window->draw(items);
			}
			if (in_bounds(y_pos + 25 * scale))
				draw_text(to_string(trading_info.trading_menu[i].result.count), x_start + trading_info.trading_menu[i].ingredients_count * x_stride * scale, y_pos + 25 * scale, 45 * scale);


			for (int j = 0; j < trading_info.trading_menu[i].ingredients_count; j++) {

				if (in_bounds(y_pos - 25 * scale)) {
					items.setTextureRect(IntRect(trading_info.trading_menu[i].ingredients[j].ID * 16, 0, 16, 16));
					items.setPosition(x_start + j * x_stride * scale, y_pos - 25 * scale);
					window->draw(items);
				}
				if (in_bounds(y_pos))
					draw_text((j == trading_info.trading_menu[i].ingredients_count - 1) ? "=" : "+", x_start + (j + 0.5f) * x_stride * scale, y_pos, 55 * scale);

				if (inventory_count[trading_info.trading_menu[i].ingredients[j].ID] < trading_info.trading_menu[i].ingredients[j].count) {
					text.setFillColor(Color(164, 0, 0, transparency));
					buttons[i].disabled = true;
				}
				if (in_bounds(y_pos + 25 * scale))
					draw_text(to_string(inventory_count[trading_info.trading_menu[i].ingredients[j].ID]) + '/' + to_string(trading_info.trading_menu[i].ingredients[j].count), x_start + j * x_stride * scale, y_pos + 25 * scale, 45 * scale);
			}
		}
	}
}

bool TradingState::in_bounds(float y_pos)
{
	if (y_pos >= y - panel.getGlobalBounds().height / 2.f + 44 * scale && y_pos <= y + panel.getGlobalBounds().height / 2.f - 44 * scale)
		return true;
	return false;
}

void TradingState::set_dialogue_lines()
{
	text.setCharacterSize(55 * scale);
	text.setOrigin(0, 0);
	text.setPosition(0, 0);
	for (auto c : trading_info.dialogue) {
		if (c == ' ') {
			if (output_strings[lines - 1].size())
				text.setString(output_strings[lines - 1] + " " + next_word);
			else
				text.setString(next_word);

			if (text.findCharacterPos(output_strings[lines - 1].size() - 1).x < 340 * scale)
				output_strings[lines - 1] += " " + next_word;
			else {
				lines++;
				output_strings[lines - 1] += next_word;
			}

			next_word.clear();
		}
		else
			next_word += c;
	}
}

TradingState::TradingState(in_order* inventory_order, unsigned short* inventory_count, TradingInfo& trading_info)
	: trading_info(trading_info)
{
	this->inventory_count = inventory_count;
	this->inventory_order = inventory_order;

	initial_textures("trading");

	panel.setTexture(*textures[0]);
	panel.setOrigin(panel.getLocalBounds().width / 2, panel.getLocalBounds().height / 2);

	tissue.setTexture(*textures[1]);
	tissue.setOrigin(tissue.getLocalBounds().width / 2, tissue.getLocalBounds().height / 2);

	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));

	items.setTexture(*textures[2]);
	items.setOrigin(8, 8);

	get_button.setTexture(*textures[3]);
	get_button.setTextureRect(IntRect(0, 0, 45, 49));
	get_button.setOrigin(45 / 2, 49 / 2);

	button_text.setFont(font);
	button_text.setString("Get");

	pic_tex.loadFromFile("textures/dialogue/" + to_string(trading_info.NPC_ID) + ".png");
	pic.setTexture(pic_tex);
	pic.setTextureRect({ 0, 0, 320, 300 });
	pic.setOrigin(320 / 2.f, 300 / 2.f);

	buttons = new button[trading_info.trades_count];

	set_dialogue_lines();
}

TradingState::~TradingState()
{
	if (buttons != nullptr)
		delete[] buttons;
}

void TradingState::update()
{
	active_cursor = true;
	mouse_pos = window->mapPixelToCoords(Mouse::getPosition(*window));

	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 1920.0 > win_y / 1080.0) scale = win_x / 1920.0;
		else scale = win_y / 1080.0;

		///////////////////////////////////////////////////////////////////////

		tint.setSize({ win_x, win_y });

		panel.setPosition(x, y);
		panel.setScale(scale * 1.4, scale * 1.4);

		tissue.setPosition(x, y);
		tissue.setScale(scale * 1.4, scale * 1.4);

		get_button.setScale(scale * 1.4, scale * 1.4);
		button_text.setCharacterSize(32 * scale);
		button_text.setOrigin(button_text.getLocalBounds().left + button_text.getLocalBounds().width / 2.f, button_text.getLocalBounds().top + button_text.getLocalBounds().height / 2.f);

		items.setScale(scale * 3, scale * 3);

		pic.setScale(scale / 1.035f, scale / 1.035f);
		pic.setPosition(x - 328.f * scale, y - 203.5f * scale);
	}

	if (exit) {
		if (fade_out()) {
			exit = false;
			delete states->at(TradingID);
			states->erase(TradingID);
			return;
		}
	}
	else
		fade_in();

	update_buttons();
}

void TradingState::render()
{
	window->draw(tint);
	window->draw(tissue);
	/////////////////////////////

	render_trades();
	render_buttons();
	window->draw(pic);

	/////////////////////////////
	window->draw(panel);

	text.setFillColor(Color(0, 0, 0, transparency));

	float y_start = y + (175 - (lines - 1) * 25.f) * scale;

	for (int i = 0; i < lines; i++)
		draw_text(output_strings[i], x - 277 * scale, y_start + i * 50.f * scale, 55 * scale);

	text.setFillColor(Color(255, 255, 255, transparency));
	draw_text("Press ESC to exit", x, y + panel.getGlobalBounds().height / 2.f + 50.f * scale, 55 * scale);

}

void TradingState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				exit = true;
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
			break;
		case Event::MouseWheelMoved:
			if (event.type == Event::MouseWheelMoved) {

				if (trading_info.trades_count > 3) {
					scroll_offset += 50 * event.mouseWheel.delta;
					if (scroll_offset > 0)
						scroll_offset = 0;
					else if (scroll_offset < trading_info.trades_count * -200 + 700)
						scroll_offset = trading_info.trades_count * -200 + 700;
				}
			}
			break;
		}
	}
}