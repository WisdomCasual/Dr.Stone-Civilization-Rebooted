#include "GameState.h"
#include "MiniMapState.h"

void GameState::black_in(float fade_speed)
{
	if (blackining > 0) {
		if (blackining - fade_speed * dt < 0)
			blackining = 0;
		else
			blackining -= fade_speed * dt;
		blackscreen.setFillColor(Color(0, 0, 0, (int)blackining));
	}
}

bool GameState::black_out(float fade_speed)
{
	if (blackining < 255) {
		if (blackining + fade_speed * dt > 255)
			blackining = 255;
		else
			blackining += fade_speed * dt;
		blackscreen.setFillColor(Color(0, 0, 0, (int)blackining));
		return false;
	}
	return true;
}

void GameState::save()
{



	ofstream ofs("Saves/Save " + to_string(save_num + 1) + "/Save.dat", ofstream::out, ofstream::trunc);

	if (ofs.is_open()) {
		ofs << character_name << '\n';
		ofs << (int)character_id << '\n';
		ofs << current_quest << '\n';
		ofs << current_map << '\n';
		ofs << player_entity->getRelativePos().x << ' ' << player_entity->getRelativePos().y << '\n';
		ofs << player_entity->health << '\n';
		ofs << game_time << '\n';
		ofs << light_level << ' ' << day_increment << '\n';
		ofs << quest_idx << '\n';

		ofs << inventory_order.size << '\n';

		nod* it = inventory_order.first;
		while (it != nullptr) {
			ofs << it->itm << ' ';
			ofs << inventory_count[it->itm] << ' ';
			it = it->link;
		}

		ofs << '\n';
	}
	ofs.close();
	

	ofstream map_ofs("Saves/Save " + to_string(save_num + 1) + "/" + current_map + ".mp", ofstream::out, ofstream::trunc);

	if (map_ofs.is_open()) {

		//save static map
		map_ofs << size_x << ' ' << size_y << '\n';
		map_ofs << respawn_point.x << ' ' << respawn_point.y << '\n';
		map_ofs << waypoint_position.x << ' ' << waypoint_position.y << '\n';

		for (int i = 0; i < size_x; i++)
			for (int j = 0; j < size_y; j++) {
				map_ofs << static_map[i][j].size << ' ';
				for (int k = 0; k < static_map[i][j].size; k++)
					map_ofs << static_map[i][j].layers[k].x << ' ' << static_map[i][j].layers[k].y << ' ' << static_map[i][j].layers[k].z << ' ';

				map_ofs << static_map[i][j].destruction_time << ' '
					    << static_map[i][j].object_ID << ' ' << static_map[i][j].tile_props << ' ' 
					    << static_map[i][j].time << ' ' << static_map[i][j].tool_type << ' ';
			}
		map_ofs << '\n';


		map_ofs << dynamic_map.curr_idx << '\n';

		for (int i = 0; i < dynamic_map.curr_idx; i++) {

			map_ofs << dynamic_map.at[i].at[0].position.x << ' ' << dynamic_map.at[i].at[0].position.y << ' ';

			map_ofs << dynamic_map.at[i].at[0].tile.x << ' ' << dynamic_map.at[i].at[0].tile.y << ' ' << dynamic_map.at[i].at[0].tile.z << ' ';

			map_ofs << dynamic_map.at[i].layer << ' ';

			map_ofs << dynamic_map.at[i].core_location.x << ' ' << dynamic_map.at[i].core_location.y << ' ';
		}


		for (int i = 0; i < dynamic_map.curr_idx; i++) {
			map_ofs << dynamic_map.at[i].curr_idx << ' ';
			for (int j = 1; j < dynamic_map.at[i].curr_idx; j++) {
				map_ofs << dynamic_map.at[i].at[j].position.x << ' ' << dynamic_map.at[i].at[j].position.y << ' ';
				map_ofs << dynamic_map.at[i].at[j].tile.x << ' ' << dynamic_map.at[i].at[j].tile.y << ' ' << dynamic_map.at[i].at[j].tile.z << ' ';
			}
		}

		map_ofs << '\n';


		map_ofs << destructable_count << '\n';
		for (int i = 0; i < destructable_count; i++) {
			map_ofs << destructable_objects[i].damage << ' ' << destructable_objects[i].drops_no << ' ' 
				    << destructable_objects[i].health << ' ' << destructable_objects[i].idx << ' ' 
				    << destructable_objects[i].speed << ' ';
			for (int j = 0; j < destructable_objects[i].drops_no; j++)
				map_ofs << destructable_objects[i].item_drops[j] << ' ';
		}
		map_ofs << '\n';

		map_ofs << items.curr_idx << '\n';

		for (int i = 0; i < items.curr_idx; i++) {
			map_ofs << items.entities[i]->id << ' ';
			map_ofs << items.entities[i]->pos.x << ' ' << items.entities[i]->pos.y << ' ';
			map_ofs << items.entities[i]->despawn_timer << ' ';
			map_ofs << items.entities[i]->health << ' ';
			map_ofs << items.entities[i]->persistant << ' ';
		}
		map_ofs << '\n';

		map_ofs << light_sources.size() << '\n';
		for (auto const &light : light_sources) {
			map_ofs << light.second.color.x << ' ' << light.second.color.y << ' ' << light.second.color.z << ' ';
			map_ofs << light.second.intensity << ' ';
			map_ofs << light.second.position.x << ' ' << light.second.position.y << ' ';
			map_ofs << light.second.day_light << ' ';
		}
		map_ofs << '\n';



		map_ofs << enemies.curr_idx << '\n';

		for (int i = 0; i < enemies.curr_idx; i++) {
			map_ofs << enemies.entities[i]->id << ' ';
			map_ofs << enemies.entities[i]->pos.x << ' ' << enemies.entities[i]->pos.y << ' ';
			map_ofs << enemies.entities[i]->aStarID << ' ';
			map_ofs << enemies.entities[i]->current_move << ' ';
			map_ofs << enemies.entities[i]->despawn_timer << ' ';
			map_ofs << enemies.entities[i]->health << ' ';
			map_ofs << enemies.entities[i]->persistant << ' ';
		}
		map_ofs << '\n';



		map_ofs << passive.curr_idx << '\n';

		for (int i = 0; i < passive.curr_idx; i++) {
			map_ofs << passive.entities[i]->id << ' ';
			map_ofs << passive.entities[i]->pos.x << ' ' << passive.entities[i]->pos.y << ' ';
			map_ofs << passive.entities[i]->current_move << ' ';
			map_ofs << passive.entities[i]->despawn_timer << ' ';
			map_ofs << passive.entities[i]->health << ' ';
			map_ofs << passive.entities[i]->persistant << ' ';
		}
		map_ofs << '\n';


		map_ofs << NPCs.curr_idx << '\n';

		for (int i = 0; i < NPCs.curr_idx; i++) {
			map_ofs << NPCs.entities[i]->id << ' ';
			map_ofs << NPCs.entities[i]->pos.x << ' ' << NPCs.entities[i]->pos.y << ' ';
			map_ofs << NPCs.entities[i]->current_move << ' ';
			map_ofs << NPCs.entities[i]->despawn_timer << ' ';
			map_ofs << NPCs.entities[i]->health << ' ';
			map_ofs << NPCs.entities[i]->persistant << ' ';
			map_ofs << NPCs.entities[i]->npc_type << ' ';
			map_ofs << NPCs.entities[i]->NPC_curr_movement.x << ' ' << NPCs.entities[i]->NPC_curr_movement.y <<' ';
			map_ofs << NPCs.entities[i]->NPC_prev_tile_x << ' ' << NPCs.entities[i]->NPC_prev_tile_y <<' ';
		}

		map_ofs << '\n';

	}
	map_ofs.close();
}

void GameState::initial_sounds()
{
	//loads the current state's Sounds files
	string audio_name;
	ifstream ifs("Audio/Game Sounds/soundnames.ini");
	if (ifs.is_open()) {
		SoundBuffer temp_buffers[150];
		int sounds_count = 0;
		while (!ifs.eof()) {
			getline(ifs, audio_name);
			temp_buffers[sounds_count].loadFromFile("Audio/Game Sounds/" + audio_name + ".ogg");
			sounds_count++;
		}
		sounds_no = sounds_count;
		sound_buffers = new SoundBuffer [sounds_count];

		for (int i = 0; i < sounds_count; i++)
		{
			sound_buffers[i] = temp_buffers[i];
		}
		//delete[] temp_buffers;
		//delete[] temp_sounds;
	}
	ifs.close();
	globalvar::dtclock.restart();
}

void GameState::load_game()
{
	ifstream ifs("Saves/Save " + to_string(save_num + 1) + "/Save.dat");

	int health = -1;
	Vector2f player_pos;

	ifs.seekg(ios::beg);
	if (ifs.is_open()) {
		getline(ifs, character_name);              // <-- name
		ifs >> character_id;				  	   // <-- selected character ID
		ifs >> current_quest;					   // <-- current quest
		ifs.ignore();									   
		getline(ifs, current_map);                 // <-- current map
		ifs >> player_pos.x >> player_pos.y;	   // <-- current position
		ifs >> health;							   // <-- current health
		ifs >> game_time;						   // <-- current game time
		ifs >> light_level >> day_increment;	   // <-- current light level - time increments
		ifs >> quest_idx;						   // <-- current quest index
		
		int itm, count;
		ifs >> count;                              // <-- current number of items in inventory
		for (int i = 0; i < count; i++) {
			ifs >> itm;                            // <-|
			ifs >> inventory_count[itm];           // <-| inventory items
			inventory_order.add(itm);              // <-|
		}
	}
	ifs.close();

	
	initial_stats();
	load_maps(); //loads all maps ( pins[name]  { location in world map x, location in world map y, size x, size, y })
	load_entities(player_pos.y);
	initial_game(current_map, player_pos);

	player_entity->setObjectStats(object_stats, &destructable_objects, item_drops, &item_drops_count);
	player_entity->health = (health == -1) ? player_stats.max_health : health;
}

