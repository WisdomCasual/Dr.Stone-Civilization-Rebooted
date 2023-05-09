#include "GameState.h"


void GameState::search_front(int x, int y, int layr, Vector3i*** temp_front, bool*** vis, int idx)
{
	vis[layr][x][y] = 1;
	for (int i = 0; i < 4; i++) {
		int new_x = x + dx[i], new_y = y + dy[i];
		if (new_x < size_x && new_y < size_y && new_x >= 0 && new_y >= 0 && temp_front[layr][new_x][new_y].x && !vis[layr][new_x][new_y]) {
			temp_front[layr][new_x][new_y].x--;
			dynamic_map.at[idx].add({Vector2f(new_x, new_y), temp_front[layr][new_x][new_y] });
			search_front(new_x, new_y, layr, temp_front, vis, idx);
		}
	}
}

void GameState::load_map(string map_name)
{
	ifstream ifs("Maps/" + map_name + ".mp");
	string line;
	
	if (!(ifs >> line))
		return;

	short mpsize = 0, count = 0;
	Vector3i tle;
	short layr;
	Vector3i temp_layers[8]{};
	Vector3i** temp_front[4];
	bool** vis[4];
	short layer_prop = 0;

	deload_map();

	
	ifs.seekg(ios::beg);

	if (ifs.is_open()) {

		ifs >> size_x >> size_y;

		for (int i = 0; i < 4; i++) {
			temp_front[i] = new Vector3i* [size_x];
			vis[i] = new bool* [size_x];
		}
		
		static_map = new render_tile* [size_x];

		for (int i = 0; i < size_x; i++) {
			for (int j = 0; j < 4; j++) {
				temp_front[j][i] = new Vector3i[size_y]({});
				vis[j][i] = new bool[size_y]({});
			}
			
			static_map[i] = new render_tile[size_y];

			for (int j = 0; j < size_y; j++) {
				ifs >> mpsize;
				while (mpsize--) {

					ifs >> layr >> tle.x >> tle.y >> tle.z;

					layer_prop = tile_props[tle.z].properties[tle.x][tle.y].props;

					if (layer_prop & 32) {
						static_map[i][j].object_type = tile_props[tle.z].properties[tle.x][tle.y].object_type;
						static_map[i][j].tool_type = tile_props[tle.z].properties[tle.x][tle.y].tool_type;
					}
					if (layer_prop & 16) { // front core
						dynamic_objects objct;
						objct.add({ Vector2f(i, j), tle});
						objct.layer = layr;
						dynamic_map.add(objct);
						dynamic_rendering.insert({ float((j+1) * 16), {short(dynamic_map.curr_idx - 1), nullptr} });
						static_map[i][j].dynamic_idx = dynamic_map.curr_idx - 1;
						//add ptr to set

						//add to dynamic tiles
					}
					else if ((layer_prop & 8)) { //front
						temp_front[layr][i][j] = { tle.x+1, tle.y, tle.z };
					}
					else {   // back
						temp_layers[count] = tle;
						count++;
					}

					static_map[i][j].tile_props |= layer_prop;
				}

				static_map[i][j].size = count;
				static_map[i][j].layers = new Vector3i[count];

				for (int l = 0; l < count; l++) // layers
					static_map[i][j].layers[l] = temp_layers[l];
				count = 0;
			}
		}
	}
	ifs.close();

	for (int i = 0; i < dynamic_map.curr_idx; i++) {
		search_front(dynamic_map.at[i].at[0].position.x, dynamic_map.at[i].at[0].position.y, dynamic_map.at[i].layer, temp_front, vis, i);
	}



	//destroy temp front
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < size_x; j++) {
			delete[] temp_front[i][j];
		}
		if (size_x)
			delete[] temp_front[i];
	}
}

