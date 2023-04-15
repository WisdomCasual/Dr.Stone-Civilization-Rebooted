#include "WorldMapState.h"


void WorldMapState::update_pins()
{
	pin.setScale(scale * 3.1, scale * 3.1);
	namebox.setScale(scale * 1.5, scale * 1.5);

	for (auto &pn : pins) {
		namebox.setPosition(x + pn.second.x * scale, y + (pn.second.y - 90) * scale);
		pin.setPosition(x + pn.second.x * scale, y + pn.second.y * scale);

		pn.second.pin_scale = scale * 3.1;
		pn.second.namebox_scale = scale * 1.5;
		pn.second.text_size = 54 * scale;
		pn.second.hover = 0;

		if ((pin.getGlobalBounds().contains(mouse_pos) || namebox.getGlobalBounds().contains(mouse_pos)) && !selected && !new_map) {
			if (Mouse::isButtonPressed(Mouse::Left) && (namebox.getGlobalBounds().contains(clicked_on) || pin.getGlobalBounds().contains(clicked_on))) {

				pn.second.pin_scale = scale * 2.9;
				pn.second.namebox_scale = scale * 1.44;
				pn.second.text_size = 50 * scale;
				pn.second.pressed = 1;
			}
			else if (pn.second.pressed) {
				pn.second.pressed = 0;
				if (admin) {
					//choose map in MapBuilder

					states->insert({ 6, new MapBuilderState(pn.first) });

					int exceptions[] = { MapBuilderID };
					game.erase_states(exceptions, 1);
					return;
				}
				else {
					//choosed map in game///////////////////////////////////////////////////


				}
			}
			else {
				pn.second.text_size = 54 * scale;
			}
			pn.second.hover = 1;
			if (move && !selected) {
				moving = pn.first;
				selected = 1;
			}
			if (del) {
				string file_name = "Maps/" + pn.first + ".mp";
				remove(file_name.c_str());
				pins.erase(pn.first);
				del = 0; 
				continue;
			}
		}
		else
			pn.second.pressed = 0;
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
				ofs << pn->first << "| " << pn->second.x << " " << pn->second.y;
				if (pn != mp_end)
					ofs << "\n";
			}
		}
	}
	ofs.close();
}

void WorldMapState::load_maps()
{
	pins.clear();

	ifstream ifs("Maps/maps.ini");
	if (ifs.is_open()) {
		string mp_name; int a, b;
		while (!ifs.eof()) {
			getline(ifs, mp_name, '|');
			ifs >> a >> b;
			ifs.ignore();
			pins.insert({ mp_name , {a,b} });
		}
	}
	ifs.close();
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
		////////////////////

		tint.setSize({ win_x, win_y });
		worldmap.setPosition(x, y);
		worldmap.setScale(scale, scale);
		pin.setScale(scale * 3.1, scale * 3.1);
		namebox.setScale(scale*1.5, scale*1.5);
	}

	if (move && selected) {
		pins[moving] = { (int)((mouse_pos.x - x) / scale) ,(int)((mouse_pos.y - y) / scale) , (float)2.5 * scale , (float)1.2 * scale, 40 * scale };
	}
	else
		move = 0;

	if (new_map) {
		pins[""] = { (int)((mouse_pos.x - x) / scale) ,(int)((mouse_pos.y - y) / scale) , (float) 2.5 * scale , (float) 1.2 * scale, 40 * scale };
		map_added = 1;
	}
	else if (map_added) {

	}
	 del = 0;
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
				states->insert(PauseST); return; break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F5:
				load_maps(); break;
			case Keyboard::F6:
				save_maps(); break;
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
				clicked_on = window->mapPixelToCoords(Mouse::getPosition(*window));
				break;
			}
		}
	}
}