void GameState::set_textures()
{
	hotbar.setTexture(*textures[0]);
	hotbar_selection.setTexture(*textures[1]);
	hotbar.setOrigin(hotbar.getLocalBounds().width / 2, hotbar.getLocalBounds().height / 2);
	hotbar_selection.setOrigin(25, hotbar_selection.getLocalBounds().height / 2);
	tool_icons[0].setTexture(*textures[2]);
	tool_icons[1].setTexture(*textures[3]);
	tool_icons[2].setTexture(*textures[4]);
	tool_icons[3].setTexture(*textures[8]);
	minimap_frame.setTexture(*textures[6]);
	minimap_frame.setOrigin(minimap_frame.getLocalBounds().width / 2, minimap_frame.getLocalBounds().height / 2);

	player_pointer.setFillColor(Color(255, 0, 0, 255));
	waypoint_pointer.setFillColor(Color(0, 0, 255, 255));
	quest_pointer.setFillColor(Color(255, 200, 0, 255));

	health_indicator.setTextureRect(IntRect(0, int(ceil(player_entity->health * 10 / player_stats.max_health) * 100), 590, 100));
	health_indicator.setOrigin(0, health_indicator.getLocalBounds().height / 2);
	health_indicator.setTexture(*textures[5]);

	for (int i = 0; i < 4; i++) {
		tool_icons[i].setOrigin(tool_icons[i].getLocalBounds().width / 2, tool_icons[i].getLocalBounds().height / 2);
		tool_icons[i].setColor(Color(130, 130, 130));
	}
}

