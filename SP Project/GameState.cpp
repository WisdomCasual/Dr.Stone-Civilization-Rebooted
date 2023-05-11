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
	short temp_destructable[800];
	short destructable_count = 0;
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
						static_map[i][j].tool_type = tile_props[tle.z].properties[tle.x][tle.y].tool_type;
						static_map[i][j].object_ID = destructable_count;
						temp_destructable[destructable_count] = tile_props[tle.z].properties[tle.x][tle.y].object_type;
						destructable_count++;
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

	destructable_objects = new base_stats[destructable_count];
	for (int i = 0; i < destructable_count; i++) {
		destructable_objects[i] = object_stats[temp_destructable[i]];
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
	enemies.add(default_enemy, { 850, 850 });
	enemies.add(0, wolf, { 750, 750 });
	enemies.add(0, lion, { 900, 900 });
	passive.add(default_passive, { 775, 775 });
	passive.add(2, cow, { 825, 825 });
	passive.add(2, llama, { 875, 875 });
	passive.add(2, deer, { 725, 725 });

	//player
	player_stats.animations = new animation * [5]({});
	player_stats.states_no = 4;
	player_stats.base_movement_speed = 130;
	player_stats.scale_const = 0.65;
	player_stats.base_animation_speed = 16.6;
	player_stats.base_damage = 20;
	for (int i = 0; i <= 3; i++) {
		player_stats.animations[i] = new animation[16];
		player_stats.animations[i][0] = { 9, {0, 8 * 65, 64, 65}, {30,14}, {32,48} }; //back
		player_stats.animations[i][1] = { 9, {0, 11 * 65, 64, 65}, {30,14}, {32,48} }; //right
		player_stats.animations[i][2] = { 9, {0, 9 * 65, 64, 65}, {30,14}, {32,48} }; //left
		player_stats.animations[i][3] = { 9, {0, 10 * 65, 64, 65}, {30,14}, {32,48} }; //front
	}
	for (int i = 1; i <= 3; i++) {
		player_stats.animations[2][0 + i * 4] = { 6, {0, 1365 + (0 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //back
		player_stats.animations[2][1 + i * 4] = { 6, {0, 1365 + (3 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //right
		player_stats.animations[2][2 + i * 4] = { 6, {0, 1365 + (1 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //left
		player_stats.animations[2][3 + i * 4] = { 6, {0, 1365 + (2 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //front
	}
	for (int i = 0; i <= 1; i++) {
		player_stats.animations[i][4] = { 5, {0, 1365 + 0 * 128, 128, 128}, {30,14}, {64,70} }; //back
		player_stats.animations[i][5] = { 5, {0, 1365 + 3 * 128, 128, 128}, {30,14}, {64,70} }; //right
		player_stats.animations[i][6] = { 5, {0, 1365 + 1 * 128, 128, 128}, {30,14}, {64,70} }; //left
		player_stats.animations[i][7] = { 5, {0, 1365 + 2 * 128, 128, 128}, {30,14}, {64,70} }; //front
	}
	player_entity.change_state(3);

	//animals
	lion_stats.animations = new animation * [1];
	lion_stats.scale_const = 0.7;
	lion_stats.base_movement_speed = 80;
	lion_stats.base_animation_speed = 16.6;

	lion_stats.states_no = 1;
	lion_stats.animations[0] = new animation[4];
	lion_stats.animations[0][0] = { 4, {0, 0 * 64, 64, 64}, {20,63}, {32,32} }; //back
	lion_stats.animations[0][1] = { 5, {0, 2 * 64, 64, 64}, {63,16}, {32,47} }; //right
	lion_stats.animations[0][2] = { 5, {0, 1 * 64, 64, 64}, {63,16}, {32,48} }; //left
	lion_stats.animations[0][3] = { 4, {0, 3 * 64, 64, 64}, {20,63}, {32,33} }; //front

	wolf_stats.animations = new animation * [1];
	wolf_stats.scale_const = 0.7;
	wolf_stats.base_movement_speed = 80;
	wolf_stats.states_no = 1;
	wolf_stats.base_animation_speed = 16.6;

	wolf_stats.animations[0] = new animation[12];
	wolf_stats.animations[0][0] = { 4, {160, 128, 32, 64}, {22,60}, {16,30} }; //back
	wolf_stats.animations[0][1] = { 5, {320, 96, 64, 32}, {62,12}, {34,28} }; //right
	wolf_stats.animations[0][2] = { 5, {320, 288, 64, 32}, {62,12}, {34,28} }; //left
	wolf_stats.animations[0][3] = { 4, {0, 128, 32 , 64}, {22,60}, {16,30} };//front

	wolf_stats.animations[0][4] = { 5, {160, 196, 32, 64}, {22,60}, {16,30} }; //back attack
	wolf_stats.animations[0][5] = { 5, {320, 128, 64, 32}, {62,12}, {34,28} }; //right attack
	wolf_stats.animations[0][6] = { 5, {320, 320, 64, 32}, {62,12}, {34,28} }; //left attack
	wolf_stats.animations[0][7] = { 5, {0, 196, 32, 64}, {22,60}, {16,30} }; //front attack

	wolf_stats.animations[0][8] = { 5, {160, 256, 32, 64}, {22,60}, {16,30} }; //back attack bsnanoh
	wolf_stats.animations[0][9] = { 5, {320, 160, 64, 32}, {62,12}, {34,28} }; //right attack bsnanoh
	wolf_stats.animations[0][10] = { 5, {320, 352, 64, 32}, {62,12}, {34,28} }; //left attack bsnanoh
	wolf_stats.animations[0][11] = { 5, {0, 256, 32, 64}, {22,60}, {16,30} }; //front attack bsnanoh

	cow_stats.animations = new animation * [1];
	cow_stats.scale_const = 0.6;
	cow_stats.base_movement_speed = 80;
	cow_stats.states_no = 1;
	cow_stats.base_animation_speed = 16.6;

	cow_stats.animations[0] = new animation[4];
	cow_stats.animations[0][0] = { 4, {0, 0 * 128, 128, 128}, {28,72}, {64,64} }; //back
	cow_stats.animations[0][1] = { 4, {0, 3 * 128, 128, 128}, {88,16}, {64,64} }; //right
	cow_stats.animations[0][2] = { 4, {0, 1 * 128, 128, 128}, {88,16}, {64,64} }; //left
	cow_stats.animations[0][3] = { 4, {0, 2 * 128, 128, 128}, {28,57}, {64,64} }; //front

	deer_stats.animations = new animation * [1];
	deer_stats.scale_const = 0.7;
	deer_stats.base_movement_speed = 80;
	deer_stats.states_no = 1;
	deer_stats.base_animation_speed = 16.6;

	deer_stats.animations[0] = new animation[5];
	deer_stats.animations[0][0] = { 4, {0, 0 * 96, 64, 96}, {22,78}, {32,48} }; //back
	deer_stats.animations[0][1] = { 4, {0, 2 * 96, 64, 96}, {63,14}, {32,84} }; //right
	deer_stats.animations[0][2] = { 4, {0, 1 * 96,64, 96}, {63,14}, {32,84} }; //left
	deer_stats.animations[0][3] = { 4, {0, 3 * 96, 64, 96}, {22,75}, {32,59} }; //front

	llama_stats.animations = new animation * [1];
	llama_stats.scale_const = 0.7;
	llama_stats.base_movement_speed = 80;
	llama_stats.states_no = 1;
	llama_stats.base_animation_speed = 16.6;

	llama_stats.animations[0] = new animation[5];
	llama_stats.animations[0][0] = { 4, {0, 0 * 128, 128, 128}, {26,68}, {64,64} }; //back
	llama_stats.animations[0][1] = { 4, {0, 3 * 128, 128, 128}, {47,17}, {64,82} }; //right
	llama_stats.animations[0][2] = { 4, {0 , 1 * 128, 128, 128}, {47,17}, {64,82} }; //left
	llama_stats.animations[0][3] = { 4, {0, 2 * 128, 128, 128}, {26,62}, {64,62} }; //front

	//TO BE REMOVED
	enemy_stats.animations = new animation * [4];
	enemy_stats.scale_const = 0.4;
	enemy_stats.states_no = 4;
	enemy_stats.base_movement_speed = 80;
	enemy_stats.base_animation_speed = 16.6;
	for (int i = 0; i < 4; i++) {
		enemy_stats.animations[i] = new animation[16];
		enemy_stats.animations[i][0] = { 9, {0, 8 * 65, 64, 65}, {30,14}, {32,48} }; //back
		enemy_stats.animations[i][1] = { 9, {0, 11 * 65, 64, 65}, {30,14}, {32,48} }; //right
		enemy_stats.animations[i][2] = { 9, {0, 9 * 65, 64, 65}, {30,14}, {32,48} }; //left
		enemy_stats.animations[i][3] = { 9, {0, 10 * 65, 64, 65}, {30,14}, {32,48} }; //front
	}
	for (int i = 1; i <= 3; i++) {
		enemy_stats.animations[2][0 + i * 4] = { 6, {0, 1365 + (0 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //back
		enemy_stats.animations[2][1 + i * 4] = { 6, {0, 1365 + (3 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //right
		enemy_stats.animations[2][2 + i * 4] = { 6, {0, 1365 + (1 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //left
		enemy_stats.animations[2][3 + i * 4] = { 6, {0, 1365 + (2 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //front
	}
	for (int i = 0; i <= 1; i++) {
		enemy_stats.animations[i][4] = { 5, {0, 1365 + 0 * 128, 128, 128}, {30,14}, {64,70} }; //back
		enemy_stats.animations[i][5] = { 5, {0, 1365 + 3 * 128, 128, 128}, {30,14}, {64,70} }; //right
		enemy_stats.animations[i][6] = { 5, {0, 1365 + 1 * 128, 128, 128}, {30,14}, {64,70} }; //left
		enemy_stats.animations[i][7] = { 5, {0, 1365 + 2 * 128, 128, 128}, {30,14}, {64,70} }; //front
	}

	passive_stats.animations = new animation * [4];
	passive_stats.scale_const = 0.6;
	passive_stats.states_no = 4;
	passive_stats.base_movement_speed = 100;
	passive_stats.base_animation_speed = 16.6;
	for (int i = 0; i < 4; i++) {
		passive_stats.animations[i] = new animation[16];
		passive_stats.animations[i][0] = { 9, {0, 8 * 65, 64, 65}, {30,14}, {32,48} }; //back
		passive_stats.animations[i][1] = { 9, {0, 11 * 65, 64, 65}, {30,14}, {32,48} }; //right
		passive_stats.animations[i][2] = { 9, {0, 9 * 65, 64, 65}, {30,14}, {32,48} }; //left
		passive_stats.animations[i][3] = { 9, {0, 10 * 65, 64, 65}, {30,14}, {32,48} }; //front
	}
	for (int i = 1; i <= 3; i++) {
		passive_stats.animations[2][0 + i * 4] = { 6, {0, 1365 + (0 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //back
		passive_stats.animations[2][1 + i * 4] = { 6, {0, 1365 + (3 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //right
		passive_stats.animations[2][2 + i * 4] = { 6, {0, 1365 + (1 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //left
		passive_stats.animations[2][3 + i * 4] = { 6, {0, 1365 + (2 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //front
	}
	for (int i = 0; i <= 1; i++) {
		passive_stats.animations[i][4] = { 5, {0, 1365 + 0 * 128, 128, 128}, {30,14}, {64,70} }; //back
		passive_stats.animations[i][5] = { 5, {0, 1365 + 3 * 128, 128, 128}, {30,14}, {64,70} }; //right
		passive_stats.animations[i][6] = { 5, {0, 1365 + 1 * 128, 128, 128}, {30,14}, {64,70} }; //left
		passive_stats.animations[i][7] = { 5, {0, 1365 + 2 * 128, 128, 128}, {30,14}, {64,70} }; //front
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

	//if(destructable_objects!= nullptr)
	//	delete[] destructable_objects;
}

void GameState::initial_stats()
{
	////// tree /////////
	object_stats[0].health = 3;
	object_stats[0].idx = 0;
	object_stats[0].drops_no = 3;
	object_stats[0].item_drops[0] = 0;
	object_stats[0].item_drops[1] = 0;
	object_stats[0].item_drops[2] = 0;

	//////// big rock /////////
	object_stats[1].health = 3;
	object_stats[1].idx = 1;
	object_stats[1].drops_no = 3;
	object_stats[1].item_drops[0] = 1;
	object_stats[1].item_drops[1] = 1;
	object_stats[1].item_drops[2] = 1;

	/////// small rock //////////
	object_stats[2].health = 1;
	object_stats[2].idx = 2;
	object_stats[2].drops_no = 1;
	object_stats[2].item_drops[0] = 1;





	//////////////// item drops /////////////////

	/////// wood ///////
	drop_stats[0] = {13, 5, 3};
	/////// stone ///////
	drop_stats[1] = { 1, 13, 3 };
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

	//render effects
	for (int i = 0; i < effects.curr_idx; i++) {
		effects.animations[i]->render();
	}

}

void GameState::render_entities()
{
	dynamic_rendering.insert({ player_entity.getRelativePos().y, {-1, &player_entity} });
	for (int i = 0; i < enemies.curr_idx; i++) {
		dynamic_rendering.insert({ enemies.entities[i]->getRelativePos().y, {-1, enemies.entities[i]}});
	}

	for (int i = 0; i < items.curr_idx; i++) {
		dynamic_rendering.insert({ items.entities[i]->getRelativePos().y, {-1, items.entities[i]} });
	}

	for (int i = 0; i < passive.curr_idx; i++) {
		dynamic_rendering.insert({ passive.entities[i]->getRelativePos().y, {-1, passive.entities[i]} });
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

GameState::GameState(int character_id, string current_map, Vector2f player_pos, string character_name, int save_num)
	: player_entity(player_stats, "character " + to_string(character_id), static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj), items(50)
{
	this->character_name = character_name, this->current_map = current_map;
	this->character_id = character_id, this->save_num = save_num;
	win_x = window->getSize().x, win_y = window->getSize().y;
	if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
	else scale = win_y / 304.5;
	initial_tile_sheets("game/tiles");
	initial_textures("game");
	hotbar.setTexture(*textures[0]);
	hotbar_selection.setTexture(*textures[1]);
	hotbar.setOrigin(hotbar.getLocalBounds().width / 2, hotbar.getLocalBounds().height / 2);
	hotbar_selection.setOrigin(25, hotbar_selection.getLocalBounds().height / 2);
	tool_icons[0].setTexture(*textures[2]);
	tool_icons[1].setTexture(*textures[3]);
	tool_icons[2].setTexture(*textures[4]);
	for (int i = 0; i < 3; i++) {
		tool_icons[i].setOrigin(tool_icons[i].getLocalBounds().width / 2, tool_icons[i].getLocalBounds().height / 2);
		tool_icons[i].setColor(Color(130, 130, 130));
	}
	health_indicator.setTexture(*textures[5]);

	health_indicator.setTextureRect(IntRect(0, ceil(player_entity.health / 10.0) * 100, 590, 100));
	health_indicator.setOrigin(health_indicator.getLocalBounds().width, health_indicator.getLocalBounds().height / 2);

	initial_stats();
	load_maps(); //loads all maps ( pins[name]  { world map location x, world map location y, size x, size, y })
	load_entities(player_pos.y);	
	initial_game(current_map, player_pos);
	player_entity.setObjectStats(object_stats, &destructable_objects, item_drops, &item_drops_count);
}

GameState::~GameState()
{
	deload_map();

	if (destructable_objects != nullptr)
		delete destructable_objects;
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
		center_cam(player_entity.getRelativePos());
		hotbar.setScale(scale * 0.1, scale * 0.1);
		hotbar.setPosition(win_x/2, win_y - 20 * scale);
		hotbar_selection.setScale(scale * 0.1, scale * 0.1);
		health_indicator.setPosition(win_x - 20 * scale , 20 * scale);
		health_indicator.setScale(scale * 0.15, scale * 0.15);
		for (int i = 0; i < 3; i++) {
			tool_icons[i].setPosition(win_x / 2 + 3*scale, win_y - 20 * scale);
			tool_icons[i].setScale(scale * 0.1, scale * 0.1);
		}
		hotbar_selection.setPosition(win_x / 2 - (hotbar.getLocalBounds().width / 2 - 12) * scale * 0.1 + (3 - player_entity.state) * 248 * scale * 0.1, win_y - 20 * scale);
	}

	if (player_entity.health < 100 && heal_delay >= 5) {
		heal_delay = 0;
		player_entity.health += 10;
	}
	else
		heal_delay+=dt;

	health_indicator.setTextureRect(IntRect(0, ceil(player_entity.health / 10.0) * 100, 590, 100));

	if (enemies.vis == nullptr) {
		enemies.vis = new short* [enemies.find_size_x];
		for (int i = 0; i < enemies.find_size_x; i++) {
			(enemies.vis)[i] = new short[enemies.find_size_y]({});
		}
	}
	for (int i = 0; i < enemies.curr_idx; i++) {
		if (!enemies.entities[i]->despawn)
			enemies.entities[i]->update();
		else {
			effects.add({ 400,0,100,100 }, 20, { int(enemies.entities[i]->getRelativePos().x) , int(enemies.entities[i]->getRelativePos().y) }, "break_animation", Color(150, 50, 50, 240), 0, map_x, map_y);
			enemies.remove(i);
		}
	}

	for (int i = 0; i < passive.curr_idx; i++) {
		if (!passive.entities[i]->despawn)
			passive.entities[i]->update();
		else {
			effects.add({ 400,0,100,100 }, 20, { int(passive.entities[i]->getRelativePos().x) , int(passive.entities[i]->getRelativePos().y) }, "break_animation", Color(150, 50, 50, 240), 0, map_x, map_y);
			passive.remove(i);
		}
	}
	
	if (enemies.astar_done) {
		for (int i = 0; i < enemies.find_size_x; i++) {
			delete[] enemies.vis[i];
		}
		delete[] enemies.vis;
		enemies.vis = nullptr;
		enemies.astar_done = 0;
	}
	if (!player_entity.despawn) {
		no_update = 0;
		player_entity.update();
	}
	else {
		health_indicator.setTextureRect(IntRect(0, 0, 590, 100));
		states->insert({ DialogueID,new DialogueState(death_message,{win_x / 2,win_y / 2},scale / 2,2) });
		no_update++;
		if (no_update>=2) {
			string file_name = "Saves/Save" + to_string(save_num + 1) + ".ini";
			remove(file_name.c_str());
			states->insert(MainMenuST);
			if (states->find(BackgroundID) == states->end())
				states->insert(BackgroundST);

			int exceptions[] = { MainMenuID , BackgroundID };
			game.erase_states(exceptions, 2);
			return;
		}
	}

	for (int i = 0; i < effects.curr_idx; i++) {
		if (effects.animations[i]->despawn) {
			effects.remove(i);
			i--;
		}
		else
			effects.animations[i]->update(scale);
	}

	for (int i = 0; i < items.curr_idx; i++) {
		if (items.entities[i]->despawn) {
			items.remove(i);
			i--;
			// add item to player_inventory
		}
		else {
			items.entities[i]->update();
		}
		
	}
	/*will_spawn = !(rand() % 10);
	if (will_spawn) {
		short choices = (x_offset - 10 >= 0) + (x_offset + 10 + roundf(window->getSize().x / 16.f / scale) >= 0) +
			(y_offset - 10 >= 0) + (y_offset + 10 + roundf(window->getSize().y / 16.f / scale) < size_y),
			choice;
		short boundry_len_x = roundf(win_x / 16.f / scale),
			boundry_len_y = roundf(win_y / 16.f / scale);
		if (choices) {
			choice = rand() % choices;
		spawn_type = rand() % 2;

		}
		if (x_offset - 10 >= 0) {
			if (!choice) {
				bool no_spawn = 0;
				if (!spawn_type) {
					for (int i = y_offset; i < boundry_len_y + y_offset; i++) {
						no_spawn = 0;
						for (int k = -1; k < 2; k++) {
							for (int l = -1; l < 2; l++) {
								if ((static_map[int(x_offset - 10) + k][i + l].tile_props & 2)); {
									no_spawn = 1;
									break;
								}
							}
							if (no_spawn) break;
						}
						if (no_spawn) break;
						short enemy_type = rand() % 2;
						switch (enemy_type) {
							cout << "spawnl\n";
							case 0:
								enemies.add(0, wolf, { float(x_offset - 10), (float)i });
							case 1:
								enemies.add(0, lion, { float(x_offset - 10), (float)i });
						}
					}
				}
				else {
					for (int i = y_offset; i < boundry_len_y + y_offset; i++) {
						no_spawn = 0;
						for (int k = -1; k < 2; k++) {
							for (int l = -1; l < 2; l++) {
								if ((static_map[int(x_offset - 10) + k][i + l].tile_props & 2)); {
									no_spawn = 1;
									break;
								}
							}
							if (no_spawn) break;
						}
						if (no_spawn) break;
						short enemy_type = rand() % 2;
						switch (enemy_type) {
							cout << "spawnl\n";
						case 0:
							enemies.add(0, cow, { float(x_offset - 10), (float)i });
						case 1:
							enemies.add(0, llama, { float(x_offset - 10), (float)i });
						}
					}
				}
			}
				
			else
				choice--;
		}
		if (x_offset + 10 + boundry_len_x >= 0) {
			if (!choice) {
				bool no_spawn = 0;
				if (!spawn_type) {
					for (int i = y_offset; i < boundry_len_y + y_offset; i++) {
						no_spawn = 0;
						for (int k = -1; k < 2; k++) {
							for (int l = -1; l < 2; l++) {
								if ((static_map[int(x_offset + 10) + k][i + l].tile_props & 2)); {
									no_spawn = 1;
									break;
								}
							}
							if (no_spawn) break;
						}
						if (no_spawn) break;
						short enemy_type = rand() % 2;
						switch (enemy_type) {
							cout << "spawnr\n";
						case 0:
							enemies.add(0, wolf, { float(x_offset + 10), (float)i });
						case 1:
							enemies.add(0, lion, { float(x_offset + 10), (float)i });
						}
					}
				}
				else {
					for (int i = y_offset; i < boundry_len_y + y_offset; i++) {
						no_spawn = 0;
						for (int k = -1; k < 2; k++) {
							for (int l = -1; l < 2; l++) {
								if ((static_map[int(x_offset + 10) + k][i + l].tile_props & 2)); {
									no_spawn = 1;
									break;
								}
							}
							if (no_spawn) break;
						}
						if (no_spawn) break;
						short enemy_type = rand() % 2;
						switch (enemy_type) {
							cout << "spawnr\n";
						case 0:
							enemies.add(0, cow, { float(x_offset + 10), (float)i });
						case 1:
							enemies.add(0, llama, { float(x_offset + 10), (float)i });
						}
					}
				}
			}

			else
				choice--;
		}
		if (y_offset - 10 >= 0) {
			if (!choice) {
				bool no_spawn = 0;
				if (!spawn_type) {
					for (int i = x_offset; i < boundry_len_y + x_offset; i++) {
						no_spawn = 0;
						for (int k = -1; k < 2; k++) {
							for (int l = -1; l < 2; l++) {
								if ((static_map[int(y_offset - 10) + k][i + l].tile_props & 2)); {
									no_spawn = 1;
									break;
								}
							}
							if (no_spawn) break;
						}
						if (no_spawn) break;
						short enemy_type = rand() % 2;
						cout << "spawnu\n";
						switch (enemy_type) {
						case 0:
							enemies.add(0, wolf, { float(y_offset - 10), (float)i });
						case 1:
							enemies.add(0, lion, { float(y_offset - 10), (float)i });
						}
					}
				}
				else {
					for (int i = x_offset; i < boundry_len_y + x_offset; i++) {
						no_spawn = 0;
						for (int k = -1; k < 2; k++) {
							for (int l = -1; l < 2; l++) {
								if ((static_map[int(y_offset - 10) + k][i + l].tile_props & 2)); {
									no_spawn = 1;
									break;
								}
							}
							if (no_spawn) break;
						}
						if (no_spawn) break;
						short enemy_type = rand() % 2;
						cout << "spawnu\n";
						switch (enemy_type) {
						case 0:
							enemies.add(0, cow, { float(y_offset - 10), (float)i });
						case 1:
							enemies.add(0, llama, { float(y_offset - 10), (float)i });
						}
					}
				}
			}

			else
				choice--;
		}
		if (y_offset + 10 + boundry_len_y < size_y) {
			bool no_spawn = 0;
			if (!spawn_type) {
				for (int i = x_offset; i < boundry_len_y + x_offset; i++) {
					no_spawn = 0;
					for (int k = -1; k < 2; k++) {
						for (int l = -1; l < 2; l++) {
							if ((static_map[int(y_offset + 10) + k][i + l].tile_props & 2)); {
								no_spawn = 1;
								break;
							}
						}
						if (no_spawn) break;
					}
					if (no_spawn) break;
					short enemy_type = rand() % 2;
					cout << "spawnd\n";
					switch (enemy_type) {
					case 0:
						enemies.add(0, wolf, { float(y_offset + 10), (float)i });
					case 1:
						enemies.add(0, lion, { float(y_offset + 10), (float)i });
					}
				}
			}
			else {
				for (int i = x_offset; i < boundry_len_y + x_offset; i++) {
					no_spawn = 0;
					for (int k = -1; k < 2; k++) {
						for (int l = -1; l < 2; l++) {
							if ((static_map[int(y_offset + 10) + k][i + l].tile_props & 2)); {
								no_spawn = 1;
								break;
							}
						}
						if (no_spawn) break;
					}
					if (no_spawn) break;
					short enemy_type = rand() % 2;
					cout << "spawnd\n";
					switch (enemy_type) {
					case 0:
						enemies.add(0, cow, { float(y_offset + 10), (float)i });
					case 1:
						enemies.add(0, llama, { float(y_offset + 10), (float)i });
					}
				}
			}
		}
	}*/ //random spawn system (WIP)
}

void GameState::render()
{
	render_static_map();
	render_entities();
	window->draw(hotbar);
	for(int i = 0; i< 3; i++)
		window->draw(tool_icons[i]);
	window->draw(hotbar_selection);
	window->draw(health_indicator);
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
			case Keyboard::F6:
			{
				string notification_s[] = { "Saved Successfully" };
				game.notification(notification_s, 1);
				ofstream ofs("Saves/Save" + to_string(save_num + 1) + ".ini", ofstream::out, ofstream::trunc);

				if (ofs.is_open()) {
					ofs << character_name << '\n';
					ofs << (int)character_id << '\n';
					ofs << 1 << '\n';
					ofs << current_map << '\n';
					ofs << player_entity.getRelativePos().x << ' ' << player_entity.getRelativePos().x << '\n';
				}
				ofs.close();
			}

					break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			case Keyboard::Num1:
				player_entity.change_state(3);
				hotbar_selection.setPosition(win_x / 2 - (hotbar.getLocalBounds().width / 2 - 12) * scale * 0.1 + (3 - player_entity.state) * 248 * scale * 0.1, win_y - 20 * scale);
				tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(130, 130, 130));
				break;
			case Keyboard::Num2:
				player_entity.change_state(2);
				hotbar_selection.setPosition(win_x / 2 - (hotbar.getLocalBounds().width / 2 - 12) * scale * 0.1 + (3 - player_entity.state) * 248 * scale * 0.1, win_y - 20 * scale);
				tool_icons[0].setColor(Color(255, 255, 255)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(130, 130, 130));
				break;
			case Keyboard::Num3:
				player_entity.change_state(1);
				hotbar_selection.setPosition(win_x / 2 - (hotbar.getLocalBounds().width / 2 - 12) * scale * 0.1 + (3 - player_entity.state) * 248 * scale * 0.1, win_y - 20 * scale);
				tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(255, 255, 255)), tool_icons[2].setColor(Color(130, 130, 130));
				break;
			case Keyboard::Num4:
				player_entity.change_state(0);
				hotbar_selection.setPosition(win_x / 2 - (hotbar.getLocalBounds().width / 2 - 12) * scale * 0.1 + (3 - player_entity.state) * 248 * scale * 0.1, win_y - 20 * scale);
				tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(255, 255, 255));
				break;
			case Keyboard::Space:
				player_entity.use_tool();
				if (player_entity.tool_used_on.x > -1) {
					Vector3i tile_info = static_map[player_entity.tool_used_on.x/16][player_entity.tool_used_on.y / 16].layers[static_map[player_entity.tool_used_on.x / 16][player_entity.tool_used_on.y / 16].size - 1];
					Color tile_color = tile_sheets_img[tile_info.z].getPixel(tile_info.x * 16 + 8, tile_info.y * 16 + 6);
					effects.add({ 0,0,100,100 }, 24, { player_entity.tool_used_on.x , player_entity.tool_used_on.y }, "break_animation", tile_color, 0, map_x, map_y);
					if (item_drops_count != -1) {
						Vector3i temp;
						for (int i = 0; i < item_drops_count; i++) {
							items.add(1, cow_stats, "item", static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, &player_entity, { (float)player_entity.tool_used_on.x , (float)player_entity.tool_used_on.y }, tile_sheets, drop_stats[item_drops[i]]);
						}
						item_drops_count = -1;
					}
					player_entity.tool_used_on.x = -1;
				}
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
				hotbar_selection.setPosition(win_x / 2 - (hotbar.getLocalBounds().width / 2 - 12) * scale * 0.1 + (3 - player_entity.state) * 248 * scale * 0.1, win_y - 20 * scale);
				tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(130, 130, 130));
				if(new_state!=3)
					tool_icons[(2 - player_entity.state)].setColor(Color(255, 255, 255));
			}
		}
	}
}