#include "MapBuilderState.h"

MapBuilderState::MapBuilderState(string map_name, int a, int b) : size_x(a), size_y(b)
{	
	this->map_name = map_name;
	load_map();

	//loads "game" textures
	initial_textures("game");
	info.setFont(font);
	layer_info.setFont(font);
	layer_info.setPosition(25 * x_scale, 800 * y_scale);
	//hover rectangle
	hover_rect.setFillColor(Color::Transparent);
	hover_rect.setOutlineThickness(1);
	hover_rect.setOutlineColor(Color::White);
	//hitbox rectangle
	hitbox_rect.setSize({ 16, 16 });
	hitbox_rect.setFillColor(Color(175, 0, 0, 80));
	//selection rectangle
	select_rect.setSize(Vector2f(16, 16));
	select_rect.setFillColor(Color::Transparent);
	select_rect.setOutlineThickness(3);
	select_rect.setOutlineColor(Color::Green);

	srand(time(0));
}

void MapBuilderState::update_info_text()
{
	info.setCharacterSize(40 * text_scale);
	//displays picked tile properties
	info.setString("\n        Selected Tile ID " + to_string(picked_tile.x) + " " + to_string(picked_tile.y) + " - spritesheet ID " + to_string(picked_tile.tex_id) + " - Selected Tile: " + to_string(selected_tile.x) + " " + to_string(selected_tile.y)
		+ "\n        Blocked (b) " + to_string(blocked) + " - hitbox (h) " + to_string(hitbox) + " - brush size (+/-): " + to_string(brush_size) + " - spread chance (alt +/-): " + to_string(spread_chances[spread_chance]) + "^-1"
		+ "\n        Layer (1,2,3,4) " + to_string(layer+1) + " - Render Priority: " + Render_in[Render_Priority]);

	//displays layer properties
	layer_info.setCharacterSize(40 * text_scale);
	layer_info.setString("\t\t Back \tFront\nlayer 1:\t" + to_string(layer_toggle[0]) + "\t\t" + to_string(layer_toggle[4]) +
		"\nlayer 2:\t" + to_string(layer_toggle[1]) + "\t\t" + to_string(layer_toggle[5]) + 
		"\nlayer 3:\t" + to_string(layer_toggle[2]) + "\t\t" + to_string(layer_toggle[6]) + 
		"\nlayer 4:\t" + to_string(layer_toggle[3]) + "\t\t" + to_string(layer_toggle[7]) );
}

void MapBuilderState::dash_cam()
{
	//moves the Camera, increase cam screen with "Left Ctrl"
	cam_speed = 350;
	if (Keyboard::isKeyPressed(Keyboard::LControl))
		cam_speed = 1000;
	Vector2f movement = delta_movement();
	x -= dt * cam_speed * movement.x * scale;
	y -= dt * cam_speed * movement.y * scale;
	x_offset = -x / (scale * 16);
	y_offset = -y / (scale * 16);
}

void MapBuilderState::hover()
{
	if(picked_tile.select_done)
		hover_rect.setSize(Vector2f(picked_tile.wdth * 16, picked_tile.hght * 16));
	else if(picked_tile.global_select_done)
		hover_rect.setSize(Vector2f(wdth * 16, hght * 16));
	else
		hover_rect.setSize(Vector2f( 16*brush_size, 16*brush_size ));
	hover_rect.setPosition(hover_tile);
	hover_rect.setScale(scale, scale);
}

MapBuilderState::~MapBuilderState()
{
	if (!picker)
		delete this->tex_picker;
	
}