void GameState::load_entities(float player_relative_y_pos)
{

	enemies.add(default_enemy, {800, 800});
	enemies.add(default_enemy, { 850, 850 });

	player_stats.animations = new animation * [5]({});
	player_stats.states_no = 4;
	player_stats.base_movement_speed = 130;
	for (int i = 0; i <= 3; i++) {
		player_stats.animations[i] = new animation[16];
		player_stats.animations[i][0] = { 9, {64, 8 * 65, 64, 65}, {30,14}, {32,48} }; //back
		player_stats.animations[i][1] = { 9, {64, 11 * 65, 64, 65}, {30,14}, {32,48} }; //right
		player_stats.animations[i][2] = { 9, {64, 9 * 65, 64, 65}, {30,14}, {32,48} }; //left
		player_stats.animations[i][3] = { 9, {64, 10 * 65, 64, 65}, {30,14}, {32,48} }; //front
	}
	for (int i = 1; i <= 3; i++) {
		player_stats.animations[2][0 + i * 4] = { 6, {192, 1365 + (0 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //back
		player_stats.animations[2][1 + i * 4] = { 6, {192, 1365 + (3 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //right
		player_stats.animations[2][2 + i * 4] = { 6, {192, 1365 + (1 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //left
		player_stats.animations[2][3 + i * 4] = { 6, {192, 1365 + (2 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //front
	}
	for (int i = 0; i <= 1; i++) {
		player_stats.animations[i][4] = { 5, {128, 1365 + 0 * 128, 128, 128}, {30,14}, {64,70} }; //back
		player_stats.animations[i][5] = { 5, {128, 1365 + 3 * 128, 128, 128}, {30,14}, {64,70} }; //right
		player_stats.animations[i][6] = { 5, {128, 1365 + 1 * 128, 128, 128}, {30,14}, {64,70} }; //left
		player_stats.animations[i][7] = { 5, {128, 1365 + 2 * 128, 128, 128}, {30,14}, {64,70} }; //front
	}
	player_entity.change_state(3);



	enemy_stats.animations = new animation * [4];
	enemy_stats.scale_const = 0.4;
	enemy_stats.states_no = 4;
	enemy_stats.base_movement_speed = 80;
	for (int i = 0; i < 4; i++) {
		enemy_stats.animations[i] = new animation[16];
		enemy_stats.animations[i][0] = { 9, {64, 8 * 65, 64, 65}, {30,14}, {32,48} }; //back
		enemy_stats.animations[i][1] = { 9, {64, 11 * 65, 64, 65}, {30,14}, {32,48} }; //right
		enemy_stats.animations[i][2] = { 9, {64, 9 * 65, 64, 65}, {30,14}, {32,48} }; //left
		enemy_stats.animations[i][3] = { 9, {64, 10 * 65, 64, 65}, {30,14}, {32,48} }; //front
	}
	for (int i = 1; i <= 3; i++) {
		enemy_stats.animations[2][0 + i * 4] = { 6, {192, 1365 + (0 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //back
		enemy_stats.animations[2][1 + i * 4] = { 6, {192, 1365 + (3 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //right
		enemy_stats.animations[2][2 + i * 4] = { 6, {192, 1365 + (1 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //left
		enemy_stats.animations[2][3 + i * 4] = { 6, {192, 1365 + (2 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //front
	}
	for (int i = 0; i <= 1; i++) {
		enemy_stats.animations[i][4] = { 5, {128, 1365 + 0 * 128, 128, 128}, {30,14}, {64,70} }; //back
		enemy_stats.animations[i][5] = { 5, {128, 1365 + 3 * 128, 128, 128}, {30,14}, {64,70} }; //right
		enemy_stats.animations[i][6] = { 5, {128, 1365 + 1 * 128, 128, 128}, {30,14}, {64,70} }; //left
		enemy_stats.animations[i][7] = { 5, {128, 1365 + 2 * 128, 128, 128}, {30,14}, {64,70} }; //front
	}

}

void GameState::deload_map()
{
	// static map destructor;

	for (int i = 0; i < size_x; i++) {
		for (int j = 0; j < size_y; j++)
			delete[] static_map[i][j].layers;

		delete[] static_map[i];
	}
	if (size_x)
		delete[] static_map;
	size_x = 0, size_y = 0;
}

void GameState::initial_game(string current_map, Vector2f player_pos)
{
	load_map(current_map);
	center_cam(player_pos);
}

void GameState::center_cam(Vector2f player_pos)
{
	map_x = -(player_pos.x - win_x / 2 / scale);
	map_y = -(player_pos.y - win_y / 2 / scale);

	if (-map_x < 0)
		map_x = 0;
	else if (-map_x > size_x * 16 - win_x / scale)
		map_x = -(size_x * 16 - win_x / scale);

	if (-map_y < 0)
		map_y = 0;
	else if (-map_y > size_y * 16 - win_y / scale)
		map_y = -(size_y * 16 - win_y / scale);

	x_offset = -map_x / 16, y_offset = -map_y / 16;

	player_entity.setPosition((player_pos.x + map_x) * scale, (player_pos.y + map_y) * scale);
}

void GameState::render_static_map()
{
	tile.setScale(scale, scale);
	for (int i = (x_offset > 0) ? x_offset : 0; i < (win_x + ((x_offset + 1) * 16 * scale)) / (16 * scale) && i < size_x; i++)
		for (int j = (y_offset > 0) ? y_offset : 0; j < (win_y + ((y_offset + 1) * 16 * scale)) / (16 * scale) && j < size_y; j++) {
			auto tile_end = static_map[i][j].layers + static_map[i][j].size - static_map[i][j].disable_top;
			for (auto map_tile = static_map[i][j].layers; map_tile != tile_end; map_tile++) {
				tile.setTexture(*tile_sheets[map_tile->z]);
				tile.setTextureRect(IntRect(map_tile->x * 16, map_tile->y * 16, 16, 16));
				tile.setPosition(map_x * scale + (16 * scale * i), map_y * scale + (16 * scale * j));
				window->draw(tile);
			}
		}
}

void GameState::render_entities()
{
	dynamic_rendering.insert({ player_entity.getRelativePos().y, {-1, &player_entity} });
	for (int i = 0; i < enemies.curr_idx; i++) {
		dynamic_rendering.insert({ enemies.entities[i]->getRelativePos().y, {-1, enemies.entities[i]}});
	}

	for (auto i = dynamic_rendering.lower_bound(-map_y-10); i != dynamic_rendering.end(); ) {
		//if (i->first > map_y + win_y / scale + 10)
		//	break;
		if (i->second.tile != -1) {
			
			for (int j = 0; j < dynamic_map.at[i->second.tile].curr_idx; j++) {
				if(i->second.tile == disable_dynamic_obj){
					i = dynamic_rendering.erase(i);
					disable_dynamic_obj = -1;
				}
				else {
					tile.setTexture(*tile_sheets[dynamic_map.at[i->second.tile].at[j].tile.z]);
					tile.setTextureRect(IntRect(dynamic_map.at[i->second.tile].at[j].tile.x * 16, dynamic_map.at[i->second.tile].at[j].tile.y * 16, 16, 16));
					tile.setPosition(map_x * scale + (16 * scale * dynamic_map.at[i->second.tile].at[j].position.x), map_y * scale + (16 * scale * dynamic_map.at[i->second.tile].at[j].position.y));
					window->draw(tile);
				}
			}
			i++;
		}
		else {
			i->second.entity->render();
			i = dynamic_rendering.erase(i);
		}
	}
}

GameState::GameState(int character_id, string current_map, Vector2f player_pos)
	: player_entity(player_stats, "character " + to_string(character_id), static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj), enemies(20)
{
	win_x = window->getSize().x, win_y = window->getSize().y;
	if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
	else scale = win_y / 304.5;

	initial_tile_sheets("game/tiles");
	load_maps(); //loads all maps ( pins[name]  { world map location x, world map location y, size x, size, y })
	load_entities(player_pos.y);
	initial_game(current_map, player_pos);


	/////////////////

}

GameState::~GameState()
{
	deload_map();
}

void GameState::update()
{
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
		else scale = win_y / 304.5;
		/////////////////////
		for (int i = 0; i < enemies.curr_idx; i++) {
			enemies.entities[i]->setScale(scale * enemies.entities[i]->entity_stats.scale_const);
		}
		center_cam(player_entity.getRelativePos());
	}


	if (enemies.vis == nullptr) {
		enemies.vis = new short* [enemies.find_size_x];
		for (int i = 0; i < enemies.find_size_x; i++) {
			(enemies.vis)[i] = new short[enemies.find_size_y]({});
		}
	}
	player_entity.update();
	for (int i = 0; i < enemies.curr_idx; i++) {
		enemies.entities[i]->update();
	}
	
	if (enemies.astar_done) {
		for (int i = 0; i < enemies.find_size_x; i++) {
			delete[] enemies.vis[i];
		}
		delete[] enemies.vis;
		enemies.vis = nullptr;
		enemies.astar_done = 0;
	}

}

void GameState::render()
{
	render_static_map();
	render_entities();
}

void GameState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				states->insert(PauseST); return; break;
				break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			case Keyboard::Num1:
				player_entity.change_state(3);
				break;
			case Keyboard::Num2:
				player_entity.change_state(2);
				break;
			case Keyboard::Num3:
				player_entity.change_state(1);
				break;
			case Keyboard::Num4:
				player_entity.change_state(0);
				break;
			case Keyboard::Space:
				player_entity.use_tool();
				break;
			}
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case Mouse::Left:
				clicked_on = window->mapPixelToCoords(Mouse::getPosition(*window));
				break;
			}
		case Event::MouseWheelMoved:
			if (event.type == Event::MouseWheelMoved) {
				int new_state = player_entity.state - event.mouseWheel.delta;
				if (new_state > 3) new_state = 3;
				else if (new_state < 0) new_state = 0;
				player_entity.change_state(new_state);
			}
		}
	}
}