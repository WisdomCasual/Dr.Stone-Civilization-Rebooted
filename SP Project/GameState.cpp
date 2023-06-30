#include "GameState.h"
#include "MiniMapState.h"

void GameState::black_in()
{
	if (blackining > 0) {
		if (blackining - 500 * dt < 0)
			blackining = 0;
		else
			blackining -= 500 * dt;
		blackscreen.setFillColor(Color(0, 0, 0, blackining));
	}
}

void GameState::search_front(int x, int y, int layr, Vector3i*** temp_front, bool*** vis, int idx)
{
	update_minimap_tile(Vector2i(x * 2, y * 2), temp_front[layr][x][y]);
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

	light_sources.clear();

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
			temp_front[i] = new Vector3i * [size_x];
			vis[i] = new bool* [size_x];
		}

		static_map = new render_tile * [size_x];

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
					if ((tle.x >= 11 && tle.x <= 14) && tle.y == 12 && tle.z == 3) {
						static_map[i][j].tile_props |= (256 << (tle.x - 11));
					}
					else if (tle.x == 15 && tle.y == 12 && tle.z == 3) {
						static_map[i][j].tile_props |= 4096;
					}
					else if (tle.y >= 16 && tle.z == 3) {
						float intensity;
						if (!(tle.x % 3)) intensity = 0.7f;
						else if (!(tle.x % 2)) intensity = 0.3f;
						else intensity = 0.5f;
						if (tle.y == 16) {
							if (tle.x < 3)
								light_sources.insert({ j * 16.0 + 8.0, light(Vector2f(i * 16.0 + 8.0, j * 16.0 + 8.0), Vector3f(1, 1, 1), intensity) });
							else if (tle.x < 6)
								light_sources.insert({ j * 16.0 + 8.0, light(Vector2f(i * 16.0 + 8.0, j * 16.0 + 8.0), Vector3f(1, 1, 0.5), intensity) });
							else if (tle.x < 9)
								light_sources.insert({ j * 16.0 + 8.0, light(Vector2f(i * 16.0 + 8.0, j * 16.0 + 8.0), Vector3f(1, 0.5, 0.5), intensity) });
							else if (tle.x < 12)
								light_sources.insert({ j * 16.0 + 8.0, light(Vector2f(i * 16.0 + 8.0, j * 16.0 + 8.0), Vector3f(0.5, 0.5, 1), intensity) });
							else if (tle.x < 15)
								light_sources.insert({ j * 16.0 + 8.0, light(Vector2f(i * 16.0 + 8.0, j * 16.0 + 8.0), Vector3f(0.5, 1, 0.5), intensity) });
						}
					}
					else {
						if (layer_prop & 32) {
							static_map[i][j].tool_type = tile_props[tle.z].properties[tle.x][tle.y].tool_type;
							static_map[i][j].object_ID = destructable_count;
							temp_destructable[destructable_count] = tile_props[tle.z].properties[tle.x][tle.y].object_type;
							destructable_count++;
						}
						if (layer_prop & 16) { // front core
							update_minimap_tile(Vector2i(i * 2, j * 2), tle);
							dynamic_objects objct;
							objct.add({ Vector2f(i, j), tle });
							objct.layer = layr;
							dynamic_map.add(objct);
							dynamic_rendering.insert({ float((j + 1) * 16), {short(dynamic_map.curr_idx - 1), nullptr} });
							static_map[i][j].dynamic_idx = dynamic_map.curr_idx - 1;
							//add ptr to set

							//add to dynamic tiles
						}
						else if ((layer_prop & 8)) { //front
							temp_front[layr][i][j] = { tle.x + 1, tle.y, tle.z };
						}
						else {   // back
							temp_layers[count] = tle;
							count++;
							if (layer_prop & 1)
								update_minimap_tile(Vector2i(i * 2, j * 2), tle);
						}

						static_map[i][j].tile_props |= layer_prop;
					}
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

	//player
	player_stats.is_player = 1;
	player_stats.animations = new animation * [5]({});
	player_stats.states_no = 4;
	player_stats.base_movement_speed = 130;
	player_stats.scale_const = 0.65;
	player_stats.base_animation_speed = 16.6;
	player_stats.textures_count = 4;
	player_stats.textures = new Texture*[player_stats.textures_count];
	player_stats.base_damage = 25;
	player_stats.max_health = 100;

	for (int i = 0; i < player_stats.textures_count; i++) {
		player_stats.textures[i] = new Texture;
		player_stats.textures[i]->loadFromFile("textures/game/entities/character " + to_string(character_id) + "/" + to_string(i) + ".png");
	}


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

	//animals
	lion_stats.animations = new animation * [1];
	lion_stats.scale_const = 0.7;
	lion_stats.base_movement_speed = 80;
	lion_stats.base_animation_speed = 16.6;
	lion_stats.textures_count = 1;
	lion_stats.textures = new Texture * [lion_stats.textures_count];
	lion_stats.textures[0] = new Texture;

	lion_stats.textures[0]->loadFromFile("textures/game/entities/lion/lion.png");

	lion_stats.states_no = 1;
	lion_stats.animations[0] = new animation[4];
						   // frames -- first Rect -- hitbox dimentions -- origin
	lion_stats.animations[0][0] = { 4, {0, 0 * 64, 64, 64}, {20,63}, {32,32} }; //back
	lion_stats.animations[0][1] = { 5, {0, 2 * 64, 64, 64}, {63,16}, {32,47} }; //right
	lion_stats.animations[0][2] = { 5, {0, 1 * 64, 64, 64}, {63,16}, {32,48} }; //left
	lion_stats.animations[0][3] = { 4, {0, 3 * 64, 64, 64}, {20,63}, {32,33} }; //front

	wolf_stats.animations = new animation * [1];
	wolf_stats.scale_const = 0.7;
	wolf_stats.base_movement_speed = 80;
	wolf_stats.states_no = 1;
	wolf_stats.base_animation_speed = 16.6;
	wolf_stats.textures_count = 1;
	wolf_stats.textures = new Texture * [wolf_stats.textures_count];
	wolf_stats.textures[0] = new Texture;


	wolf_stats.textures[0]->loadFromFile("textures/game/entities/wolf/wolf.png");

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
	cow_stats.textures_count = 1;
	cow_stats.textures = new Texture * [cow_stats.textures_count];
	cow_stats.textures[0] = new Texture;


	cow_stats.textures[0]->loadFromFile("textures/game/entities/cow/cow.png");

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
	deer_stats.textures_count = 1;
	deer_stats.textures = new Texture * [deer_stats.textures_count];
	deer_stats.textures[0] = new Texture;

	deer_stats.textures[0]->loadFromFile("textures/game/entities/deer/deer.png");

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
	llama_stats.textures_count = 1;
	llama_stats.textures = new Texture * [llama_stats.textures_count];
	llama_stats.textures[0] = new Texture;

	llama_stats.textures[0]->loadFromFile("textures/game/entities/llama/llama.png");

	llama_stats.animations[0] = new animation[5];
	llama_stats.animations[0][0] = { 4, {0, 0 * 128, 128, 128}, {26,68}, {64,64} }; //back
	llama_stats.animations[0][1] = { 4, {0, 3 * 128, 128, 128}, {47,17}, {64,82} }; //right
	llama_stats.animations[0][2] = { 4, {0 , 1 * 128, 128, 128}, {47,17}, {64,82} }; //left
	llama_stats.animations[0][3] = { 4, {0, 2 * 128, 128, 128}, {26,62}, {64,62} }; //front

	NPC_stats.animations = new animation * [1];
	NPC_stats.scale_const = 0.7;
	NPC_stats.base_movement_speed = 80;
	NPC_stats.states_no = 1;
	NPC_stats.base_animation_speed = 16.6;
	NPC_stats.textures_count = 1;
	NPC_stats.textures = new Texture * [deer_stats.textures_count];
	NPC_stats.textures[0] = new Texture;

	NPC_stats.textures[0]->loadFromFile("textures/game/entities/deer/deer.png");

	NPC_stats.animations[0] = new animation[5];
	NPC_stats.animations[0][0] = { 4, {0, 0 * 96, 64, 96}, {22,78}, {32,48} }; //back
	NPC_stats.animations[0][1] = { 4, {0, 2 * 96, 64, 96}, {63,14}, {32,84} }; //right
	NPC_stats.animations[0][2] = { 4, {0, 1 * 96,64, 96}, {63,14}, {32,84} }; //left
	NPC_stats.animations[0][3] = { 4, {0, 3 * 96, 64, 96}, {22,75}, {32,59} }; //front

	item_stats.textures_count = 1;
	item_stats.textures = new Texture * [item_stats.textures_count];
	item_stats.textures[0] = new Texture;

	item_stats.textures[0]->loadFromFile("textures/game/item drops.png");

	player_entity = new Player(player_stats, 1, static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, destroy_object_location);
	if (character_id == 3 && character_name == "Saitama") {
		player_entity->setDamage(SHRT_MAX/2);
		player_entity->destruction_power = SHRT_MAX/2;
		player_entity->set_movement_speed(269);
		player_stats.max_health = SHRT_MAX / 2;
	}
	enemies.add(wolf(0), {750, 750}, 1);
	enemies.add(lion(0), {900, 900}, 1);
	passive.add(cow(2), {825, 825}, 1);
	passive.add(llama(2), {875, 875}, 1);
	passive.add(deer(2), {725, 725}, 1);
	dialogue test[3] = { {"NPC", "hi there"}, {"NPC", "hello there"}, {"NPC", "welcome, traveller!"} } ;
	NPCs.add(default_npc, { 968, 712}, npc_details(1, 10, 0), 3, test);

	player_entity->change_state(3);

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
}