void MapBuilderState::grid(int x_win, int y_win)
{
	//draws grid lines, "G" to activate/deactivate
	if (active_grid) {
		RectangleShape grid_rect;
		grid_rect.setSize(Vector2f(2, y_win));
		grid_rect.setFillColor(Color::Black);
		for (int i = 0; i < x_win - x % (int)(scale * 16); i += 16 * scale) {
			grid_rect.setPosition(i + x % (int)(scale * 16), 0);
			window->draw(grid_rect);
		}
		grid_rect.setSize(Vector2f(x_win, 2));
		for (int i = 0; i < y_win - y % (int)(scale * 16); i += 16 * scale) {
			grid_rect.setPosition(0, i + y % (int)(scale * 16));
			window->draw(grid_rect);
		}
	}
}

void MapBuilderState::render_tiles(int x_win, int y_win, int priority)
{
	
	//renders the map in active screen area only
	Sprite tile;
	hitbox_rect.setScale(scale, scale);
	tile.setScale(scale, scale);
	for (int i = (x_offset > 0) ? x_offset : 0; i < (x_win + ((x_offset + 1) * 16 * scale)) / (16 * scale) && i < size_x; i++)
		for (int j = (y_offset > 0) ? y_offset : 0; j < (y_win + ((y_offset + 1) * 16 * scale)) / (16 * scale) && j < size_y; j++) {
			for (auto props : tiles[i][j].layer[priority]) {
				if (layer_toggle[props.first + 4 * priority]) {
					tile.setTexture(*textures[props.second.z]);
					tile.setTextureRect(IntRect(props.second.x * 16, props.second.y * 16, 16, 16));
					tile.setPosition(x + (16 * scale * i), y + (16 * scale * j));
					window->draw(tile);
				}
			}
			if ((fps_active || hitbox) && priority && tiles[i][j].hitbox) {
				hitbox_rect.setPosition(x + (16 * scale * i), y + (16 * scale * j));
				window->draw(hitbox_rect);
			}
		}
	grid(x_win, y_win);
	if (display_text) {
		tile.setTextureRect(IntRect(picked_tile.x * 16, picked_tile.y * 16, 16, 16));
		tile.setPosition(15, 50); tile.setScale(4, 4);	tile.setTexture(*textures[picked_tile.tex_id]);
		window->draw(tile);
	}
}

void MapBuilderState::update()
{
	if (prev_win != window->getSize()) {
		x_scale = window->getSize().x / 1920.0, y_scale = window->getSize().y / 1080.0;
		global_scale = x_scale, text_scale = y_scale;
		if (global_scale < text_scale)
			swap(global_scale, text_scale);
		scale = round(10 * global_scale);
		prev_win = window->getSize();
		layer_info.setPosition(25 * x_scale, 800 * y_scale);
	}

	mouse_cords();

	dash_cam();

    update_info_text();

	hover();

	selection();

	draw_tools();

	erase_tools();

	undo();

	if (!picker)
		tex_picker->run(picker);
}

void MapBuilderState::selection()
{
	if (selected_tile.x < 0)
		selected_tile.x = 0;
	else if (selected_tile.x >= size_x)
		selected_tile.x = size_x - 1;
	if (selected_tile.y < 0)
		selected_tile.y = 0;
	else if (selected_tile.y >= size_y)
		selected_tile.y = size_y - 1;

	if (Keyboard::isKeyPressed(Keyboard::LShift) && window->hasFocus()) {
		if (!selecting) {
			selection_start.x = selected_tile.x;
			selection_start.y = selected_tile.y;
		}
		selecting = 1;
		picked_tile.global_select_done = 0;
	}
	else {
		if (selecting) {
			picked_tile.global_select_done = 1, picked_tile.global_priority = Render_Priority, picked_tile.global_layer = layer;
			selection_end = selected_tile;
		}
		selecting = 0;
	}


	if (selecting) {
		start_x = min(selection_start.x, selected_tile.x), start_y = min(selection_start.y, selected_tile.y);

		if (start_x < 0) start_x = 0; 
		if (start_y < 0) start_y = 0;

		select_rect.setPosition(x+start_x * 16 * scale, y+start_y * 16 * scale);

		wdth = abs(selection_start.x - selected_tile.x) + 1, hght = abs(selection_start.y - selected_tile.y) + 1;


		select_rect.setSize(Vector2f(wdth * 16 * scale, hght * 16 * scale));
		picked_tile.select_done = 0;
	}
	if (picked_tile.global_select_done) {
		select_rect.setSize(Vector2f(wdth * 16 * scale, hght * 16 * scale));
		select_rect.setPosition(x + start_x * 16 * scale, y + start_y * 16 * scale);
	}

}