void GameState::load_initial_map(string map_name)
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
	bool** vis[4];
	short layer_prop = 0;

	Vector2i pre_disabled_objects[200];
	short pre_disabled_count = 0;

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
								light_sources.insert({ j * 16.f + 8.f, light(Vector2f(i * 16.f + 8.f, j * 16.f + 8.f), Vector3f(1.f, 1.f, 1.f), intensity) });
							else if (tle.x < 6)
								light_sources.insert({ j * 16.f + 8.f, light(Vector2f(i * 16.f + 8.f, j * 16.f + 8.f), Vector3f(1.f, 1.f, 0.5f), intensity) });
							else if (tle.x < 9)
								light_sources.insert({ j * 16.f + 8.f, light(Vector2f(i * 16.f + 8.f, j * 16.f + 8.f), Vector3f(1.f, 0.5f, 0.5f), intensity) });
							else if (tle.x < 12)
								light_sources.insert({ j * 16.f + 8.f, light(Vector2f(i * 16.f + 8.f, j * 16.f + 8.f), Vector3f(0.5f, 0.5f, 1.f), intensity) });
							else if (tle.x < 15)
								light_sources.insert({ j * 16.f + 8.f, light(Vector2f(i * 16.f + 8.f, j * 16.f + 8.f), Vector3f(0.5f, 1.f, 0.5f), intensity) });
							else if (tle.x == 15)
								light_sources.insert({ j * 16.f + 8.f, light(Vector2f(i * 16.f + 8.f, j * 16.f + 8.f), Vector3f(1.f, 1.f, 1.f), 0.5f, true) });
						}
						if (tle.y == 17 && tle.z == 3) {
							if (tle.x == 0)
								respawn_point = {i * 16.0f + 8.0f, j * 16.0f + 8.0f};
						}
					}
					else {
						if (layer_prop & 32 && layer_prop & 128) {
							pre_disabled_objects[pre_disabled_count] = { i, j };
							pre_disabled_count++;
						}
						if (layer_prop & 32 && tile_props[tle.z].properties[tle.x][tle.y].object_type > -1) {
							static_map[i][j].tool_type = tile_props[tle.z].properties[tle.x][tle.y].tool_type;
							static_map[i][j].object_ID = destructable_count;
							temp_destructable[destructable_count] = tile_props[tle.z].properties[tle.x][tle.y].object_type;
							destructable_count++;
						}
						if (layer_prop & 16) { // front core
							update_minimap_tile(Vector2i(i * 2, j * 2), tle);
							dynamic_objects objct;
							objct.add({ Vector2f((float)i, (float)j), tle });
							objct.layer = layr;
							dynamic_map.add(objct);
							dynamic_rendering.insert({ float((j + 1) * 16), {short(dynamic_map.curr_idx - 1), nullptr} });
							static_map[i][j].dynamic_idx = dynamic_map.curr_idx - 1;
							//dynamic_map.at[dynamic_map.curr_idx - 1].time = &zero;
							//dynamic_map.at[dynamic_map.curr_idx - 1].destruction_time = &zero;
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
		search_front((int)dynamic_map.at[i].at[0].position.x, (int)dynamic_map.at[i].at[0].position.y, dynamic_map.at[i].layer, temp_front, vis, i);
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


	for (int i = 0; i < pre_disabled_count; i++)
		destroyANDrestore_objects(pre_disabled_objects[i], true, true);

	initial_entities();
	waypoint_position = { -1, -1 };
	minimap_tex.loadFromImage(minimap_img);
}

void GameState::load_saved_map(string map_name)
{
	ifstream ifs("Saves/Save " + to_string(save_num + 1) + "/" + map_name + ".mp");
	if (ifs.is_open()) {

		//save static map
		ifs >> size_x >> size_y;

		ifs >> respawn_point.x >> respawn_point.y;

		ifs >> waypoint_position.x >> waypoint_position.y;

		static_map = new render_tile * [size_x];

		for (int i = 0; i < size_x; i++) {
			static_map[i] = new render_tile[size_y];
			for (int j = 0; j < size_y; j++) {
				ifs >> static_map[i][j].size;

				static_map[i][j].layers = new Vector3i[static_map[i][j].size];

				for (int k = 0; k < static_map[i][j].size; k++)
					ifs >> static_map[i][j].layers[k].x >> static_map[i][j].layers[k].y >> static_map[i][j].layers[k].z;

				ifs >> static_map[i][j].destruction_time
					>> static_map[i][j].object_ID >> static_map[i][j].tile_props
					>> static_map[i][j].time >> static_map[i][j].tool_type;
				if(!static_map[i][j].destruction_time)
					update_minimap_tile(Vector2i(i * 2, j * 2), static_map[i][j].layers[static_map[i][j].size - 1]);
			}
		}




		int count;
		ifs >> count;


		for (int i = 0; i < count; i++) {
			dynamic_objects temp;
			Vector2f temp_pos; Vector3i temp_tile;

			ifs >> temp_pos.x >> temp_pos.y;
			ifs >> temp_tile.x >> temp_tile.y >> temp_tile.z;
			temp.add({ temp_pos, temp_tile });
			dynamic_map.add(temp);
			ifs >> dynamic_map.at[i].layer;
			ifs >> dynamic_map.at[i].core_location.x >> dynamic_map.at[i].core_location.y;
			dynamic_rendering.insert({ float((temp_pos.y + 1) * 16), {short(i), nullptr} });
			if (dynamic_map.at[i].core_location.x > -1) {
				dynamic_map.at[i].time = &static_map[dynamic_map.at[i].core_location.x][dynamic_map.at[i].core_location.y].time;
				dynamic_map.at[i].destruction_time = &static_map[dynamic_map.at[i].core_location.x][dynamic_map.at[i].core_location.y].destruction_time;
			}
			if(!dynamic_map.at[i].destruction_time)
				update_minimap_tile(Vector2i((int)temp_pos.x * 2, (int)temp_pos.y * 2), temp_tile);
			static_map[(int)temp_pos.x][(int)temp_pos.y].dynamic_idx = i;
		}

		for (int i = 0; i < count; i++) {

			int obj_count;
			ifs >> obj_count;

			for (int j = 1; j < obj_count; j++) {
				Vector2f temp_pos; Vector3i temp_tile;
				ifs >> temp_pos.x >> temp_pos.y;
				ifs >> temp_tile.x >> temp_tile.y >> temp_tile.z;
				dynamic_map.at[i].add({ temp_pos, temp_tile });
				if(!dynamic_map.at[i].destruction_time)
					update_minimap_tile(Vector2i((int)temp_pos.x * 2, (int)temp_pos.y * 2), temp_tile);
			}
		}


		ifs >> destructable_count;
		destructable_objects = new base_stats[destructable_count];
		for (int i = 0; i < destructable_count; i++) {
			ifs >> destructable_objects[i].damage >> destructable_objects[i].drops_no
				>> destructable_objects[i].health >> destructable_objects[i].idx
			    >> destructable_objects[i].speed;

			for (int j = 0; j < destructable_objects[i].drops_no; j++)
				ifs >> destructable_objects[i].item_drops[j];
		}


		ifs >> count;
		int id;
		Vector2f pos;
		for (int i = 0; i < count; i++) {
			ifs >> id;
			ifs >> pos.x >> pos.y;
			items.add(item_spawn(id), pos, 0, 300.f);
			ifs >> items.entities[items.curr_idx - 1]->despawn_timer;
			items.entities[items.curr_idx - 1]->interact = 1;
			ifs >> items.entities[items.curr_idx - 1]->health;
			ifs >> items.entities[items.curr_idx - 1]->persistant;
		}


		ifs >> count;
		for (int i = 0; i < count; i++) {
			Vector3f color; Vector2f position; float inten; bool day_light;
			ifs >> color.x >> color.y >> color.z;
			ifs >> inten;
			ifs >> position.x >> position.y;
			ifs >> day_light;
			light_sources.insert({ position.y, light(position, color, inten, day_light) });
		}



		ifs >> count;

		for (int i = 0; i < count; i++) {
			//cout << "you thought it was passive, but it was me, DIO\n";
			ifs >> id;
			ifs >> pos.x >> pos.y;
			enemies.add(enemy_spawn(id), pos);
			ifs >> enemies.entities[enemies.curr_idx - 1]->aStarID ;
			ifs >> enemies.entities[enemies.curr_idx - 1]->current_move;
			ifs >> enemies.entities[enemies.curr_idx - 1]->despawn_timer ;
			ifs >> enemies.entities[enemies.curr_idx - 1]->health ;
			ifs >> enemies.entities[enemies.curr_idx - 1]->persistant ;
		}



		ifs >> count;
		for (int i = 0; i < count; i++) {
			//cout << "you thought it was passive, and you are goddamn right\n";
			ifs >> id;
			ifs >> pos.x >> pos.y;
			passive.add(passive_spawn(id), pos);
			ifs >> passive.entities[passive.curr_idx - 1]->current_move;
			ifs >> passive.entities[passive.curr_idx - 1]->despawn_timer ;
			ifs >> passive.entities[passive.curr_idx - 1]->health ;
			ifs >> passive.entities[passive.curr_idx - 1]->persistant ;
		}



		ifs >> count;

		for (int i = 0; i < count; i++) {
			ifs >> id;
			ifs >> pos.x >> pos.y;
			NPCs.add(spawn_npc(id), pos);
			npc_initalize;
			ifs >> NPCs.entities[NPCs.curr_idx - 1]->current_move;
			ifs >> NPCs.entities[NPCs.curr_idx - 1]->despawn_timer ;
			ifs >> NPCs.entities[NPCs.curr_idx - 1]->health ;
			ifs >> NPCs.entities[NPCs.curr_idx - 1]->persistant ;
			ifs >> NPCs.entities[NPCs.curr_idx - 1]->npc_type ;
			ifs >> NPCs.entities[NPCs.curr_idx - 1]->NPC_curr_movement.x >> NPCs.entities[i]->NPC_curr_movement.y;
			ifs >> NPCs.entities[NPCs.curr_idx - 1]->NPC_prev_tile_x >> NPCs.entities[i]->NPC_prev_tile_y;
		}

	}
	ifs.close();

	minimap_tex.loadFromImage(minimap_img);
}

void GameState::search_front(int x, int y, int layr, Vector3i*** temp_front, bool*** vis, int idx)
{
	update_minimap_tile(Vector2i(x * 2, y * 2), temp_front[layr][x][y]);
	vis[layr][x][y] = 1;

	for (int i = 0; i < 4; i++) {
		int new_x = x + dx[i], new_y = y + dy[i];
		if (new_x < size_x && new_y < size_y && new_x >= 0 && new_y >= 0 && temp_front[layr][new_x][new_y].x && !vis[layr][new_x][new_y]) {
			temp_front[layr][new_x][new_y].x--;
			dynamic_map.at[idx].add({Vector2f((float)new_x, (float)new_y), temp_front[layr][new_x][new_y] });
			dynamic_map.at[idx].height += (!dx[i]) * abs(dy[i]);
			search_front(new_x, new_y, layr, temp_front, vis, idx);
		}
	}
}

void GameState::load_map(string map_name)
{
	deload_map();
	light_sources.clear();

	ifstream ifs("Saves/Save " + to_string(save_num + 1) + "/" + map_name + ".mp");
	string line;

	if (ifs >> line)
		load_saved_map(map_name);
	else
		load_initial_map(map_name);
	ifs.close();

	dtclock.restart();
}

void GameState::load_entities(float player_relative_y_pos)
{

	//player
	player_stats.is_player = 1;
	player_stats.animations = new animation * [5]({});
	player_stats.states_no = 4;
	player_stats.base_movement_speed = 130;
	player_stats.scale_const = 0.65f;
	player_stats.base_animation_speed = 16.6f;
	player_stats.textures_count = 5;
	player_stats.textures = new Texture*[player_stats.textures_count];
	player_stats.base_damage = 25;
	player_stats.max_health = 100;

	for (int i = 0; i < player_stats.textures_count; i++) {
		player_stats.textures[i] = new Texture;
		player_stats.textures[i]->loadFromFile("textures/game/entities/character " + to_string(character_id) + "/" + to_string(i) + ".png");
	}

	//Walking
	for (int i = 0; i <= 4; i++) {
		player_stats.animations[i] = new animation[16];
		player_stats.animations[i][0] = { 9, {0, 8 * 64, 64, 64}, {30,14}, {32,58} }; //back
		player_stats.animations[i][1] = { 9, {0, 11 * 64, 64, 64}, {30,14}, {32,58} }; //right
		player_stats.animations[i][2] = { 9, {0, 9 * 64, 64, 64}, {30,14}, {32,58} }; //left
		player_stats.animations[i][3] = { 9, {0, 10 * 64, 64, 64}, {30,14}, {32,58} }; //front
	}
	for (int i = 1; i <= 3; i++) {
		player_stats.animations[3][0 + i * 4] = { 6, {0, 1365 + (0 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //back
		player_stats.animations[3][1 + i * 4] = { 6, {0, 1365 + (3 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //right
		player_stats.animations[3][2 + i * 4] = { 6, {0, 1365 + (1 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //left
		player_stats.animations[3][3 + i * 4] = { 6, {0, 1365 + (2 + (i - 1) * 4) * 192, 192, 192}, {30,14}, {96,100} }; //front
	}
	for (int i = 1; i <= 4; i+=i) {
		player_stats.animations[i][4] = { 5, {0, 1365 + 0 * 128, 128, 128}, {30,14}, {64,70} }; //back
		player_stats.animations[i][5] = { 5, {0, 1365 + 3 * 128, 128, 128}, {30,14}, {64,70} }; //right
		player_stats.animations[i][6] = { 5, {0, 1365 + 1 * 128, 128, 128}, {30,14}, {64,70} }; //left
		player_stats.animations[i][7] = { 5, {0, 1365 + 2 * 128, 128, 128}, {30,14}, {64,70} }; //front
	}

	player_stats.buffers_count = 14;
	player_stats.soundbuffers = new SoundBuffer* [player_stats.buffers_count];

	for(int i = 0; i < player_stats.buffers_count; i++)
		player_stats.soundbuffers[i] = &sound_buffers[i];


	//animals
	//lion
	enemy_stats[0].animations = new animation * [1];
	enemy_stats[0].scale_const = 0.85f;
	enemy_stats[0].base_movement_speed = 80;
	enemy_stats[0].base_animation_speed = 12;
	enemy_stats[0].textures_count = 1;
	enemy_stats[0].textures = new Texture * [enemy_stats[0].textures_count];
	enemy_stats[0].textures[0] = new Texture;

	enemy_stats[0].item_drop_count = 2;
	enemy_stats[0].item_drops = new int[enemy_stats[0].item_drop_count];
	enemy_stats[0].item_drops[0] = 6;
	enemy_stats[0].item_drops[1] = 6;

	enemy_stats[0].textures[0]->loadFromFile("textures/game/entities/enemies/lion.png");

	enemy_stats[0].buffers_count = 9;
	enemy_stats[0].soundbuffers = new SoundBuffer * [enemy_stats[0].buffers_count];

	for (int i = 0; i < enemy_stats[0].buffers_count; i++)
		enemy_stats[0].soundbuffers[i] = &sound_buffers[24 + i];

	enemy_stats[0].states_no = 1;
	enemy_stats[0].animations[0] = new animation[4];
	enemy_stats[0].animations[0][0] = { 4, {0, 146, 48, 49}, {24,48}, {24,24} }; //back
	enemy_stats[0].animations[0][1] = { 4, {0, 103, 48, 42}, {46,12}, {24,36} }; //right
	enemy_stats[0].animations[0][2] = { 4, {0,55, 48, 48}, {46,18}, {24,36} }; //left
	enemy_stats[0].animations[0][3] = { 4, {0, 0, 48, 50}, {30,48}, {24,24} }; //front

	// Tiger

	enemy_stats[1].animations = new animation * [1];
	enemy_stats[1].scale_const = 0.85f;
	enemy_stats[1].base_movement_speed = 80;
	enemy_stats[1].states_no = 1;
	enemy_stats[1].base_animation_speed = 12;

	enemy_stats[1].item_drop_count = 2;
	enemy_stats[1].item_drops = new int[enemy_stats[1].item_drop_count];
	enemy_stats[1].item_drops[0] = 6;
	enemy_stats[1].item_drops[1] = 6;

	enemy_stats[1].textures_count = 1;
	enemy_stats[1].textures = new Texture * [enemy_stats[1].textures_count];
	enemy_stats[1].textures[0] = new Texture;

	enemy_stats[1].textures[0]->loadFromFile("textures/game/entities/enemies/tiger.png");

	enemy_stats[1].buffers_count = 9;
	enemy_stats[1].soundbuffers = new SoundBuffer * [enemy_stats[1].buffers_count];

	for (int i = 0; i < enemy_stats[1].buffers_count; i++)
		enemy_stats[1].soundbuffers[i] = &sound_buffers[33 + i];

	enemy_stats[1].animations[0] = new animation[4];
	enemy_stats[1].animations[0][0] = { 4, {0, 149, 48, 46}, {23,46}, {24,23} }; //back
	enemy_stats[1].animations[0][1] = { 4, {0, 106, 48, 42}, {46,16}, {25,34} }; //right
	enemy_stats[1].animations[0][2] = { 4, {0, 60, 48, 44}, {45,16}, {21,32} }; //left
	enemy_stats[1].animations[0][3] = { 4, {0, 0, 48 , 54}, {21,54}, {24,27} };//front

	//Cow
	passive_stats[0].animations = new animation * [1];
	passive_stats[0].scale_const = 0.85f;
	passive_stats[0].base_movement_speed = 80;
	passive_stats[0].states_no = 1;
	passive_stats[0].base_animation_speed = 12;

	passive_stats[0].item_drop_count = 3;
	passive_stats[0].item_drops = new int[passive_stats[0].item_drop_count];
	passive_stats[0].item_drops[0] = 2;
	passive_stats[0].item_drops[1] = 2;
	passive_stats[0].item_drops[2] = 6;

	passive_stats[0].textures_count = 1;
	passive_stats[0].textures = new Texture * [passive_stats[0].textures_count];
	passive_stats[0].textures[0] = new Texture;

	passive_stats[0].textures[0]->loadFromFile("textures/game/entities/passive/cow.png");

	passive_stats[0].buffers_count = 6;
	passive_stats[0].soundbuffers = new SoundBuffer * [passive_stats[0].buffers_count];

	for (int i = 0; i < passive_stats[0].buffers_count; i++)
		passive_stats[0].soundbuffers[i] = &sound_buffers[18 + i];

	passive_stats[0].animations[0] = new animation[4];
	passive_stats[0].animations[0][0] = { 4, {0,164,48, 56}, {29,51}, {24,27} }; //back
	passive_stats[0].animations[0][1] = { 4, {0,120,48,44}, {48,22}, {24,30} }; //right
	passive_stats[0].animations[0][2] = { 4, {0,65,48, 50}, {48,22}, {24,37} }; //left
	passive_stats[0].animations[0][3] = { 4, {0,0,48,65}, {30,52}, {24,37} }; //front
	//Sheep
	passive_stats[1].animations = new animation * [1];
	passive_stats[1].scale_const = 0.85f;
	passive_stats[1].base_movement_speed = 80;
	passive_stats[1].states_no = 1;
	passive_stats[1].base_animation_speed = 12;

	passive_stats[1].item_drop_count = 4;
	passive_stats[1].item_drops = new int[passive_stats[1].item_drop_count];
	passive_stats[1].item_drops[0] = 3;
	passive_stats[1].item_drops[1] = 3;
	passive_stats[1].item_drops[2] = 4;
	passive_stats[1].item_drops[3] = 4;

	passive_stats[1].textures_count = 1;
	passive_stats[1].textures = new Texture * [passive_stats[1].textures_count];
	passive_stats[1].textures[0] = new Texture;

	passive_stats[1].textures[0]->loadFromFile("textures/game/entities/passive/sheep.png");

	passive_stats[1].buffers_count = 6;
	passive_stats[1].soundbuffers = new SoundBuffer * [passive_stats[1].buffers_count];

	for (int i = 0; i < 3; i++) {
		passive_stats[1].soundbuffers[i] = &sound_buffers[15 + i];
		passive_stats[1].soundbuffers[i + 3] = &sound_buffers[15 + i];
	}

	passive_stats[1].animations[0] = new animation[5];
	passive_stats[1].animations[0][0] = { 4, {0,154,48,46}, {24,42}, {29,22} }; //back
	passive_stats[1].animations[0][1] = { 4, {1,112,48,41}, {38,20}, {29,29} }; //right
	passive_stats[1].animations[0][2] = { 4, {0,60,48,45}, {40,18}, {27,33} }; //left
	passive_stats[1].animations[0][3] = { 4, {0,0,48,55}, {24,40}, {29,33} }; //front
	//Lama
	passive_stats[2].animations = new animation * [1];
	passive_stats[2].scale_const = 0.7f;
	passive_stats[2].base_movement_speed = 80;
	passive_stats[2].states_no = 1;
	passive_stats[2].base_animation_speed = 16.6f;
	passive_stats[2].textures_count = 1;
	passive_stats[2].textures = new Texture * [passive_stats[2].textures_count];
	passive_stats[2].textures[0] = new Texture;

	passive_stats[2].textures[0]->loadFromFile("textures/game/entities/passive/llama.png");

	passive_stats[2].animations[0] = new animation[5];
	passive_stats[2].animations[0][0] = { 4, {0, 0 * 128, 128, 128}, {26,68}, {64,64} }; //back
	passive_stats[2].animations[0][1] = { 4, {0, 3 * 128, 128, 128}, {47,17}, {64,82} }; //right
	passive_stats[2].animations[0][2] = { 4, {0 , 1 * 128, 128, 128}, {47,17}, {64,82} }; //left
	passive_stats[2].animations[0][3] = { 4, {0, 2 * 128, 128, 128}, {26,62}, {64,62} }; //front

	NPC_stats[0].animations = new animation * [2];
	NPC_stats[0].scale_const = 0.65f;
	NPC_stats[0].base_movement_speed = 80;
	NPC_stats[0].states_no = 1;
	NPC_stats[0].base_animation_speed = 12;
	NPC_stats[0].textures_count = 2;
	NPC_stats[0].textures = new Texture * [NPC_stats[0].textures_count];
	NPC_stats[0].textures[0] = new Texture;
	NPC_stats[0].textures[1] = new Texture;

	NPC_stats[0].textures[0]->loadFromFile("textures/game/entities/NPCs/senku.png");
	NPC_stats[0].textures[1]->loadFromFile("textures/game/entities/NPCs/senku_petrified.png");

	for (int i = 0; i < 2; i++) {
		NPC_stats[0].animations[i] = new animation[4];
		NPC_stats[0].animations[i][0] = { 9, {0, 8 * 64, 64, 64}, {30,14}, {32,48} }; //back
		NPC_stats[0].animations[i][1] = { 9, {0, 11 * 64, 64, 64}, {30,14}, {32,48} }; //right
		NPC_stats[0].animations[i][2] = { 9, {0, 9 * 64, 64, 64}, {30,14}, {32,48} }; //left
		NPC_stats[0].animations[i][3] = { 9, {0, 10 * 64, 64, 64}, {30,14}, {32,48} }; //front
	}
	NPC_stats[1].animations = new animation * [2];
	NPC_stats[1].scale_const = 0.65f;
	NPC_stats[1].base_movement_speed = 80;
	NPC_stats[1].states_no = 1;
	NPC_stats[1].base_animation_speed = 12;
	NPC_stats[1].textures_count = 2;
	NPC_stats[1].textures = new Texture * [NPC_stats[1].textures_count];
	NPC_stats[1].textures[0] = new Texture;
	NPC_stats[1].textures[1] = new Texture;

	NPC_stats[1].textures[0]->loadFromFile("textures/game/entities/NPCs/kaseki.png");


		NPC_stats[1].animations[0] = new animation[4];
		NPC_stats[1].animations[0][0] = { 9, {0, 8 * 64, 64, 64}, {30,14}, {32,48} }; //back
		NPC_stats[1].animations[0][1] = { 9, {0, 11 * 64, 64, 64}, {30,14}, {32,48} }; //right
		NPC_stats[1].animations[0][2] = { 9, {0, 9 * 64, 64, 64}, {30,14}, {32,48} }; //left
		NPC_stats[1].animations[0][3] = { 9, {0, 10 * 64, 64, 64}, {30,14}, {32,48} }; //front

	NPC_stats[2].animations = new animation * [1];
	NPC_stats[2].scale_const = 0.65f;
	NPC_stats[2].base_movement_speed = 80;
	NPC_stats[2].states_no = 1;
	NPC_stats[2].base_animation_speed = 12;
	NPC_stats[2].textures_count = 1;
	NPC_stats[2].textures = new Texture * [NPC_stats[2].textures_count];
	NPC_stats[2].textures[0] = new Texture;

	NPC_stats[2].textures[0]->loadFromFile("textures/game/entities/NPCs/francois.png");

		NPC_stats[2].animations[0] = new animation[4];
		NPC_stats[2].animations[0][0] = { 9, {0, 8 * 64, 64, 64}, {30,14}, {32,48} }; //back
		NPC_stats[2].animations[0][1] = { 9, {0, 11 * 64, 64, 64}, {30,14}, {32,48} }; //right
		NPC_stats[2].animations[0][2] = { 9, {0, 9 * 64, 64, 64}, {30,14}, {32,48} }; //left
		NPC_stats[2].animations[0][3] = { 9, {0, 10 * 64, 64, 64}, {30,14}, {32,48} }; //front

		NPC_stats[3].animations = new animation * [2];
		NPC_stats[3].scale_const = 0.65f;
		NPC_stats[3].base_movement_speed = 80;
		NPC_stats[3].states_no = 1;
		NPC_stats[3].base_animation_speed = 12;
		NPC_stats[3].textures_count = 2;
		NPC_stats[3].textures = new Texture * [NPC_stats[0].textures_count];
		NPC_stats[3].textures[0] = new Texture;
		NPC_stats[3].textures[1] = new Texture;

		NPC_stats[3].textures[0]->loadFromFile("textures/game/entities/NPCs/Ryusui.png");
		NPC_stats[3].textures[1]->loadFromFile("textures/game/entities/NPCs/Ryusui_petrified.png");

		for (int i = 0; i < 2; i++) {
			NPC_stats[3].animations[i] = new animation[4];
			NPC_stats[3].animations[i][0] = { 9, {0, 8 * 64, 64, 64}, {30,14}, {32,48} }; //back
			NPC_stats[3].animations[i][1] = { 9, {0, 11 * 64, 64, 64}, {30,14}, {32,48} }; //right
			NPC_stats[3].animations[i][2] = { 9, {0, 9 * 64, 64, 64}, {30,14}, {32,48} }; //left
			NPC_stats[3].animations[i][3] = { 9, {0, 10 * 64, 64, 64}, {30,14}, {32,48} }; //front
		}

	item_stats.textures_count = 1;
	item_stats.textures = new Texture * [item_stats.textures_count];
	item_stats.textures[0] = new Texture;

	item_pickup_sound.setBuffer(sound_buffers[14]);
	poof_pop.setBuffer(sound_buffers[42]);

	item_stats.textures[0]->loadFromFile("textures/game/item drops.png");

	player_entity = new Player(player_stats, 1, static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, destroy_object_location);
	if (character_id == 3 && character_name == "Saitama") {
		player_entity->setDamage(SHRT_MAX/2);
		player_entity->destruction_power = SHRT_MAX/2;
		player_entity->set_movement_speed(269);
		player_stats.max_health = SHRT_MAX / 2;
	}


	player_entity->change_state(4);




	//////////set NPC trades /////////////////


	/////////////////////FOR TESTING //////////////////////////////////////


	//FOLLOW THE SAME TEMPLATE TO ADD TRADES (PRESS T to test it out)

	//initializing info:
	trading_test.dialogue = "I'm Francois, and I can cook the best food there is now .. and 3700 years ago";//<--- just write the dialogue in one string (splitting is automatic)
	trading_test.texture_name = "francois";        //<---- write the NPC name to get the pic
	trading_test.trades_count = 2;  //<---- number of trades
	trading_test.trading_menu = new TradingMenu[trading_test.trades_count]; //<-- just creates the array (it deletes itself, no need for worring about delete[])

	//initializing trades:
	trading_test.trading_menu[0].item_name = "Cooked Beef";     //<--- name of the trade (CURRENT NAMES AND INGREDIENTS ARE JUST RANDOM)
	trading_test.trading_menu[0].ingredients_count = 1;     //<--- number of ingredient for the trade [0] (UP TO 3 ONLY)

	trading_test.trading_menu[0].ingredients[0].ID = 2;     //<--- ingredient 1 ID [0]
	trading_test.trading_menu[0].ingredients[0].count = 1;     //<--- ingredient 1 count [0]

	trading_test.trading_menu[0].result.ID = 8;                //<--- the item you get from the trade's ID 
	trading_test.trading_menu[0].result.count = 1;             //<--- count of items you'll get

	//////////continue the rest of the trades the same way:

	trading_test.trading_menu[1].item_name = "Cooked Lamb";     //<--- name of the trade (CURRENT NAMES AND INGREDIENTS ARE JUST RANDOM)
	trading_test.trading_menu[1].ingredients_count = 1;     //<--- number of ingredient for the trade [0] (UP TO 3 ONLY)

	trading_test.trading_menu[1].ingredients[0].ID = 3;     //<--- ingredient 1 ID [0]
	trading_test.trading_menu[1].ingredients[0].count = 1;     //<--- ingredient 1 count [0]

	trading_test.trading_menu[1].result.ID = 9;                //<--- the item you get from the trade's ID 
	trading_test.trading_menu[1].result.count = 1;             //<--- count of items you'll get


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

	if (!dynamic_rendering.empty())
		dynamic_rendering.clear();
	dynamic_map.delete_all();


	items.clear();
	enemies.clear();
	passive.clear();
	NPCs.clear();

	destructable_count = 0;
	delete[] destructable_objects;
	destructable_objects = nullptr;
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

	/////// small plant //////////
	object_stats[3].health = 1;
	object_stats[3].idx = 3;
	object_stats[3].drops_no = 1;
	object_stats[3].item_drops[0] = 7;

	/////// big plant //////////
	object_stats[4].health = 3;
	object_stats[4].idx = 4;
	object_stats[4].drops_no = 3;
	object_stats[4].item_drops[0] = 7;
	object_stats[4].item_drops[1] = 7;
	object_stats[4].item_drops[2] = 7;
}

void GameState::initial_game(string current_map, Vector2f player_pos)
{
	this->current_map = current_map;

	base_minimap.loadFromFile("Maps/" + current_map + "_minimap.png");
	minimap_tex.loadFromFile("Maps/" + current_map + "_minimap.png");
	minimap_img.loadFromFile("Maps/" + current_map + "_minimap.png");

	minimap.setTexture(minimap_tex);
	minimap.setTextureRect(IntRect(0, 0, 96, 96));
	minimap.setOrigin(minimap.getLocalBounds().width / 2, minimap.getLocalBounds().height / 2);

	load_map(current_map);
	if (player_pos.x == -1)
		player_pos = respawn_point;
	center_cam(player_pos);

	if (current_map == "Doz World") {   //<-add indoor maps here
		DoDayLightCycle = false;
		DoEntitySpawning = false;
		EnableMiniMap = false;
	}
	else {
		DoDayLightCycle = true;
		DoEntitySpawning = true;
		EnableMiniMap = true;
	}

}

void GameState::cam_movement()
{
	Vector2f camera_movement = { ((player_entity->getRelativePos().x + map_x - win_x / (scale * z_scale) / 2.f) * 1.3f) * dt / (win_x / win_y),
								 ((player_entity->getRelativePos().y + map_y - win_y / (scale * z_scale) / 2.f) * 1.3f) * dt };
	if (size_x * 16 * (scale * z_scale) > win_x)
		move_cam(camera_movement.x, 0);
	if (size_y * 16 * (scale * z_scale) > win_y)
		move_cam(0, camera_movement.y);
}

void GameState::move_cam(float x_movement, float y_movement)
{
	if (map_x - x_movement <= 0 && map_x - x_movement >= -(size_x * 16 - win_x / (scale * z_scale))) {
		map_x -= x_movement;
		x_offset = -map_x / 16;
	}
	if (map_y - y_movement <= 0 && map_y - y_movement >= -(size_y * 16 - win_y / (scale * z_scale))) {
		map_y -= y_movement;
		y_offset = -map_y / 16;
	}
}

void GameState::center_cam(Vector2f player_pos)
{
	map_x = -(player_pos.x - win_x / 2 / (scale * z_scale));
	map_y = -(player_pos.y - win_y / 2 / (scale * z_scale));

	if (size_x * 16 * (scale * z_scale) > win_x) {
		if (-map_x < 0)
			map_x = 0;
		else if (-map_x > size_x * 16 - win_x / (scale * z_scale))
			map_x = -(size_x * 16 - win_x / (scale * z_scale));
	}
	else
		map_x = x/ (scale * z_scale) - size_x * 8;

	if (size_y * 16 * (scale * z_scale) > win_y) {
		if (-map_y < 0)
			map_y = 0;
		else if (-map_y > size_y * 16 - win_y / (scale * z_scale))
			map_y = -(size_y * 16 - win_y / (scale * z_scale));
	}
	else
		map_y = y / (scale * z_scale) - size_y * 8;

	x_offset = -map_x / 16, y_offset = -map_y / 16;

	player_entity->setPosition(player_pos.x, player_pos.y);
}

void GameState::maps_travel()
{
	if (!travel_map.empty()) {
		if (quest_idx != 1 && quest_idx != 2 && black_out()) {
			save();
			// maybe add time zone changes
			initial_game(travel_map, travel_location);
			travel_map.clear();
			travel_location = { -1, -1 };
		}
	}
	else {
		if(quest_idx != 1 && quest_idx != 2)
			black_in();
	}
}

void GameState::destroyANDrestore_objects(Vector2i core_location, bool destroy, bool ToEternityAndByound)
{
	object_height = 1;
	destroyANDrestore(core_location, destroy, ToEternityAndByound);
	if (destroy && !ToEternityAndByound) {
		poof_pop.play();
		effects.add({ 0, 0, 256, 256 }, 22, { core_location.x * 16 + 8 , core_location.y * 16 + 8 }, "Poof", sqrtf(object_height / 6.f), Color(255, 255, 255, 255), 0, map_x, map_y);
	}
}

void GameState::destroyANDrestore(Vector2i core_location, bool destroy, bool ToEternityAndByound, Vector2i check_tile)
{
	if (check_tile.x == -10)
		check_tile = core_location;
	if (destroy) {
		if (static_map[check_tile.x][check_tile.y].tile_props & 16) {
			for (int j = 0; j < dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].curr_idx; j++) {
				Uint8 pixels[16];
				for (int k = 0; k < 2; k++)
					for (int l = 0; l < 2; l++) {
						pixels[(k + 2 * l) * 4] = base_minimap.getPixel(int(dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].at[j].position.x * 2 + k), int(dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].at[j].position.y * 2 + l)).r; // red
						pixels[(k + 2 * l) * 4 + 1] = base_minimap.getPixel(int(dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].at[j].position.x * 2 + k), int(dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].at[j].position.y * 2 + l)).g; // green
						pixels[(k + 2 * l) * 4 + 2] = base_minimap.getPixel(int(dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].at[j].position.x * 2 + k), int(dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].at[j].position.y * 2 + l)).b; // blue
						pixels[(k + 2 * l) * 4 + 3] = base_minimap.getPixel(int(dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].at[j].position.x * 2 + k), int(dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].at[j].position.y * 2 + l)).a; // alpha
						minimap_img.setPixel(int(dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].at[j].position.x * 2 + k), int(dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].at[j].position.y * 2 + l), Color(pixels[(k + 2 * l) * 4], pixels[(k + 2 * l) * 4 + 1], pixels[(k + 2 * l) * 4 + 2], pixels[(k + 2 * l) * 4 + 3]));
					}
				minimap_tex.update(pixels, 2, 2, int(dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].at[j].position.x * 2), int(dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].at[j].position.y * 2));
			}
			dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].core_location = core_location;
			static_map[core_location.x][core_location.y].destruction_time = game_time;
			dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].destruction_time = &static_map[core_location.x][core_location.y].destruction_time;
			dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].time = &static_map[core_location.x][core_location.y].time;
			dynamic_update_minimap = 2;
			object_height += dynamic_map.at[static_map[check_tile.x][check_tile.y].dynamic_idx].height - 1;
		}
		else if (static_map[check_tile.x][check_tile.y].tile_props & 1 || static_map[check_tile.x][check_tile.y].tile_props & 32)
			bigbang(check_tile, 1, ToEternityAndByound);
	}
	else {
		if (static_map[check_tile.x][check_tile.y].destruction_time)
			bigbang(check_tile, 0, ToEternityAndByound);
	}
	for (int i = 0; i < 4; i++)
		if (destroy && static_map[check_tile.x + dx[i]][check_tile.y + dy[i]].tile_props & 1) {
			if (!static_map[check_tile.x + dx[i]][check_tile.y + dy[i]].destruction_time) {
				destroyANDrestore(core_location, destroy, ToEternityAndByound, { check_tile.x + dx[i], check_tile.y + dy[i] });
				object_height += (!dx[i]) * abs(dy[i]);
			}
		}
		else if(!destroy && static_map[check_tile.x + dx[i]][check_tile.y + dy[i]].destruction_time)
			destroyANDrestore(core_location, destroy, ToEternityAndByound, { check_tile.x + dx[i], check_tile.y + dy[i] });
}