void GameState::initial_game(string current_map, Vector2f player_pos)
{
	this->current_map = current_map;
	base_minimap.loadFromFile("Maps/" + current_map + "_minimap.png");
	minimap_tex.loadFromFile("Maps/" + current_map + "_minimap.png");
	minimap_img.loadFromFile("Maps/" + current_map + "_minimap.png");
	load_map(current_map);
	center_cam(player_pos);
	minimap.setTexture(minimap_tex);
	minimap.setTextureRect(IntRect(0, 0, 96, 96));
	minimap.setOrigin(minimap.getLocalBounds().width / 2, minimap.getLocalBounds().height / 2);
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

	player_entity->setPosition((player_pos.x + map_x) * scale, (player_pos.y + map_y) * scale);
}

void GameState::destroyANDrestore_objects(Vector2i target_tile, bool destroy)
{
		for (int i = 0; i < 3; i++)
			for (int j = 1; j < 4; j++) {
					Vector2i check_area{ target_tile.x + dx[i], target_tile.y + dy[j] };
				if(destroy){
					if (static_map[check_area.x][check_area.y].tile_props & 16) {
						for (int j = 0; j < dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].curr_idx; j++) {
							Uint8 pixels[16];
							for (int k = 0; k < 2; k++)
								for (int l = 0; l < 2; l++) {
									pixels[(k + 2 * l) * 4] = base_minimap.getPixel(dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].at[j].position.x * 2 + k, dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].at[j].position.y * 2 + l).r; // red
									pixels[(k + 2 * l) * 4 + 1] = base_minimap.getPixel(dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].at[j].position.x * 2 + k, dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].at[j].position.y * 2 + l).g; // green
									pixels[(k + 2 * l) * 4 + 2] = base_minimap.getPixel(dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].at[j].position.x * 2 + k, dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].at[j].position.y * 2 + l).b; // blue
									pixels[(k + 2 * l) * 4 + 3] = base_minimap.getPixel(dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].at[j].position.x * 2 + k, dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].at[j].position.y * 2 + l).a; // alpha
									minimap_img.setPixel(dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].at[j].position.x * 2 + k, dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].at[j].position.y * 2 + l, Color(pixels[(k + 2 * l) * 4], pixels[(k + 2 * l) * 4 + 1], pixels[(k + 2 * l) * 4 + 2], pixels[(k + 2 * l) * 4 + 3]));
								}
							minimap_tex.update(pixels, 2, 2, dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].at[j].position.x * 2, dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].at[j].position.y * 2);
						}
						dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].core_location = target_tile;
						static_map[target_tile.x][target_tile.y].destruction_time = game_time;
						dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].destruction_time = &static_map[target_tile.x][target_tile.y].destruction_time;
						dynamic_map.at[static_map[check_area.x][check_area.y].dynamic_idx].time = &static_map[target_tile.x][target_tile.y].time;
						dynamic_update_minimap = 2;
					}
					else if (static_map[check_area.x][check_area.y].tile_props & 32) {
						if (!dx[i] && !dy[j])
							bigbang(check_area, 1);
					}
					else if (static_map[check_area.x][check_area.y].tile_props & 1)
						bigbang(check_area, 1);
				}
				else {
					if (static_map[check_area.x][check_area.y].destruction_time)
						bigbang(check_area, 0);
				}
			}
}