void MapBuilderState::draw_tools()
{
	if (Mouse::isButtonPressed(Mouse::Left) && selected_tile.x >= 0 && selected_tile.x < size_x && selected_tile.y >= 0 && selected_tile.y < size_y && window->hasFocus()) {
		if (selected_tile != picked_tile.previous_drawn_tile) {
			picked_tile.previous_drawn_tile = selected_tile;
			picked_tile.previous_erased_tile = { -1, -1 };
			if (!undid_changes.empty())
				undid_changes.clear();
			if (!hitbox) {
				if (picked_tile.select_done) {
					if (!drawn_selection) {
						//store changed area info
						changes.push_back(change{ { selected_tile.x , selected_tile.y }, { selected_tile.x + picked_tile.wdth, selected_tile.y + picked_tile.hght } });

						for (int i1 = picked_tile.start_x, i2 = selected_tile.x; i1 < picked_tile.start_x + picked_tile.wdth && i2 < size_x; i1++, i2++)
							for (int j1 = picked_tile.start_y, j2 = selected_tile.y; j1 < picked_tile.start_y + picked_tile.hght && j2 < size_y; j1++, j2++) {
								changes.back().tiles.push_back(tiles[i2][j2]); //<--store tiles before changes
								tiles[i2][j2].layer[Render_Priority][layer] = { i1, j1, picked_tile.tex_id};
							}
						drawn_selection = 1;
					}
				}
				else if (picked_tile.global_select_done) {

					if (!drawn_map_selection) {
						//store changed area info
						changes.push_back(change{ { selected_tile.x , selected_tile.y }, { selected_tile.x + wdth, selected_tile.y + hght } });

						for (int i1 = start_x, i2 = selected_tile.x; i1 < start_x + wdth && i2 < size_x && i1 < size_x; i1++, i2++)
							for (int j1 = start_y, j2 = selected_tile.y; j1 < start_y + hght && j2 < size_y && j1 < size_x; j1++, j2++) {

									changes.back().tiles.push_back(tiles[i2][j2]); //<--store tiles before changes
									if (Keyboard::isKeyPressed(Keyboard::LAlt))
										tiles[i2][j2] = tiles[i1][j1];
									else
										tiles[i2][j2].layer[Render_Priority][layer] = { tiles[i1][j1].layer[picked_tile.global_priority][picked_tile.global_layer].x, tiles[i1][j1].layer[picked_tile.global_priority][picked_tile.global_layer].y, tiles[i1][j1].layer[picked_tile.global_priority][picked_tile.global_layer].z };
							}
						drawn_map_selection = 1;
					}
				}
				else {
					//store changed area info
					changes.push_back(change{ { selected_tile.x , selected_tile.y }, { selected_tile.x + brush_size, selected_tile.y + brush_size } });

					if (!(Keyboard::isKeyPressed(Keyboard::LAlt) && brush_size > 1)) {
						for (int i = selected_tile.x; i < selected_tile.x + brush_size && i < size_x; i++)
							for (int j = selected_tile.y; j < selected_tile.y + brush_size && i < size_y; j++) {
								changes.back().tiles.push_back(tiles[i][j]); //<--store tiles before changes

								tiles[i][j].layer[Render_Priority][layer] = { picked_tile.x, picked_tile.y, picked_tile.tex_id };
							}
					}
					else {
						for (int i = selected_tile.x; i < selected_tile.x + brush_size && i < size_x; i++) {
							for (int j = selected_tile.y; j < selected_tile.y + brush_size && i < size_y; j++) {
								rand_spray = rand() % spread_chances[spread_chance];
								changes.back().tiles.push_back(tiles[i][j]); //<--store tiles before changes
								if (!rand_spray) {
									tiles[i][j].layer[Render_Priority][layer] = { picked_tile.x, picked_tile.y, picked_tile.tex_id };

								}
							}
						}
					}
				}
			}
			else {
				if (!picked_tile.select_done) {
					//store changed area info
					changes.push_back(change{ { selected_tile.x , selected_tile.y }, { selected_tile.x + brush_size, selected_tile.y + brush_size } });

					for (int i = selected_tile.x; i < selected_tile.x + brush_size && i < size_x; i++)
						for (int j = selected_tile.y; j < selected_tile.y + brush_size && i < size_y; j++) {

							changes.back().tiles.push_back(tiles[i][j]); //<--store tiles before changes

							tiles[i][j].hitbox = 1;
						}
				}
				else {
					//store changed area info
					changes.push_back(change{ { selected_tile.x , selected_tile.y }, { selected_tile.x + picked_tile.wdth, selected_tile.y + picked_tile.hght } });

					for (int i1 = picked_tile.start_x, i2 = selected_tile.x; i1 < picked_tile.start_x + picked_tile.wdth; i1++, i2++)
						for (int j1 = picked_tile.start_y, j2 = selected_tile.y; j1 < picked_tile.start_y + picked_tile.hght; j1++, j2++) {

							changes.back().tiles.push_back(tiles[i2][j2]); //<--store tiles before changes

							tiles[i2][j2].hitbox = 1;
						}
				}
			}
			tiles[selected_tile.x][selected_tile.y].blocked = blocked;
		}
	}
	else { drawn_selection = 0; drawn_map_selection = 0; }
}