void GameState::bigbang(Vector2i target_tile, bool destroy, bool ToEternityAndByound)
{
	short last = static_map[target_tile.x][target_tile.y].size - 1;
	Vector3i last_tile = static_map[target_tile.x][target_tile.y].layers[last];
	static_map[target_tile.x][target_tile.y].tile_props ^= tile_props[last_tile.z].properties[last_tile.x][last_tile.y].props;
	if (destroy) {
		static_map[target_tile.x][target_tile.y].destruction_time = ToEternityAndByound ? DBL_MAX : game_time;

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
	tile.setScale((scale * z_scale), (scale * z_scale));
	for (int i = (x_offset > 0) ? (int)x_offset : 0; i < (win_x + ((x_offset + 1) * 16 * (scale * z_scale))) / (16 * (scale * z_scale)) && i < size_x; i++)
		for (int j = (y_offset > 0) ? (int)y_offset : 0; j < (win_y + ((y_offset + 1) * 16 * (scale * z_scale))) / (16 * (scale * z_scale)) && j < size_y; j++) {
			auto tile_end = static_map[i][j].layers + static_map[i][j].size - 1;

			for (auto map_tile = static_map[i][j].layers; map_tile != tile_end; map_tile++) {
				tile.setTexture(*tile_sheets[map_tile->z]);
				tile.setTextureRect(IntRect(map_tile->x * 16, map_tile->y * 16, 16, 16));
				tile.setPosition(round(map_x * (scale * z_scale)) + (16 * (scale * z_scale) * i), round(map_y * (scale * z_scale)) + (16 * (scale * z_scale) * j));
				window->draw(tile, &shader);
			}
			if (!static_map[i][j].destruction_time) {
				if (dynamic_update_minimap)
					update_minimap_tile(Vector2i(i * 2, j * 2), *tile_end);
				tile.setTexture(*tile_sheets[tile_end->z]);
				tile.setTextureRect(IntRect(tile_end->x * 16, tile_end->y * 16, 16, 16));
				tile.setPosition(round(map_x * (scale * z_scale)) + (16 * (scale * z_scale) * i), round(map_y * (scale * z_scale)) + (16 * (scale * z_scale) * j));
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
}

void GameState::render_entities()
{
	if(player_entity->health > 0)
		dynamic_rendering.insert({ player_entity->getRelativePos().y, {-1, player_entity} });
	for (int i = 0; i < enemies.curr_idx; i++) {
		if (enemies.entities[i] != nullptr &&
			enemies.entities[i]->pos.y >= -map_y - entity_render_distance &&
			enemies.entities[i]->pos.y <= -map_y + entity_render_distance + win_y / (scale * z_scale) &&
			enemies.entities[i]->pos.x >= -map_x - entity_render_distance &&
			enemies.entities[i]->pos.x <= -map_x + entity_render_distance + win_x / (scale * z_scale)) {
			if (enemies.entities[i]->despawn)
				continue;
			dynamic_rendering.insert({ enemies.entities[i]->getRelativePos().y, {-1, enemies.entities[i]} });
		}
	}

	for (int i = 0; i < items.curr_idx; i++) {
		if (items.entities[i] != nullptr &&
			items.entities[i]->pos.y >= -map_y - entity_render_distance &&
			items.entities[i]->pos.y <= -map_y + entity_render_distance + win_y / (scale * z_scale) &&
			items.entities[i]->pos.x >= -map_x - entity_render_distance &&
			items.entities[i]->pos.x <= -map_x + entity_render_distance + win_x / (scale * z_scale)) {
			if (items.entities[i]->despawn)
				continue;
			dynamic_rendering.insert({ items.entities[i]->getRelativePos().y, {-1, items.entities[i]} });
		}
	}

	for (int i = 0; i < passive.curr_idx; i++) {
		if (passive.entities[i] != nullptr &&
			passive.entities[i]->pos.y >= -map_y - entity_render_distance &&
			passive.entities[i]->pos.y <= -map_y + entity_render_distance + win_y / (scale * z_scale) &&
			passive.entities[i]->pos.x >= -map_x - entity_render_distance &&
			passive.entities[i]->pos.x <= -map_x + entity_render_distance + win_x / (scale * z_scale)) {
			if (passive.entities[i]->despawn)
				continue;
			dynamic_rendering.insert({ passive.entities[i]->pos.y, {-1, passive.entities[i]} });
		}
	}

	for (int i = 0; i < NPCs.curr_idx; i++) {
		if (NPCs.entities[i] != nullptr &&
			NPCs.entities[i]->pos.y >= -map_y - entity_render_distance &&
			NPCs.entities[i]->pos.y <= -map_y + entity_render_distance + win_y / (scale * z_scale) &&
			NPCs.entities[i]->pos.x >= -map_x - entity_render_distance &&
			NPCs.entities[i]->pos.x <= -map_x + entity_render_distance + win_x / (scale * z_scale)) {
			if (NPCs.entities[i]->despawn)
				continue;
			dynamic_rendering.insert({ NPCs.entities[i]->pos.y, {-1, NPCs.entities[i]} });
		}
	}

	//render effects
	for (int i = 0; i < effects.curr_idx; i++) {
		if (effects.animations[i]->pos.y >= -map_y - entity_render_distance &&
			effects.animations[i]->pos.y <= -map_y + entity_render_distance + win_y / (scale * z_scale) &&
			effects.animations[i]->pos.x >= -map_x - entity_render_distance &&
			effects.animations[i]->pos.x <= -map_x + entity_render_distance + win_x / (scale * z_scale)) {
			if (effects.animations[i]->despawn)
				continue;
			dynamic_rendering.insert({ (float)effects.animations[i]->pos.y, {-1, nullptr, effects.animations[i]} });
		}
	}

	//int debug_ctr = 0;
	//cout << "total entities/objects: " << dynamic_rendering.size() << '\n';
	for (auto i = dynamic_rendering.lower_bound(-map_y - min(entity_render_distance, object_render_distance) - 32.f);
		i != dynamic_rendering.end() && i->first <= -map_y + max(short(obj_up_offset + object_render_distance), entity_render_distance) + 32.f + win_y / (scale * z_scale); ) {
		
		if (i->second.tile != -1) {
			if (dynamic_map.at[i->second.tile].at[0].position.x*16 < -map_x - obj_right_offest - object_render_distance ||
				dynamic_map.at[i->second.tile].at[0].position.x*16 > -map_x + obj_left_offset + object_render_distance + win_x / (scale * z_scale) ||
				i->first < -map_y - object_render_distance - obj_down_offset || i->first > -map_y + object_render_distance + obj_up_offset + win_y/(scale * z_scale)) {
				i++;
				continue;
			}
			//debug_ctr++;
			if(!dynamic_map.at[i->second.tile].destruction_time){
				for (int j = 0; j < dynamic_map.at[i->second.tile].curr_idx; j++) {
					tile.setTexture(*tile_sheets[dynamic_map.at[i->second.tile].at[j].tile.z]);
					tile.setTextureRect(IntRect(dynamic_map.at[i->second.tile].at[j].tile.x * 16, dynamic_map.at[i->second.tile].at[j].tile.y * 16, 16, 16));
					tile.setPosition(round(map_x * (scale * z_scale)) + (16 * (scale * z_scale) * dynamic_map.at[i->second.tile].at[j].position.x), round(map_y * (scale * z_scale)) + (16 * (scale * z_scale) * dynamic_map.at[i->second.tile].at[j].position.y));
					window->draw(tile, &shader);
					if (dynamic_update_minimap)
						update_minimap_tile(Vector2i((int)dynamic_map.at[i->second.tile].at[j].position.x * 2, (int)dynamic_map.at[i->second.tile].at[j].position.y * 2), dynamic_map.at[i->second.tile].at[j].tile);
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
				if(dynamic_map.at[i->second.tile].core_location.x > -1)
					*dynamic_map.at[i->second.tile].time = game_time;
			}

			i++;
		}
		else if(i->second.entity != nullptr){
			if (i->second.entity->despawn) {
				i = dynamic_rendering.erase(i);
				continue;
			}
			i->second.entity->render(&shader);
			i->second.entity = nullptr;
			i = dynamic_rendering.erase(i);
		}
		else {
			if (i->second.effect != nullptr) {
				if (i->second.effect->despawn) {
					i = dynamic_rendering.erase(i);
					continue;
				}
				i->second.effect->render(&shader);
				i->second.effect = nullptr;
			}
			i = dynamic_rendering.erase(i);
		}
	}

	if (dynamic_update_minimap) {
		dynamic_update_minimap--;
		if (!dynamic_update_minimap)
			minimap_tex.loadFromImage(minimap_img);
	}

	//cout << "total rendering: " << debug_ctr << '\n';
}

void GameState::update_minimap()
{
	int minimap_player_pos_x = (int)player_entity->getRelativePos().x / 8 , minimap_player_pos_y = (int)player_entity->getRelativePos().y / 8;
	int minimap_x = minimap_player_pos_x - 48, minimap_y = minimap_player_pos_y - 48;
	minimap_x = (minimap_x < 0) ? 0 : (minimap_x > size_x * 2 - 98) ? size_x * 2 - 98 : minimap_x;
	minimap_y = (minimap_y < 0) ? 0 : (minimap_y > size_y * 2 - 98) ? size_y * 2 - 98 : minimap_y;
	minimap.setTextureRect(IntRect(minimap_x, minimap_y, 96, 96));
	player_pointer.setPosition(win_x - 52 * scale + (minimap_player_pos_x - minimap_x - 48) * 0.8f * scale, 52 * scale + (minimap_player_pos_y - minimap_y - 48) * 0.8f * scale);

	if (waypoint_position.x > -1) {
		int waypoint_minimap_pos_x = clamp<int>((int)waypoint_position.x - minimap_x - 48, -48, 48);
		int waypoint_minimap_pos_y = clamp<int>((int)waypoint_position.y - minimap_y - 48, -48, 48);
		waypoint_pointer.setPosition(win_x - 52 * scale + waypoint_minimap_pos_x * 0.8f * scale, 52 * scale + waypoint_minimap_pos_y * 0.8f * scale);
	}

	if (quest_location.x > -1) {
		int quest_minimap_pos_x = clamp<int>(int(quest_location.x / 8 - minimap_x - 48), -48, 48);
		int quest_minimap_pos_y = clamp<int>(int(quest_location.y / 8 - minimap_y - 48), -48, 48);
		quest_pointer.setPosition(win_x - 52 * scale + quest_minimap_pos_x * 0.8f * scale, 52 * scale + quest_minimap_pos_y * 0.8f * scale);
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
		screen_length = short(win_x / (16 * (scale * z_scale))), screen_height = short(win_y / (16 * (scale * z_scale)));
		short spawn_rect_x = screen_length + spawn_dist, spawn_rect_y = screen_height + spawn_dist;
		spawn_total = generate_random(0, float(spawn_rect_x * 2 + spawn_rect_y * 2 - 1));
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

		spawn_x += short(x_offset - spawn_dist / 2.f), spawn_y += short(y_offset - spawn_dist / 2.f);

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
			if (light_level <= 0.4) {
				enemies.add(enemy_spawn(generate_random(0, float(number_of_enemies - 1))), { 16.f * spawn_x, 16.f * spawn_y });
				enemies.entities[enemies.curr_idx - 1]->update((scale * z_scale));
			}
			else {
				passive.add(passive_spawn(generate_random(0, float(number_of_passives - 1))), { 16.f * spawn_x, 16.f * spawn_y });
				passive.entities[passive.curr_idx - 1]->update((scale * z_scale));

			}
		}
	}
}

bool GameState::entity_in_range(Vector2f cords, short offset)
{
	return(cords.y >= -map_y - entity_render_distance &&
		cords.y <= -map_y + entity_render_distance + win_y / (scale * z_scale) &&
		cords.x >= -map_x - entity_render_distance &&
		cords.x <= -map_x + entity_render_distance + win_x / (scale * z_scale));
}

void GameState::block_interactions_list(Vector2i interaction_tile)
{
	//cout << interaction_tile.x << ' ' << interaction_tile.y << '\n';
	if (current_map == "Japan") {
		if ((interaction_tile.x == 14 || interaction_tile.x == 15) && (interaction_tile.y == 13)) {
			travel_map = "Doz World";
		}
		else if (interaction_tile.x == 26 && interaction_tile.y == 19) {
			states->insert({ 9, new WorldMapState(travel_map) });
			states->at(WorldMapID)->update();
		}
	}
	else if (current_map == "Doz World") {
		if (interaction_tile.x >= 22 && interaction_tile.x <= 26 && interaction_tile.y == 29) {
			travel_map = "Japan";
			travel_location = { 250.f, 236.f };
		}
	}
}

void GameState::DayLightCycle()
{
	int count = 0;
	const int viewDist = 13;

	shader.setUniform("scale", scale * z_scale);
	for (auto i = light_sources.lower_bound(-map_y - (viewDist * 16 * (scale * z_scale))); i != light_sources.end() && i->first <= -map_y + win_y / (scale * z_scale) + (viewDist * 16 * (scale * z_scale)); i++) {
		if (i->second.position.x > -map_x - (viewDist * 16 * (scale * z_scale)) && i->second.position.x < -map_x + win_x / (scale * z_scale) + (viewDist * 16 * (scale * z_scale))) {
			if(i->second.day_light)
				shader.setUniform("lights[" + to_string(count) + "].color", Vector3f(light_level, light_level, light_level));
			else
				shader.setUniform("lights[" + to_string(count) + "].color", i->second.color);
			shader.setUniform("lights[" + to_string(count) + "].position", (i->second.position + Vector2f(map_x, map_y)) * (scale * z_scale));
			shader.setUniform("lights[" + to_string(count) + "].intensity", i->second.intensity * (scale * z_scale));
			count++;
		}
	}
	if (player_entity->state == 0) {
		
		if (torch_intensity > 0.7f)
			torch_delta -= 0.2f;
		else if (torch_intensity < 0.6f)
			torch_delta = 0.2f;

		torch_intensity += torch_delta * dt;

		shader.setUniform("lights[" + to_string(count) + "].position", (player_entity->getRelativePos() + Vector2f(map_x, map_y)) * (scale * z_scale));
		shader.setUniform("lights[" + to_string(count) + "].color", Vector3f(0.95f, 0.92f, 0.7f));
		shader.setUniform("lights[" + to_string(count) + "].intensity", torch_intensity * (scale * z_scale));
		count++;
	}

	light_level += day_increment * dt;
	if (light_level < 0.1f) {
		light_level = 0.1f;
		day_increment = 0.01f;
	}
	else if (light_level > 1.f) {
		light_level = 1.f;
		day_increment = -0.01f;
	}
	if(DoDayLightCycle)
		shader.setUniform("ambient_light", Glsl::Vec4(light_level, light_level, (light_level + 0.2f < 1.f ? light_level + 0.2f : 1.f), 1.f));
	else
		shader.setUniform("ambient_light", Glsl::Vec4(constant_light_level, constant_light_level, constant_light_level + 0.1f, 1.f));
	shader.setUniform("lightsCount", count);
}

void GameState::initial_entities()
{
	if (current_map == "maze") {
		enemies.add(enemy_spawn(1), { 488.f, 1624.f }, 1);
		enemies.entities[enemies.curr_idx - 1]->setScale(0.35f);
	}

	if (current_map == "Japan") {
		NPCs.add(spawn_npc(1), { 648, 776 }, npc_details(1, 10, 1));
		npc_initalize;
		NPCs.add(spawn_npc(2), { 968, 680 }, npc_details(1, 10, 4));
		npc_initalize;
		NPCs.add(spawn_npc(3), { 904, 872 }, npc_details(1, 10, 2));
		npc_initalize;
	}
	else if (current_map == "Doz World") {
		NPCs.add(spawn_npc(0), { 128, 112}, npc_details(1, 10, 0));
		npc_initalize;
	}
}

void GameState::quests()
{
	switch (quest_idx) {
		case -1:
		case 0:
			if (quest_dialogue != nullptr)
				delete[] quest_dialogue;
			quest_dialogue = new dialogue[10];
			quest_dialogue[0] = { character_name, "Since that day I don't know what happened, That beam appeared out of no where...", 0, short(1 + character_id)};
			quest_dialogue[1] = { character_name, "Since then I feel like I have been sleeping and fully emersed in my thoughts for years.", 0, short(1 + character_id) };
			quest_dialogue[2] = { character_name, "I want to wake up now, I want to check on my family and friends.", 0, short(1 + character_id) };
			quest_dialogue[3] = { character_name, "Are they alright?...", 0, short(1 + character_id) };
			quest_dialogue[4] = { character_name, "I MUST..", 0, short(1 + character_id) };
			quest_dialogue[5] = { character_name, "WAKE UP..", 0, short(1 + character_id) };
			quest_dialogue[6] = { character_name, "NOW..", 0, short(1 + character_id) };
			quest_dialogue[7] = { character_name, "WAKE UP..", 0, short(1 + character_id) };
			quest_dialogue[8] = { character_name, "MY BODY..", 0, short(1 + character_id) };
			quest_dialogue[9] = { character_name, "RESPOND..", 0, short(1 + character_id) };
			quest_dialogue_num = 10;

			states->insert({ DialogueID,new DialogueState(quest_dialogue,{0, 140}, 1, quest_dialogue_num) });
			states->at(DialogueID)->update();
			quest_idx++;
			blackining = 255, blink_delay = 1.f;
			break;

		case 1:
			player_entity->stun = 1000;
			blink_delay += dt;
			if (blink_delay >= 1.f) {
				float blink_val = (blink_delay < 2.25f) ? 12.f * (1+blink_count) * blink_delay + 25 * blink_count: 250 - (blink_count >= 2) * 200;
				black_in(blink_val);
				if (!blackining) quest_idx+= (blink_count < 2) ? 1 : 2;
			}
			break;
		case 2:
			//cout << blackining <<'\n';
			player_entity->stun = 1000;
			if (black_out(500.f - blink_count * 125.f)) {
				quest_idx--, blink_count++, blink_delay = 0;
			}
			break;
		case 3:
			if (quest_dialogue != nullptr)
				delete[] quest_dialogue;
			quest_dialogue = new dialogue[6];
			quest_dialogue[0] = {"Senku" ,"Hey, Are you okay?!...", 0, 1};
			quest_dialogue[1] = {"Senku", "Don't panic I will explain everything now...", 0, 1 };
			quest_dialogue[2] = {"Senku", "3700 years ago earth was hit by a huge disaster, a green beam covered all of earth and resulted in all of humanity getting petrified.", 0, 1 };
			quest_dialogue[3] = {"Senku", "Don't mind the details right now about how I got up from petrification. You'll know everything eventually, now let's just get you started.", 0, 1 };
			quest_dialogue[4] = {"Senku", "First things first, I have provided you with stone tools to make collecting items doable...", 0, 1 };
			quest_dialogue[5] = {"Senku", "I want you to gather some wood (12 pieces) and stone (8 pieces) and then go to Kaseki and hand him the materials and he'll get the rest done", 0, 1 };
			quest_dialogue_num = 6;

			states->insert({ DialogueID,new DialogueState(quest_dialogue,{0, 140}, 1, quest_dialogue_num) });
			states->at(DialogueID)->update();
			player_entity->stun = 0;
			quest_idx++;
			break;
		case 4:
			if (inventory_count[0] >= 12 && inventory_count[1] >= 8) {
				quest_location = NPCs.entities[npc_id_to_idx[1]]->pos;

			}
			else {
				quest_location.x = -1.f;
			}
			break;
		case 5:
			destroyANDrestore_objects({ 40, 46 }, 0);
			quest_idx++;
		default:
			quest_location.x = -1.f;
	}
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
			old_alpha = int((float)pixels[(k + 2 * l) * 4 + 3] / 255.f), tile_alpha = int((float)tile_color.a / 255.f);
			sum_alpha = int(1.f - (1.f - tile_alpha) * (1.f - old_alpha));
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
}

void GameState::check_in_inventory(int id)
{
	if (!inventory_count[id]) {
		inventory_order.erase(id);
	}
}

GameState::GameState(int save_num)
	: items(50)
{
	this->save_num = save_num;

	win_x = (float)window->getSize().x, win_y = (float)window->getSize().y;

	if (win_x / 540.f < win_y / 304.5f) scale = win_x / 540.f;
	else scale = win_y / 304.5f;

	if (!shader.loadFromFile("VertexShader.shader", "FragmentShader.shader")) {
		cout << "Couldn't load shaders\n";
	}

	initial_tile_sheets("game/tiles");
	initial_textures("game");
	initial_sounds();


	load_game();
	set_textures();

}

GameState::~GameState()
{
	deload_map();

	if (destructable_objects != nullptr)
		delete destructable_objects;

	if (player_entity != nullptr)
		delete player_entity;

	if (sound_buffers != nullptr)
		delete[] sound_buffers;

	if (quest_dialogue != nullptr)
		delete[] quest_dialogue;
}

void GameState::update()
{
	active_cursor = false;
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = (float)window->getSize().x, win_y = (float)window->getSize().y;
		x = win_x / 2.f, y = win_y / 2.f;
		if (win_x / 540.f < win_y / 304.5f) scale = win_x / 540.f;
		else scale = win_y / 304.5f;
		/////////////////////
		center_cam(player_entity->getRelativePos());
		hotbar.setScale(scale * 0.1f, scale * 0.1f);
		hotbar.setPosition(win_x / 2.f, win_y - 20.f * scale);
		hotbar_selection.setScale(scale * 0.1f, scale * 0.1f);
		health_indicator.setPosition(20.f * scale , 20.f * scale);
		health_indicator.setScale(scale * 0.15f, scale * 0.15f);
		blackscreen.setSize({ win_x, win_y });
		for (int i = 0; i < 4; i++) {
			tool_icons[i].setPosition(win_x / 2.f + 3.f * scale, win_y - 20.f * scale);
			tool_icons[i].setScale(scale * 0.1f, scale * 0.1f);
		}
		hotbar_selection.setPosition(win_x / 2.f - (hotbar.getLocalBounds().width / 2.f - 12.f) * scale * 0.1f + (4.f - player_entity->state) * 248.f * scale * 0.1f, win_y - 20.f * scale);
		minimap_frame.setPosition(win_x - 52.f * scale, 52.f * scale);
		minimap_frame.setScale(scale * 0.96f, scale * 0.96f);
		minimap.setPosition(win_x - 52.f * scale, 52.f * scale);
		minimap.setScale(scale * 0.8f, scale * 0.8f);

		player_pointer.setRadius(1.5f * scale);
		player_pointer.setOrigin(0.75f * scale, 0.75f * scale);

		waypoint_pointer.setRadius(1.5f * scale);
		waypoint_pointer.setOrigin(0.75f * scale, 0.75f * scale);

		quest_pointer.setRadius(1.5f * scale);
		quest_pointer.setOrigin(0.75f * scale, 0.75f * scale);

		shader.setUniform("ratio", win_x / win_y);
	}

	if (z_scale > 0.6)
		z_scale -= 0.4f * dt;

	if (fps_active)
		fps_text.setString(fps_text.getString() + "\tCoordinates " + to_string(int(player_entity->getRelativePos().x / 16)) + ' ' + to_string(int(player_entity->getRelativePos().y / 16)));

	cam_movement();

	//entity spawning
	spawn_cd += dt;
	if (spawn_cd >= def_spawn_cd) {
		spawn_cd = 0;
		if (!generate_random(0, 3))
			entity_spawning();
	}

	player_entity->movement = delta_movement();

	maps_travel();

	if (destroy_object_location.x >= 0) {
		destroyANDrestore_objects(destroy_object_location, 1);
		destroy_object_location = { -1, -1};
	}

	if (EnableMiniMap)
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
				enemies.entities[i]->update((scale * z_scale));
			if(enemies.entities[i]->got_hit)
				effects.add({ 400,0,100,100 }, 20, { int(enemies.entities[i]->getRelativePos().x) , int(enemies.entities[i]->getRelativePos().y + 2) }, "break_animation", 0.9f, Color(136, 8, 8, 240), 0, map_x, map_y);
		}
		else {
			if (enemies.entities[i]->health <= 0) {
				poof_pop.play();
				effects.add({ 0, 0, 256, 256 }, 22, { (int)enemies.entities[i]->getRelativePos().x, (int)enemies.entities[i]->getRelativePos().y + 2}, "Poof", 0.5f, Color(255, 255, 255, 255), 0, map_x, map_y);
				for (int j = 0; j < enemies.entities[i]->entity_stats.item_drop_count; j++)
					items.add(item_spawn(enemies.entities[i]->entity_stats.item_drops[j]), enemies.entities[i]->getRelativePos(), 0, 300.f);
			}
			enemies.rem_ove(i);
		}
	}

	for (int i = 0; i < passive.curr_idx; i++) {
		if (!passive.entities[i]->despawn) {
			if (passive.entities[i]->action_state != 0 || entity_in_range(passive.entities[i]->pos, entity_update_distance))
				passive.entities[i]->update((scale * z_scale));
			if (passive.entities[i]->got_hit)
				effects.add({ 400,0,100,100 }, 20, { int(passive.entities[i]->getRelativePos().x) , int(passive.entities[i]->getRelativePos().y + 2) }, "break_animation", 0.9f, Color(136, 8, 8, 240), 0, map_x, map_y);
		}
		else {
			if (passive.entities[i]->health <= 0) {
				effects.add({ 0, 0, 256, 256 }, 22, { (int)passive.entities[i]->getRelativePos().x, (int)passive.entities[i]->getRelativePos().y + 2 }, "Poof", 0.5f, Color(255, 255, 255, 255), 0, map_x, map_y);
				poof_pop.play();
				for (int j = 0; j < passive.entities[i]->entity_stats.item_drop_count; j++)
					items.add(item_spawn(passive.entities[i]->entity_stats.item_drops[j]), passive.entities[i]->getRelativePos(), 0, 300.f);
			}
				passive.rem_ove(i);
		}
	}

	for (int i = 0; i < NPCs.curr_idx; i++) {
		if (!NPCs.entities[i]->despawn) {
			if (NPCs.entities[i]->npc_type == 3 && game_time - NPCs.entities[i]->despawn_timer > NPCs.entities[i]->time_to_despawn / 3.f)
				NPCs.entities[i]->update((scale * z_scale));
			npc_id_to_idx[NPCs.entities[i]->id] = i;
			if (NPCs.entities[i]->action_state != 0 || entity_in_range(NPCs.entities[i]->pos, entity_update_distance))
				NPCs.entities[i]->update((scale * z_scale));
		}
		if(NPCs.entities[i]->despawn) {
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
		player_entity->update((scale * z_scale));
	}
	else {
		if (!no_update) {
			health_indicator.setTextureRect(IntRect(0, 0, 590, 100));
			//CHANGE MESSAGE TO RESPAWN
			states->insert({ DialogueID,new DialogueState(death_message,{0, 140}, 1, 2) });
			states->at(DialogueID)->update();
			no_update = 1;
		}
		else {
			no_update = 0;
			player_entity->health = player_stats.max_health;
			player_entity->despawn = false;
			player_entity->stun = 0;
			player_entity->change_state(3);
			hotbar_selection.setPosition(win_x / 2.f - (hotbar.getLocalBounds().width / 2.f - 12) * scale * 0.1f + (3 - player_entity->state) * 248 * scale * 0.1f, win_y - 20 * scale);
			tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(130, 130, 130));
			if (player_entity->state != 3)
				tool_icons[(2 - player_entity->state)].setColor(Color(255, 255, 255));
			player_entity->direction({ 0, 1 });


			while (effects.curr_idx)
				effects.remove(effects.curr_idx - 1);

			nod* it = inventory_order.first;

			while (it != NULL) {
				items.add(item_spawn(it->itm), player_entity->getRelativePos(), 0, 300.f);
				items.entities[items.curr_idx - 1]->health = inventory_count[it->itm];
				inventory_count[it->itm] = 0;
				it = it->link;
			}
			inventory_order.clear();

			center_cam(respawn_point);
		}
	}

	for (int i = 0; i < effects.curr_idx; i++) {
		if (effects.animations[i]->despawn) {
			effects.remove(i);
			i--;
		}
		else
			effects.animations[i]->update((scale * z_scale));
	}

	player_entity->interact = 0;

	for (int i = 0; i < items.curr_idx; i++) {

		if (items.entities[i]->despawn) {

			item_pickup_sound.setVolume(game_volume);
			item_pickup_sound.play();

            // add item to player_inventory
			if (!inventory_count[items.entities[i]->id])
				inventory_order.add(items.entities[i]->id);
			inventory_count[items.entities[i]->id]+= items.entities[i]->health;
			// despawn item
			item_pickup_sound.setPitch(generate_random_f(0.7f, 0.9f));
			items.rem_ove(i);
			i--;
			
		}
		else{
			if(entity_in_range(items.entities[i]->pos, entity_update_distance))
				items.entities[i]->update((scale * z_scale));
		}
		
	}
	game_time += dt;
	quests();
}