void GameState::bigbang(Vector2i target_tile, bool destroy)
{
	short last = static_map[target_tile.x][target_tile.y].size - 1;
	Vector3i last_tile = static_map[target_tile.x][target_tile.y].layers[last];
	static_map[target_tile.x][target_tile.y].tile_props ^= tile_props[last_tile.z].properties[last_tile.x][last_tile.y].props;
	if (destroy) {
		static_map[target_tile.x][target_tile.y].destruction_time = game_time;

		Uint8 pixels[16];
		for (int k = 0; k < 2; k++)
			for (int l = 0; l < 2; l++) {
				pixels[(k + 2 * l) * 4] = base_minimap.getPixel(target_tile.x * 2 + k, target_tile.y * 2 + l).r; // red
				pixels[(k + 2 * l) * 4 + 1] = base_minimap.getPixel(target_tile.x * 2 + k, target_tile.y * 2 + l).g; // green
				pixels[(k + 2 * l) * 4 + 2] = base_minimap.getPixel(target_tile.x * 2 + k, target_tile.y * 2 + l).b; // blue
				pixels[(k + 2 * l) * 4 + 3] = base_minimap.getPixel(target_tile.x * 2 + k, target_tile.y * 2 + l).a; // alpha
				minimap_img.setPixel(target_tile.x * 2 + k, target_tile.y * 2 + l, Color(pixels[(k + 2 * l) * 4], pixels[(k + 2 * l) * 4 + 1], pixels[(k + 2 * l) * 4 + 2], pixels[(k + 2 * l) * 4 + 3]));
			}
		minimap_tex.update(pixels, 2, 2, target_tile.x * 2, target_tile.y * 2);
	}
	else {
		static_map[target_tile.x][target_tile.y].destruction_time = 0;
		update_minimap_tile(Vector2i(target_tile * 2), last_tile);
	}
}

void GameState::render_static_map()
{
	tile.setScale(scale, scale);
	for (int i = (x_offset > 0) ? x_offset : 0; i < (win_x + ((x_offset + 1) * 16 * scale)) / (16 * scale) && i < size_x; i++)
		for (int j = (y_offset > 0) ? y_offset : 0; j < (win_y + ((y_offset + 1) * 16 * scale)) / (16 * scale) && j < size_y; j++) {
			auto tile_end = static_map[i][j].layers + static_map[i][j].size - 1;

			for (auto map_tile = static_map[i][j].layers; map_tile != tile_end; map_tile++) {
				tile.setTexture(*tile_sheets[map_tile->z]);
				tile.setTextureRect(IntRect(map_tile->x * 16, map_tile->y * 16, 16, 16));
				tile.setPosition(map_x * scale + (16 * scale * i), map_y * scale + (16 * scale * j));
				window->draw(tile, &shader);
			}
			if (!static_map[i][j].destruction_time) {
				if (dynamic_update_minimap)
					update_minimap_tile(Vector2i(i * 2, j * 2), *tile_end);
				tile.setTexture(*tile_sheets[tile_end->z]);
				tile.setTextureRect(IntRect(tile_end->x * 16, tile_end->y * 16, 16, 16));
				tile.setPosition(map_x * scale + (16 * scale * i), map_y * scale + (16 * scale * j));
				window->draw(tile, &shader);
			}
			else if (tile_props[tile_end->z].properties[tile_end->x][tile_end->y].props & 32 && game_time - static_map[i][j].destruction_time > 300 && game_time - static_map[i][j].time > 5) {
				destroyANDrestore_objects(Vector2i(i, j), 0);
				static_map[i][j].destruction_time = 0;
				static_map[i][j].time = 0;
			}
			else
				static_map[i][j].time = game_time;
		}

	//render effects
	for (int i = 0; i < effects.curr_idx; i++) {
		effects.animations[i]->render(&shader);
	}

}