void MapBuilderState::erase_tools()
{
	if (Mouse::isButtonPressed(Mouse::Right) && selected_tile.x >= 0 && selected_tile.x < size_x && selected_tile.y >= 0 && selected_tile.y < size_y && window->hasFocus()) {
		if (selected_tile != picked_tile.previous_erased_tile) {
			picked_tile.previous_erased_tile = selected_tile;
			picked_tile.previous_drawn_tile = { -1, -1 };
			if (!undid_changes.empty())
				undid_changes.clear();
			if (!hitbox) {
				if (picked_tile.select_done) {
					//store changed area info
					changes.push_back(change{ { selected_tile.x , selected_tile.y }, { selected_tile.x + picked_tile.wdth, selected_tile.y + picked_tile.hght } });

					for (int i1 = picked_tile.start_x, i2 = selected_tile.x; i1 < picked_tile.start_x + picked_tile.wdth; i1++, i2++)
						for (int j1 = picked_tile.start_y, j2 = selected_tile.y; j1 < picked_tile.start_y + picked_tile.hght; j1++, j2++) {

							changes.back().tiles.push_back(tiles[i2][j2]); //<--store tiles before changes

							if (Keyboard::isKeyPressed(Keyboard::LAlt))
								for (int f = 0; f < 2; f++)
									tiles[i2][j2].layer[f].clear();
							else
								tiles[i2][j2].layer[Render_Priority].erase(layer);
						}
				}
				else {
					//store changed area info
					changes.push_back(change{ { selected_tile.x , selected_tile.y }, { selected_tile.x + brush_size, selected_tile.y + brush_size } });

					for (int i = selected_tile.x; i < selected_tile.x + brush_size && i < size_x; i++)
						for (int j = selected_tile.y; j < selected_tile.y + brush_size && i < size_y; j++) {

							changes.back().tiles.push_back(tiles[i][j]); //<--store tiles before changes

							if (Keyboard::isKeyPressed(Keyboard::LAlt))
								for (int f = 0; f < 2; f++)
									tiles[i][j].layer[f].clear();
							else
								tiles[i][j].layer[Render_Priority].erase(layer);
						}
				}
			}
			else {
				if (!picked_tile.select_done) {
					//store changed area info
					changes.push_back(change{ { selected_tile.x , selected_tile.y }, { selected_tile.x + brush_size, selected_tile.y + brush_size } });

					for (int i = selected_tile.x; i < selected_tile.x + brush_size && i < size_x; i++)
						for (int j = selected_tile.y; j < selected_tile.y + brush_size && i < size_y; j++) {

							changes.back().tiles.push_back(tiles[i][j]); //<--store tiles before changes

							tiles[i][j].hitbox = 0;
						}
				}
				else {
					//store changed area info
					changes.push_back(change{ { selected_tile.x , selected_tile.y }, { selected_tile.x + picked_tile.wdth, selected_tile.y + picked_tile.hght } });

					for (int i1 = picked_tile.start_x, i2 = selected_tile.x; i1 < picked_tile.start_x + picked_tile.wdth; i1++, i2++)
						for (int j1 = picked_tile.start_y, j2 = selected_tile.y; j1 < picked_tile.start_y + picked_tile.hght; j1++, j2++) {

							changes.back().tiles.push_back(tiles[i2][j2]); //<--store tiles before changes

							tiles[i2][j2].hitbox = 0;
						}
				}
			}
			picked_tile.global_select_done = 0;
			tiles[selected_tile.x][selected_tile.y].blocked = 0;
		}
	}
}