void GameState::render()
{
	render_static_map();
	render_entities();
	if (states->rbegin()->first == GameID || states->rbegin()->first == InventoryID || states->rbegin()->first == NotificationID || states->rbegin()->first == DialogueID) {
		if(states->rbegin()->first != DialogueID){
			window->draw(hotbar);
			for (int i = 0; i < 4; i++)
				window->draw(tool_icons[i]);
			window->draw(hotbar_selection);
		}
		health_indicator.setTextureRect(IntRect(0, (int)ceil(player_entity->health * 10 / player_stats.max_health) * 100, 590, 100));
		window->draw(health_indicator);
		if (EnableMiniMap)
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
				save(); 
				}
				break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				if (!shader.loadFromFile("VertexShader.shader", "FragmentShader.shader")) {
					cout << "Couldn't reload shaders\n";
				}
				shader.setUniform("ratio", win_x / win_y);
				break;
			case Keyboard::Num1:
				player_entity->change_state(4);
				hotbar_selection.setPosition(win_x / 2.f - (hotbar.getLocalBounds().width / 2.f - 12.f) * scale * 0.1f + (4.f - player_entity->state) * 248.f * scale * 0.1f, win_y - 20.f * scale);
				tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(130, 130, 130)), tool_icons[3].setColor(Color(130, 130, 130));
				break;
			case Keyboard::Num2:
				player_entity->change_state(3);
				hotbar_selection.setPosition(win_x / 2.f - (hotbar.getLocalBounds().width / 2.f - 12.f) * scale * 0.1f + (4.f - player_entity->state) * 248.f * scale * 0.1f, win_y - 20.f * scale);
				tool_icons[0].setColor(Color(255, 255, 255)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(130, 130, 130)), tool_icons[3].setColor(Color(130, 130, 130));
				break;
			case Keyboard::Num3:
				player_entity->change_state(2);
				hotbar_selection.setPosition(win_x / 2.f - (hotbar.getLocalBounds().width / 2.f - 12.f) * scale * 0.1f + (4.f - player_entity->state) * 248.f * scale * 0.1f, win_y - 20.f * scale);
				tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(255, 255, 255)), tool_icons[2].setColor(Color(130, 130, 130)), tool_icons[3].setColor(Color(130, 130, 130));
				break;
			case Keyboard::Num4:
				player_entity->change_state(1);
				hotbar_selection.setPosition(win_x / 2.f - (hotbar.getLocalBounds().width / 2.f - 12.f) * scale * 0.1f + (4.f - player_entity->state) * 248.f * scale * 0.1f, win_y - 20.f * scale);
				tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(255, 255, 255)), tool_icons[3].setColor(Color(130, 130, 130));
				break;
			case Keyboard::Num5:
				player_entity->change_state(0);
				hotbar_selection.setPosition(win_x / 2.f - (hotbar.getLocalBounds().width / 2.f - 12.f) * scale * 0.1f + (4.f - player_entity->state) * 248.f * scale * 0.1f, win_y - 20.f * scale);
				tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(130, 130, 130)), tool_icons[3].setColor(Color(255, 255, 255));
				break;
			case Keyboard::E:
				states->insert({ InventoryID, new InventoryState(&inventory_order, inventory_count, &player_entity->health, player_entity->entity_stats.max_health, &player_entity->combat_tag)});
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
					effects.add({ 0,0,100,100 }, 24, { player_entity->tool_used_on.x , player_entity->tool_used_on.y }, "break_animation", 0.6f, tile_color, 0, map_x, map_y);
					if (item_drops_count != -1) {
						Vector3i temp;
						for (int i = 0; i < item_drops_count; i++) {
							items.add(item_spawn(item_drops[i]), {(float)player_entity->tool_used_on.x , (float)player_entity->tool_used_on.y}, 0, 300.f);
						}
						item_drops_count = -1;
					}
					player_entity->tool_used_on.x = -1;
				}
				break;
			}
			break;
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case Mouse::Left:
				clicked_on = window->mapPixelToCoords(Mouse::getPosition(*window));
				break;
			}
			break;
		case Event::MouseWheelMoved:
			if (event.type == Event::MouseWheelMoved) {
				if (!player_entity->active_action) {
					int new_state = player_entity->state + event.mouseWheel.delta;
					if (new_state > 4) new_state = 4;
					else if (new_state < 0) new_state = 0;
					player_entity->change_state(new_state);
					hotbar_selection.setPosition(win_x / 2.f - (hotbar.getLocalBounds().width / 2.f - 12.f) * scale * 0.1f + (4.f - player_entity->state) * 248.f * scale * 0.1f, win_y - 20.f * scale);
					tool_icons[0].setColor(Color(130, 130, 130)), tool_icons[1].setColor(Color(130, 130, 130)), tool_icons[2].setColor(Color(130, 130, 130)), tool_icons[3].setColor(Color(130, 130, 130));
					if (new_state != 4)
						tool_icons[(3 - player_entity->state)].setColor(Color(255, 255, 255));
				}
			}
		}
	}
}