void GameState::render_entities()
{
	dynamic_rendering.insert({ player_entity->getRelativePos().y, {-1, player_entity} });
	for (int i = 0; i < enemies.curr_idx; i++) {
		if (enemies.entities[i] != nullptr &&
			enemies.entities[i]->pos.y >= -map_y - entity_render_distance &&
			enemies.entities[i]->pos.y <= -map_y + entity_render_distance + win_y / scale &&
			enemies.entities[i]->pos.x >= -map_x - entity_render_distance &&
			enemies.entities[i]->pos.x <= -map_x + entity_render_distance + win_x / scale) {
			if (enemies.entities[i]->despawn)
				continue;
			dynamic_rendering.insert({ enemies.entities[i]->getRelativePos().y, {-1, enemies.entities[i]} });
		}
	}

	for (int i = 0; i < items.curr_idx; i++) {
		if (items.entities[i] != nullptr &&
			items.entities[i]->pos.y >= -map_y - entity_render_distance &&
			items.entities[i]->pos.y <= -map_y + entity_render_distance + win_y / scale &&
			items.entities[i]->pos.x >= -map_x - entity_render_distance &&
			items.entities[i]->pos.x <= -map_x + entity_render_distance + win_x / scale) {
			if (items.entities[i]->despawn)
				continue;
			dynamic_rendering.insert({ items.entities[i]->getRelativePos().y, {-1, items.entities[i]} });
		}
	}

	for (int i = 0; i < passive.curr_idx; i++) {
		if (passive.entities[i] != nullptr &&
			passive.entities[i]->pos.y >= -map_y - entity_render_distance &&
			passive.entities[i]->pos.y <= -map_y + entity_render_distance + win_y / scale &&
			passive.entities[i]->pos.x >= -map_x - entity_render_distance &&
			passive.entities[i]->pos.x <= -map_x + entity_render_distance + win_x / scale) {
			if (passive.entities[i]->despawn)
				continue;
			dynamic_rendering.insert({ passive.entities[i]->pos.y, {-1, passive.entities[i]} });
		}
	}

	for (int i = 0; i < NPCs.curr_idx; i++) {
		if (NPCs.entities[i] != nullptr &&
			NPCs.entities[i]->pos.y >= -map_y - entity_render_distance &&
			NPCs.entities[i]->pos.y <= -map_y + entity_render_distance + win_y / scale &&
			NPCs.entities[i]->pos.x >= -map_x - entity_render_distance &&
			NPCs.entities[i]->pos.x <= -map_x + entity_render_distance + win_x / scale) {
			if (NPCs.entities[i]->despawn)
				continue;
			dynamic_rendering.insert({ NPCs.entities[i]->pos.y, {-1, NPCs.entities[i]} });
		}
	}
	//int debug_ctr = 0;
	//cout << "total entities/objects: " << dynamic_rendering.size() << '\n';
	for (auto i = dynamic_rendering.lower_bound(-map_y - min(entity_render_distance, object_render_distance) - 32.f);
		i != dynamic_rendering.end() && i->first <= -map_y + max(short(obj_up_offset + object_render_distance), entity_render_distance) + 32.f + win_y / scale; ) {
		
		if (i->second.tile != -1) {
			if (dynamic_map.at[i->second.tile].at[0].position.x*16 < -map_x - obj_right_offest - object_render_distance ||
				dynamic_map.at[i->second.tile].at[0].position.x*16 > -map_x + obj_left_offset + object_render_distance + win_x / scale ||
				i->first < -map_y - object_render_distance - obj_down_offset || i->first > -map_y + object_render_distance + obj_up_offset + win_y/scale) {
				i++;
				continue;
			}
			//debug_ctr++;
			if(!dynamic_map.at[i->second.tile].destruction_time){
				for (int j = 0; j < dynamic_map.at[i->second.tile].curr_idx; j++) {
					tile.setTexture(*tile_sheets[dynamic_map.at[i->second.tile].at[j].tile.z]);
					tile.setTextureRect(IntRect(dynamic_map.at[i->second.tile].at[j].tile.x * 16, dynamic_map.at[i->second.tile].at[j].tile.y * 16, 16, 16));
					tile.setPosition(map_x * scale + (16 * scale * dynamic_map.at[i->second.tile].at[j].position.x), map_y * scale + (16 * scale * dynamic_map.at[i->second.tile].at[j].position.y));
					window->draw(tile, &shader);
					if (dynamic_update_minimap)
						update_minimap_tile(Vector2i(dynamic_map.at[i->second.tile].at[j].position.x * 2, dynamic_map.at[i->second.tile].at[j].position.y * 2), dynamic_map.at[i->second.tile].at[j].tile);
				}
			}
			else if (game_time - *dynamic_map.at[i->second.tile].destruction_time > 300 && game_time - *dynamic_map.at[i->second.tile].time > 5) {
				destroyANDrestore_objects(dynamic_map.at[i->second.tile].core_location, 0);
				dynamic_map.at[i->second.tile].destruction_time = 0;
				dynamic_map.at[i->second.tile].time = 0;
				dynamic_update_minimap = 2;
			}
			else
			{
				*dynamic_map.at[i->second.tile].time = game_time;
			}

			i++;
		}
		else {
			if (i->second.entity != nullptr) {
				if (i->second.entity->despawn) {
					i = dynamic_rendering.erase(i);
					return;
				}
				i->second.entity->render(&shader);
				i->second.entity = nullptr;
			}
			i = dynamic_rendering.erase(i);
		}
	}

	if(dynamic_update_minimap)
		dynamic_update_minimap--;

	//cout << "total rendering: " << debug_ctr << '\n';
}