void MapBuilderState::mouse_cords()
{
	mouse_pos = window->mapPixelToCoords(Mouse::getPosition(*window));
	relative_mouse_pos = mouse_pos;
	relative_mouse_pos.x -= x % int(16 * scale), relative_mouse_pos.y -= y % int(16 * scale);
	if (mouse_pos.x > 0 && mouse_pos.x < window->getSize().x && mouse_pos.y > 0 && mouse_pos.y < window->getSize().y) {
		hover_tile = { int(relative_mouse_pos.x / scale / 16) * 16 * scale + x % int(16 * scale), int(relative_mouse_pos.y / scale / 16) * 16 * scale + y % int(16 * scale) };
	}
	selected_tile = { int((mouse_pos.x - x) / scale / 16), int((mouse_pos.y - y) / scale / 16) };
}

void MapBuilderState::save_map()
{
	ofstream ofs("Maps/" + map_name + ".mp", ofstream::out, ofstream::trunc);
	if (ofs.is_open()) {
		ofs << size_x << ' ' << size_y << '\n';
		for (int i = 0; i < size_x; i++) {
			for (int j = 0; j < size_y; j++) {
				for (int k = 0; k < 2; k++) {
					ofs << tiles[i][j].layer[k].size() <<' ';
					for (auto curr_tile : tiles[i][j].layer[k])
						ofs << (int)(curr_tile.first) <<' ' << curr_tile.second.x << ' ' << curr_tile.second.y << ' ' << curr_tile.second.z <<' ';
					ofs << tiles[i][j].hitbox << ' ' << tiles[i][j].blocked <<' ';
				}
			}
			ofs << '\n';
		}
		ofs.close();
	}
}

void MapBuilderState::load_map()
{
	ifstream ifs("Maps/" + map_name + ".mp");
	string line;
	int mpsize = 0;
	pair<char, Vector3i> tle;
	if (!(ifs >> line)) {
		for (int i = 0; i < size_x; i++) {
			for (int j = 0; j < size_y; j++) {
				tiles[i][j].layer[0][0] = { 21,49, 1 };
			}
		}
	}
	ifs.seekg(ios::beg);
	if (ifs.is_open()) {
		ifs >> size_x >> size_y;
		for (int i = 0; i < size_x; i++) {
			for (int j = 0; j < size_y; j++) {
				for (int k = 0; k < 2; k++) {
					tiles[i][j].layer[k].clear();
					ifs >> mpsize;
					while (mpsize--) {
						ifs >> tle.first >> tle.second.x >> tle.second.y >> tle.second.z;
						tle.first -= '0';
						tiles[i][j].layer[k].insert(tle);
					}
					ifs >> tiles[i][j].hitbox >> tiles[i][j].blocked;
				}
			}
		}
	}
	ifs.close();
}

