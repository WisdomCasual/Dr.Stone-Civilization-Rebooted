#include "WorldMapState.h"


void WorldMapState::fade_in()
{
	if (transparency < 255) {
		if (transparency + 1500 * dt > 255)
			transparency = 255;
		else
			transparency += 1500 * dt;

		worldmap.setColor(Color(255, 255, 255, transparency));
		namebox.setColor(Color(255, 255, 255, transparency));
		pin.setColor(Color(255, 255, 255, transparency));

		if (tint_fade && darkness < 154) {
			if (darkness + 154 * dt * 6 > 154)
				darkness = 154;
			else
				darkness += 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, darkness));
		}
	}
}

bool WorldMapState::fade_out()
{
	if (transparency > 0) {
		if (transparency - 1500 * dt < 0)
			transparency = 0;
		else
			transparency -= 1500 * dt;

		worldmap.setColor(Color(255, 255, 255, transparency));
		namebox.setColor(Color(255, 255, 255, transparency));
		pin.setColor(Color(255, 255, 255, transparency));

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

void WorldMapState::update_pins()
{
	pin.setScale(scale * 3.1, scale * 3.1);
	namebox.setScale(scale * 1.5, scale * 1.5);

	for (auto pn = pins.begin(); pn != pins.end(); pn++) {
		namebox.setPosition(x + pn->second.x * scale, y + (pn->second.y - 90) * scale);
		pin.setPosition(x + pn->second.x * scale, y + pn->second.y * scale);

		pn->second.pin_scale = scale * 3.1;
		pn->second.namebox_scale = scale * 1.5;
		pn->second.text_size = 54 * scale;
		pn->second.hover = 0;

		if ((pin.getGlobalBounds().contains(mouse_pos) || namebox.getGlobalBounds().contains(mouse_pos)) && !selected && !new_map) {
			if (Mouse::isButtonPressed(Mouse::Left) && (namebox.getGlobalBounds().contains(clicked_on) || pin.getGlobalBounds().contains(clicked_on))) {

				pn->second.pin_scale = scale * 2.9;
				pn->second.namebox_scale = scale * 1.44;
				pn->second.text_size = 50 * scale;
				pn->second.pressed = 1;
			}
			else if (pn->second.pressed) {
				pn->second.pressed = 0;
				if (admin) {
					//choose map in MapBuilder
					save_maps();
					states->insert({ MapBuilderID, new MapBuilderState(pn->first, pn->second.x_size, pn->second.y_size) });
					states->at(MapBuilderID)->update();

					int exceptions[] = { MapBuilderID };
					game.erase_states(exceptions, 1);
					destruct = 1;
					return;
				}
				else {
					//choosed map in game///////////////////////////////////////////////////


				}
			}
			else {
				pn->second.text_size = 54 * scale;
			}
			pn->second.hover = 1;
			if (move && !selected) {
				moving = pn->first;
				selected = 1;
			}
			if (del) {
				del = 0;
				map_to_delete = pn->first;
				string strings_array[] = { "Are you sure that you", "want to delete", "\"" + map_to_delete + "\""};
				states->insert({ ConfirmationID, new ConfirmationState(strings_array, 3, delete_map) });
				states->at(ConfirmationID)->update();
			}
		}
		else
			pn->second.pressed = 0;
	}
}

void WorldMapState::render_pins()
{
	for (auto &pn : pins) {

		pin.setPosition(x + pn.second.x * scale, y + pn.second.y * scale);
		pin.setTextureRect(IntRect((int)pn.second.hover * 21, 0, 21, 22));
		pin.setScale(pn.second.pin_scale, pn.second.pin_scale);
		namebox.setPosition(x + pn.second.x * scale, y + (pn.second.y - 90) * scale);
		namebox.setTextureRect(IntRect(0, (int)pn.second.hover * 55, 150, 55));
		namebox.setScale(pn.second.namebox_scale, pn.second.namebox_scale);
		
		window->draw(pin);
		window->draw(namebox);
		text.setFillColor(Color(0, 0, 0, transparency));
		draw_text(pn.first, x + pn.second.x * scale, y + (pn.second.y - 90) * scale, pn.second.text_size);
	}

}

void WorldMapState::save_maps()
{
	ofstream ofs("Maps/maps.ini", ofstream::out, ofstream::trunc);
	auto mp_end = --pins.end();
	if (ofs.is_open()) {
		for (auto pn = pins.begin(); pn != pins.end(); pn++) {
			if (pn->first != "") {
				ofs << pn->first << "| " << pn->second.x << " " << pn->second.y << " " << pn->second.x_size << " " << pn->second.y_size;
				if (pn != mp_end)
					ofs << "\n";
			}
		}
	}
	ofs.close();
}

WorldMapState::WorldMapState(bool admin, bool tint_fade)
{
	this->tint_fade = tint_fade;
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

	load_maps();
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

		if (win_x > 1280) scale *= 0.9;

		////////////////////

		tint.setSize({ win_x, win_y });
		worldmap.setPosition(x, y);
		worldmap.setScale(scale, scale);
		pin.setScale(scale * 3.1, scale * 3.1);
		namebox.setScale(scale*1.5, scale*1.5);
	}

	fade_in();

	update_pins();

	if (destruct)
		return;

	if (move && selected) {
		pins[moving] = { (int)((mouse_pos.x - x) / scale) ,(int)((mouse_pos.y - y) / scale) , pins[moving].x_size, pins[moving].y_size, (float)2.5 * scale , (float)1.2 * scale, 40 * scale };
	}
	else
		move = 0;

	if (new_map) {
		pins[""] = { (int)((mouse_pos.x - x) / scale) ,(int)((mouse_pos.y - y) / scale) , 0, 0, (float) 2.5 * scale , (float) 1.2 * scale, 40 * scale };
		map_added = 1;
	}
	else if (map_added) {
		map_added = 0;
		states->insert(NewMapST);
		states->at(NewMapID)->update();
	}
	 del = 0;

	 if (loadmap) { loadmap = 0;  load_maps(); }

	 if (delete_map) {
		 delete_map = 0;
		 string file_name = "Maps/" + map_to_delete + ".mp";
		 remove(file_name.c_str());
		 pins.erase(map_to_delete);
	 }
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
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				states->insert(PauseST);
				states->at(PauseID)->update();
				return; break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F5:
			{
				string strings_array[] = { "Are you sure that you", "want to revert to ", "the last saved world map?", "" , "Any changes will be lost" };
				states->insert({ ConfirmationID, new ConfirmationState(strings_array,5, loadmap) });
				states->at(ConfirmationID)->update();
			} break;
			case Keyboard::F6:
				save_maps(); 
				{
					string notification_s[] = { "Saved Successfully" };
					game.notification(notification_s, 1);
				}
				break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			case Keyboard::N:
				if (admin)
					new_map = !new_map; break;
			case Keyboard::M:
				if (admin)
					move = !move; selected = 0; break;
			case Keyboard::Delete:
				if (admin)
					del = 1; break;
			}
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case Mouse::Left:
				if (!move && !new_map)
					clicked_on = window->mapPixelToCoords(Mouse::getPosition(*window));
				else
					clicked_on = { -100,-100 };
				move = 0; selected = 0, new_map = 0;

				break;
			}
		}
	}
}