void GameState::update_minimap()
{
	int minimap_player_pos_x = player_entity->getRelativePos().x / 8 , minimap_player_pos_y = player_entity->getRelativePos().y / 8;
	int minimap_x = minimap_player_pos_x - 48, minimap_y = minimap_player_pos_y - 48;
	minimap_x = (minimap_x < 0) ? 0 : (minimap_x > size_x * 2 - 98) ? size_x * 2 - 98 : minimap_x;
	minimap_y = (minimap_y < 0) ? 0 : (minimap_y > size_y * 2 - 98) ? size_y * 2 - 98 : minimap_y;
	minimap.setTextureRect(IntRect(minimap_x, minimap_y, 96, 96));
	player_pointer.setPosition(win_x - 52 * scale + (minimap_player_pos_x - minimap_x - 48) * 0.8 * scale, 52 * scale + (minimap_player_pos_y - minimap_y - 48) * 0.8 * scale);

	if (waypoint_position.x > -1) {
		int waypoint_minimap_pos_x = clamp<int>((int)waypoint_position.x - minimap_x - 48, -48, 48);
		int waypoint_minimap_pos_y = clamp<int>((int)waypoint_position.y - minimap_y - 48, -48, 48);
		waypoint_pointer.setPosition(win_x - 52 * scale + waypoint_minimap_pos_x * 0.8 * scale, 52 * scale + waypoint_minimap_pos_y * 0.8 * scale);
	}

	if (quest_location.x > -1) {
		int quest_minimap_pos_x = clamp<int> (quest_location.x / 8 - minimap_x - 48, -48, 48);
		int quest_minimap_pos_y = clamp<int> (quest_location.y / 8 - minimap_y - 48, -48, 48);
		quest_pointer.setPosition(win_x - 52 * scale + quest_minimap_pos_x * 0.8 * scale, 52 * scale + quest_minimap_pos_y * 0.8 * scale);
	}
}

void GameState::render_minimap()
{
	window->draw(minimap);
	window->draw(minimap_frame);

	if(quest_location.x > -1)
		window->draw(quest_pointer);

	if (waypoint_position.x > -1)
		window->draw(waypoint_pointer);

	window->draw(player_pointer);
}

void GameState::entity_spawning()
{
	if (DoEntitySpawning) {
		//cout << "not my lucky day\n";
		screen_length = win_x / (16 * scale), screen_height = win_y / (16 * scale);
		short spawn_rect_x = screen_length + spawn_dist, spawn_rect_y = screen_height + spawn_dist;
		spawn_total = rand() % (spawn_rect_x * 2 + spawn_rect_y * 2);
		if (spawn_total > 2 * spawn_rect_x + spawn_rect_y) {
			spawn_x = 0, spawn_y = 2 * spawn_rect_y - (spawn_total - 2 * spawn_rect_x);
		}
		else if (spawn_total > spawn_rect_x + spawn_rect_y) {
			spawn_x = 2 * spawn_rect_x - (spawn_total - spawn_rect_y), spawn_y = spawn_rect_y;
		}
		else if (spawn_total > spawn_rect_x) {
			spawn_x = spawn_rect_x, spawn_y = spawn_total - spawn_rect_x;
		}
		else {
			spawn_x = spawn_total, spawn_y = 0;
		}

		spawn_x += x_offset - spawn_dist, spawn_y += y_offset - spawn_dist;

		bool valid_spawn = 1;

		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (spawn_x + i < 0 || spawn_x >= size_x || spawn_y < 0 || spawn_y >= size_y || static_map[spawn_x + i][spawn_y + j].tile_props & 2) {
					valid_spawn = 0;
					break;
				}
			}
		}

		if (valid_spawn) {
			//cout << "ONE PUUUUUUUNCH\n";
			enemies.add(wolf(0), { 16.f * spawn_x, 16.f * spawn_y });
			enemies.entities[enemies.curr_idx - 1]->update();
			//cout <<enemies.entities[enemies.curr_idx - 1]->getRelativePos().x <<' ' << enemies.entities[enemies.curr_idx - 1]->getRelativePos().y << '\n';
		}
	}
}

bool GameState::entity_in_range(Vector2f cords, short offset)
{
	return(cords.y >= -map_y - entity_render_distance &&
		cords.y <= -map_y + entity_render_distance + win_y / scale &&
		cords.x >= -map_x - entity_render_distance &&
		cords.x <= -map_x + entity_render_distance + win_x / scale);
}

void GameState::block_interactions_list(Vector2i interaction_tile)
{
	//cout << interaction_tile.x << ' ' << interaction_tile.y << '\n';
	if (current_map == "Sheraton") {
		if (interaction_tile.x == 46 && (interaction_tile.y == 25 || interaction_tile.y == 26)) {
			if (!dynamic_rendering.empty())
				dynamic_rendering.clear();
			dynamic_map.delete_all();
			initial_game("Doz World", { 264, 264 });
			DoDayLightCycle = false;
			DoEntitySpawning = false;
		}
		//else if (interaction_tile.x == 46 && (interaction_tile.y == 25 || interaction_tile.y == 26)) {
		//	states->insert(WorldMapST);
		//	states->at(WorldMapID)->update();
		//}
	}
}

void GameState::DayLightCycle()
{
	int count = 0;
	for (auto i = light_sources.lower_bound(-map_y - 160); i != light_sources.end() && i->first <= -map_y + win_y / scale + 160; i++) {
		if (i->second.position.x > -map_x - 160 && i->second.position.x < -map_x + win_x + 160) {
			shader.setUniform("lights[" + to_string(count) + "].position", (i->second.position + Vector2f(map_x, map_y)) * scale);
			shader.setUniform("lights[" + to_string(count) + "].color", i->second.color);
			shader.setUniform("lights[" + to_string(count) + "].intensity", i->second.intensity);
			count++;
		}
	}
	light_level += day_increment * dt;
	if (!night && light_level < 0.1) {
		night = true;
		light_level = 0.1;
		day_increment = -day_increment;
	}
	else if (night && light_level > 1) {
		night = false;
		light_level = 1;
		day_increment = -day_increment;
	}
	if(DoDayLightCycle)
		shader.setUniform("ambient_light", Glsl::Vec4(light_level, light_level, (light_level + 0.2 < 1 ? light_level + 0.2 : 1), 1.0));
	else
		shader.setUniform("ambient_light", Glsl::Vec4(constant_light_level, constant_light_level, constant_light_level + 0.1, 1.0));
	shader.setUniform("lightsCount", count);
}