void MapBuilderState::undo()
{
	if (Keyboard::isKeyPressed(Keyboard::LControl) && window->hasFocus())
		ctrl_pressed = 1;
	else
		ctrl_pressed = 0;
	if (Keyboard::isKeyPressed(Keyboard::Z) && window->hasFocus())
		z_pressed = 1;
	else
		z_pressed = 0;
	if (Keyboard::isKeyPressed(Keyboard::Y) && window->hasFocus())
		y_pressed = 1;
	else
		y_pressed = 0;

	if (changes.size() > 25)
		changes.pop_front();
	if (ctrl_pressed && y_pressed) {
		if (!redone) {
			redone = 1;
			if (!undid_changes.empty()) {
				changes.push_back(change{ { undid_changes.back().start.x , undid_changes.back().start.y }, { undid_changes.back().end.x, undid_changes.back().end.y } });
				for (int i = undid_changes.back().start.x, c = 0; i < undid_changes.back().end.x && i < size_x; i++)
					for (int j = undid_changes.back().start.y; j < undid_changes.back().end.y && j < size_y; j++) {\
						changes.back().tiles.push_back(tiles[i][j]);
						tiles[i][j] = undid_changes.back().tiles[c];
						c++;
					}
				undid_changes.pop_back();
			}
		}
	}
	else
		redone = 0;

	if (ctrl_pressed && z_pressed) {
		if (!undid) {
			undid = 1; 
			picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 };
			if (!changes.empty()) {
				undid_changes.push_back(change{ { changes.back().start.x , changes.back().start.y }, { changes.back().end.x, changes.back().end.y } });
				for (int i = changes.back().start.x, c = 0; i < changes.back().end.x && i < size_x; i++)
					for (int j = changes.back().start.y; j < changes.back().end.y && j < size_y; j++) {
						undid_changes.back().tiles.push_back(tiles[i][j]);
						tiles[i][j] = changes.back().tiles[c];
						c++;
					}
				changes.pop_back();
			}
		}
	}
	else
		undid = 0;
}

void MapBuilderState::render()
{
	int x_win = window->getSize().x, y_win = window->getSize().y; //<-- window boundaries
	x_mid = x_offset + (x_win / (16 * scale) / 2), y_mid = y_offset + (y_win / (16 * scale) / 2); // updates offset

	render_tiles(x_win, y_win, 0);
	//----------render entities herre in game--------------
	render_tiles(x_win, y_win, 1);
	window->draw(hover_rect);
	if (display_text) {
		window->draw(info);
		window->draw(layer_info);
	}
	if (selecting || picked_tile.global_select_done) {
		window->draw(select_rect);
	}
}