void GameState::update_minimap_tile(Vector2i position, Vector3i tile)
{
	Uint8 pixels[16];
	int old_alpha = 0, tile_alpha = 0, sum_alpha = 0;
	Color tile_color;
	for (int k = 0; k < 2; k++)
		for (int l = 0; l < 2; l++) {
			pixels[(k + 2 * l) * 4]     = minimap_img.getPixel(position.x + k, position.y + l).r; // red
			pixels[(k + 2 * l) * 4 + 1] = minimap_img.getPixel(position.x + k, position.y + l).g; // green
			pixels[(k + 2 * l) * 4 + 2] = minimap_img.getPixel(position.x + k, position.y + l).b; // blue
			pixels[(k + 2 * l) * 4 + 3] = minimap_img.getPixel(position.x + k, position.y + l).a; // alpha

			tile_color = tile_sheets_img[tile.z].getPixel(tile.x * 16 + 5 + k * 6, tile.y * 16 + 5 + l * 6);
			old_alpha = (float)pixels[(k + 2 * l) * 4 + 3] / 255.0, tile_alpha = (float)tile_color.a / 255.0;
			sum_alpha = 1.0 - (1.0 - tile_alpha) * (1.0 - old_alpha);
			pixels[(k + 2 * l) * 4 + 3] = sum_alpha * 255;
			if (sum_alpha) {
				pixels[(k + 2 * l) * 4]     = tile_color.r * tile_alpha / sum_alpha + pixels[(k + 2 * l) * 4] * old_alpha * (1 - tile_alpha) / sum_alpha;
				pixels[(k + 2 * l) * 4 + 1] = tile_color.g * tile_alpha / sum_alpha + pixels[(k + 2 * l) * 4 + 1] * old_alpha * (1 - tile_alpha) / sum_alpha;
				pixels[(k + 2 * l) * 4 + 2] = tile_color.b * tile_alpha / sum_alpha + pixels[(k + 2 * l) * 4 + 2] * old_alpha * (1 - tile_alpha) / sum_alpha;
			}
			else {
				pixels[(k + 2 * l) * 4]     = tile_color.r;
				pixels[(k + 2 * l) * 4 + 1] = tile_color.g;
				pixels[(k + 2 * l) * 4 + 2] = tile_color.b;
				pixels[(k + 2 * l) * 4 + 3] = tile_color.a;
			}
			minimap_img.setPixel(position.x + k, position.y + l, Color(pixels[(k + 2 * l) * 4], pixels[(k + 2 * l) * 4 + 1], pixels[(k + 2 * l) * 4 + 2], pixels[(k + 2 * l) * 4 + 3]));
		}
	minimap_tex.update(pixels, 2, 2, position.x, position.y);
}

void GameState::check_in_inventory(int item_id)
{
	if (!inventory_count[item_id]) {
		inventory_order.erase(item_id);
	}
}

GameState::GameState(int character_id, string current_map, Vector2f player_pos, string character_name, int save_num, int health, double curr_game_time)
	: items(50)
{
	window->setMouseCursorVisible(false);
	this->character_name = character_name;
	this->character_id = character_id, this->save_num = save_num;
	win_x = window->getSize().x, win_y = window->getSize().y;
	if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
	else scale = win_y / 304.5;
	if (!shader.loadFromFile("VertexShader.shader", "FragmentShader.shader")) {
		cout << "Couldn't load shaders\n";
	}

	game_time = curr_game_time;
	initial_tile_sheets("game/tiles");
	initial_textures("game");
	hotbar.setTexture(*textures[0]);
	hotbar_selection.setTexture(*textures[1]);
	hotbar.setOrigin(hotbar.getLocalBounds().width / 2, hotbar.getLocalBounds().height / 2);
	hotbar_selection.setOrigin(25, hotbar_selection.getLocalBounds().height / 2);
	tool_icons[0].setTexture(*textures[2]);
	tool_icons[1].setTexture(*textures[3]);
	tool_icons[2].setTexture(*textures[4]);
	minimap_frame.setTexture(*textures[6]);
	minimap_frame.setOrigin(minimap_frame.getLocalBounds().width / 2, minimap_frame.getLocalBounds().height / 2);

	player_pointer.setFillColor(Color(255, 0, 0, 255));
	waypoint_pointer.setFillColor(Color(0, 0, 255, 255));
	quest_pointer.setFillColor(Color(255, 200, 0, 255));

	for (int i = 0; i < 3; i++) {
		tool_icons[i].setOrigin(tool_icons[i].getLocalBounds().width / 2, tool_icons[i].getLocalBounds().height / 2);
		tool_icons[i].setColor(Color(130, 130, 130));
	}

	initial_stats();
	load_maps(); //loads all maps ( pins[name]  { world map location x, world map location y, size x, size, y })
	load_entities(player_pos.y);
	player_entity->health = (health == -1) ? player_stats.max_health : health;
	health_indicator.setTexture(*textures[5]);

	health_indicator.setTextureRect(IntRect(0, ceil(player_entity->health * 10 / player_stats.max_health) * 100, 590, 100));
	health_indicator.setOrigin(0, health_indicator.getLocalBounds().height / 2);
	initial_game(current_map, player_pos);
	player_entity->setObjectStats(object_stats, &destructable_objects, item_drops, &item_drops_count);
	health_indicator.setTexture(*textures[5]);
	player_entity->setObjectStats(object_stats, &destructable_objects, item_drops, &item_drops_count);
}

GameState::~GameState()
{
	deload_map();

	if (destructable_objects != nullptr)
		delete destructable_objects;

	if (player_entity != nullptr)
		delete player_entity;
}

void GameState::update()
{
	window->setMouseCursorVisible(false);

	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
		else scale = win_y / 304.5;
		/////////////////////
		center_cam(player_entity->getRelativePos());
		hotbar.setScale(scale * 0.1, scale * 0.1);
		hotbar.setPosition(win_x/2, win_y - 20 * scale);
		hotbar_selection.setScale(scale * 0.1, scale * 0.1);
		health_indicator.setPosition(20 * scale , 20 * scale);
		health_indicator.setScale(scale * 0.15, scale * 0.15);
		blackscreen.setSize({ win_x, win_y });
		for (int i = 0; i < 3; i++) {
			tool_icons[i].setPosition(win_x / 2 + 3*scale, win_y - 20 * scale);
			tool_icons[i].setScale(scale * 0.1, scale * 0.1);
		}
		hotbar_selection.setPosition(win_x / 2 - (hotbar.getLocalBounds().width / 2 - 12) * scale * 0.1 + (3 - player_entity->state) * 248 * scale * 0.1, win_y - 20 * scale);
		minimap_frame.setPosition(win_x - 52 * scale,52 * scale);
		minimap_frame.setScale(scale * 0.96, scale * 0.96);
		minimap.setPosition(win_x - 52 * scale, 52 * scale);
		minimap.setScale(scale * 0.8, scale * 0.8);

		player_pointer.setRadius(1.5 * scale);
		player_pointer.setOrigin(0.75 * scale, 0.75 * scale);

		waypoint_pointer.setRadius(1.5 * scale);
		waypoint_pointer.setOrigin(0.75 * scale, 0.75 * scale);

		quest_pointer.setRadius(1.5 * scale);
		quest_pointer.setOrigin(0.75 * scale, 0.75 * scale);

		shader.setUniform("ratio", win_x / win_y);
	}

	if (fps_active)
		fps_text.setString(fps_text.getString() + "\tCoordinates: " + to_string(int(player_entity->getRelativePos().x / 16)) + ' ' + to_string(int(player_entity->getRelativePos().y / 16)));

	black_in();


	//entity spawning                *******FIX RANDOM DAMAGE WITH ENEMY SPAWNING*****
	spawn_cd += dt;
	if (spawn_cd >= def_spawn_cd) {
		spawn_cd = 0;
		if (!(rand() % 3))
			entity_spawning();
	}

	player_entity->movement = delta_movement();

	if (player_entity->combat_tag <= 0 && player_entity->health < player_stats.max_health && heal_delay >= 2) {
		heal_delay = 0;
		player_entity->health += 10;
	}
	else {
		if(player_entity->combat_tag > 0)
			player_entity->combat_tag -= dt;
		else
			heal_delay += dt;
	}
	health_indicator.setTextureRect(IntRect(0, ceil(player_entity->health * 10 / player_stats.max_health) * 100, 590, 100));

	if (destroy_object_location.x >= 0) {
		destroyANDrestore_objects(destroy_object_location, 1);
		destroy_object_location = { -1, -1};
	}

	update_minimap();

	DayLightCycle();

	if (enemies.vis == nullptr) {
		enemies.vis = new short* [enemies.find_size_x];
		for (int i = 0; i < enemies.find_size_x; i++) {
			(enemies.vis)[i] = new short[enemies.find_size_y]({});
		}
	}
	for (int i = 0; i < enemies.curr_idx; i++) {
		if (!enemies.entities[i]->despawn) {
			if (enemies.entities[i]->action_state != 0 || entity_in_range(enemies.entities[i]->pos, entity_update_distance))
				enemies.entities[i]->update();
		}
		if(enemies.entities[i]->despawn) {
			effects.add({ 400,0,100,100 }, 20, { int(enemies.entities[i]->getRelativePos().x) , int(enemies.entities[i]->getRelativePos().y) }, "break_animation", 0.9, Color(136, 8, 8, 240), 0, map_x, map_y);
			enemies.rem_ove(i);
		}
	}

	for (int i = 0; i < passive.curr_idx; i++) {
		if (!passive.entities[i]->despawn) {
			if (passive.entities[i]->action_state != 0 || entity_in_range(passive.entities[i]->pos, entity_update_distance))
				passive.entities[i]->update();
		}
		if(passive.entities[i]->despawn) {
			effects.add({ 400,0,100,100 }, 20, { int(passive.entities[i]->getRelativePos().x) , int(passive.entities[i]->getRelativePos().y) }, "break_animation", 0.9, Color(136, 8, 8, 240), 0, map_x, map_y);
			passive.rem_ove(i);
		}
	}

	for (int i = 0; i < NPCs.curr_idx; i++) {
		if (!NPCs.entities[i]->despawn) {
			if (NPCs.entities[i]->action_state != 0 || entity_in_range(NPCs.entities[i]->pos, entity_update_distance))
				NPCs.entities[i]->update();
		}
		if(NPCs.entities[i]->despawn) {
			effects.add({ 400,0,100,100 }, 20, { int(NPCs.entities[i]->getRelativePos().x) , int(NPCs.entities[i]->getRelativePos().y) }, "break_animation", 0.9, Color(136, 8, 8, 240), 0, map_x, map_y);
			NPCs.rem_ove(i);
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
	if (!player_entity->despawn) {
		no_update = 0;
		player_entity->update();
	}
	else {
		health_indicator.setTextureRect(IntRect(0, 0, 590, 100));
		states->insert({ DialogueID,new DialogueState(death_message,{win_x / 2,win_y / 2},scale / 2,2) });
		states->at(DialogueID)->update();
		no_update++;
		if (no_update>=2) {
			string file_name = "Saves/Save" + to_string(save_num + 1) + ".ini";
			remove(file_name.c_str());
			states->insert(MainMenuST);
			states->at(MainMenuID)->update();
			if (states->find(BackgroundID) == states->end()) {
				states->insert(BackgroundST);
				states->at(BackgroundID)->update();
			}

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

	player_entity->interact = 0;

	for (int i = 0; i < items.curr_idx; i++) {

		if(!items.entities[i]->despawn){
			if(entity_in_range(items.entities[i]->pos, entity_update_distance))
				items.entities[i]->update();
		}
		if (items.entities[i]->despawn) {

            // add item to player_inventory
			if (!inventory_count[items.entities[i]->item_ID])
				inventory_order.add(items.entities[i]->item_ID);
			inventory_count[items.entities[i]->item_ID]++;
			// despawn item
			items.rem_ove(i);
			i--;
			
		}
		
	}
	game_time += dt;
}

void GameState::render()
{
	render_static_map();
	render_entities();
	if(states->rbegin()->first == GameID || states->rbegin()->first == InventoryID || states->rbegin()->first == NotificationID) {
		window->draw(hotbar);
		for (int i = 0; i < 3; i++)
			window->draw(tool_icons[i]);
		window->draw(hotbar_selection);
		window->draw(health_indicator);
		render_minimap();
	}
	if (blackining)
		window->draw(blackscreen);
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
				states->insert(PauseST);
				states->at(PauseID)->update(); 
				return; break;
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
					ofs << player_entity->getRelativePos().x << ' ' << player_entity->getRelativePos().y << '\n';
					ofs << player_entity->health << '\n';
					ofs << game_time << '\n';
				}
				ofs.close();
			}

					break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			case Keyboard::Num1:
				player_entity->change_state(3);
				hotbar_selection.setPosition(win_x / 2 - (hotbar.getLocalBounds().width / 2 - 12) * scale * 0.1 + (3 - player_entity->state) * 248 * scale * 0.1, win_y - 20 * scale);
				tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(130, 130, 130));
				break;
			case Keyboard::Num2:
				player_entity->change_state(2);
				hotbar_selection.setPosition(win_x / 2 - (hotbar.getLocalBounds().width / 2 - 12) * scale * 0.1 + (3 - player_entity->state) * 248 * scale * 0.1, win_y - 20 * scale);
				tool_icons[0].setColor(Color(255, 255, 255)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(130, 130, 130));
				break;
			case Keyboard::Num3:
				player_entity->change_state(1);
				hotbar_selection.setPosition(win_x / 2 - (hotbar.getLocalBounds().width / 2 - 12) * scale * 0.1 + (3 - player_entity->state) * 248 * scale * 0.1, win_y - 20 * scale);
				tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(255, 255, 255)), tool_icons[2].setColor(Color(130, 130, 130));
				break;
			case Keyboard::Num4:
				player_entity->change_state(0);
				hotbar_selection.setPosition(win_x / 2 - (hotbar.getLocalBounds().width / 2 - 12) * scale * 0.1 + (3 - player_entity->state) * 248 * scale * 0.1, win_y - 20 * scale);
				tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(255, 255, 255));
				break;
			case Keyboard::E:
				states->insert({ InventoryID, new InventoryState(&inventory_order, inventory_count)});
				states->at(InventoryID)->update();
				break;
			case Keyboard::M:
				states->insert({ MiniMapID, new MiniMapState(minimap_tex, player_entity->getRelativePos(), waypoint_position, quest_location) });
				states->at(MiniMapID)->update();
				break;
			case Keyboard::F: {
				player_entity->interact = 1;
				Vector2i interaction_block = player_entity->block_interaction();
				if (interaction_block.x != -1) {
					block_interactions_list(interaction_block);
				}
				break;
			}
			case Keyboard::Space:
				player_entity->use_tool();
				if (player_entity->tool_used_on.x > -1) {
					Vector3i tile_info = static_map[player_entity->tool_used_on.x/16][player_entity->tool_used_on.y / 16].layers[static_map[player_entity->tool_used_on.x / 16][player_entity->tool_used_on.y / 16].size - 1];
					Color tile_color = tile_sheets_img[tile_info.z].getPixel(tile_info.x * 16 + 8, tile_info.y * 16 + 6);
					effects.add({ 0,0,100,100 }, 24, { player_entity->tool_used_on.x , player_entity->tool_used_on.y }, "break_animation", 0.6, tile_color, 0, map_x, map_y);
					if (item_drops_count != -1) {
						Vector3i temp;
						for (int i = 0; i < item_drops_count; i++) {
							items.add(1, item_stats, 0, static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, destroy_object_location, player_entity, { (float)player_entity->tool_used_on.x , (float)player_entity->tool_used_on.y }, 0, 300.0, item_drops[i]);
						}
						item_drops_count = -1;
					}
					player_entity->tool_used_on.x = -1;
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
				int new_state = player_entity->state + event.mouseWheel.delta;
				if (new_state > 3) new_state = 3;
				else if (new_state < 0) new_state = 0;
				player_entity->change_state(new_state);
				hotbar_selection.setPosition(win_x / 2 - (hotbar.getLocalBounds().width / 2 - 12) * scale * 0.1 + (3 - player_entity->state) * 248 * scale * 0.1, win_y - 20 * scale);
				tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(130, 130, 130));
				if(new_state!=3)
					tool_icons[(2 - player_entity->state)].setColor(Color(255, 255, 255));
			}
		}
	}
}