void MapBuilderState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			window->close(); break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				states->insert(PauseST); return; break;
			case Keyboard::Space:
				x = 0, y = 0; break;
			case Keyboard::F1:
				display_text = !display_text; break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F5:
				//ARE YOU REALLY REALLY REALLY REALLY SURE YOU WANT TO LOAD!!????
				load_map(); break;
			case Keyboard::F6:
				//are you sure?
				save_map(); break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			case Keyboard::E:
				if (picker)
					tex_picker = new CreativeMode(&textures, picked_tile);
				else
					delete this->tex_picker;
				picker = !picker; break;
				//activate deactivate grid lines
			case Keyboard::G:
				active_grid = !active_grid; break;
			case Keyboard::B:
				picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 };
				blocked = !blocked; break;
			case Keyboard::H:
				picked_tile.global_select_done = 0;
				picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 };
				hitbox = !hitbox; break;
			case Keyboard::F:
				picked_tile.global_select_done = 0;
				picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 };
				Render_Priority = !Render_Priority; break;
			case Keyboard::Num1:
				if ((Keyboard::isKeyPressed(Keyboard::LAlt)))
					layer_toggle[0] = !layer_toggle[0];
				else
					picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 }, layer = 0;
				break;
			case Keyboard::Num2:
				if ((Keyboard::isKeyPressed(Keyboard::LAlt)))
					layer_toggle[1] = !layer_toggle[1];
				else
					picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 }, layer = 1;
				break;
			case Keyboard::Num3:
				if ((Keyboard::isKeyPressed(Keyboard::LAlt)))
					layer_toggle[2] = !layer_toggle[2];
				else
					picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 }, layer = 2;
				break;
			case Keyboard::Num4:
				if ((Keyboard::isKeyPressed(Keyboard::LAlt)))
					layer_toggle[3] = !layer_toggle[3];
				else
					picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 }, layer = 3; break;
			case Keyboard::Num5:
				if ((Keyboard::isKeyPressed(Keyboard::LAlt)))
					layer_toggle[4] = !layer_toggle[4];
				else
					picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 }, layer = 3; break;
			case Keyboard::Num6:
				if ((Keyboard::isKeyPressed(Keyboard::LAlt)))
					layer_toggle[5] = !layer_toggle[5];
				else
					picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 }, layer = 3; break;
			case Keyboard::Num7:
				if ((Keyboard::isKeyPressed(Keyboard::LAlt)))
					layer_toggle[6] = !layer_toggle[6];
				else
					picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 }, layer = 3; break;
			case Keyboard::Num8:
				if ((Keyboard::isKeyPressed(Keyboard::LAlt)))
					layer_toggle[7] = !layer_toggle[7];
				else
					picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 }, layer = 3; break;
			case Keyboard::Equal:
				if (!event.key.alt) {
					picked_tile.global_select_done = 0;
					picked_tile.select_done = 0;
					picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 };
					brush_size += (brush_size < 25);
				}
				else {
					if (spread_chance < spread_chances_num-1)
						spread_chance++;
				}
				break;
			case Keyboard::Hyphen:
				if (!event.key.alt) {
					picked_tile.global_select_done = 0;
					picked_tile.select_done = 0;
					picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 };
					brush_size -= (brush_size > 1);
				}
				else {
					if (spread_chance > 0)
						spread_chance--;
				}
				break;
			}
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
				case Mouse::Middle:
					picked_tile.select_done = 0;
					picked_tile.global_select_done = 0;
					picked_tile.previous_drawn_tile = { -1,-1 }, picked_tile.previous_erased_tile = { -1,-1 };
					picked_tile.tex_id = tiles[selected_tile.x][selected_tile.y].layer[Render_Priority][layer].z;
					picked_tile.x = tiles[selected_tile.x][selected_tile.y].layer[Render_Priority][layer].x;
					picked_tile.y = tiles[selected_tile.x][selected_tile.y].layer[Render_Priority][layer].y;
					break;
			}


			//camera zoom
		case Event::MouseWheelMoved:
			if (event.type == Event::MouseWheelMoved)
				if (scale + event.mouseWheel.delta > global_scale && scale + event.mouseWheel.delta < 20*global_scale) {
					if (global_scale < 1)
						scaling_speed = 1.0/speed_list[(int)round(log(int(round(1 / global_scale))))];
					else
						scaling_speed = round(global_scale);
					x -= event.mouseWheel.delta * 16*scaling_speed * x_mid;
					y -= event.mouseWheel.delta * 16*scaling_speed * y_mid;
					scale += event.mouseWheel.delta * scaling_speed;
				} break;
		}
	}

}