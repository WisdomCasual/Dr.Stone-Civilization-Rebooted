#pragma once
#include"State.h"
#include"Game.h"
#include"Entity.h"
#include"Enemy.h"
#include"Player.h"
#include"Items.h"
#include "Passive.h"
#include "NPC.h"
#include"Global.h"
#include"Animation.h"
#include"DialogueState.h"
#include "InventoryState.h"

#define lion(type) type, lion_stats, 1, static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player_entity
#define wolf(type) type, wolf_stats, 1, static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player_entity
//eneimies = 0, items = 1, passive = 2, NPC = 3

#define cow(type) type, cow_stats, 1, static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player_entity
#define deer(type) type, deer_stats, 1, static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player_entity
#define llama(type) type, llama_stats, 1, static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player_entity

#define npc_details(persistant, despawn_time, type) persistant, despawn_time, type, 0
#define default_npc 3, NPC_stats, 1, static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player_entity

using namespace globalvar;

struct GameState : public State
{
private:
	//variables:

	short no_update=0;
	dialogue death_message[2] = { {"Sneku: " ,"Well, this is unfortunate:\n/E1you're about to die", 0, 1}, {"Sneku: " ,"Heading back to the mainmenu\n/E2Better luck next time!", 1, 1} };

	Shader shader;
	entity player_stats, lion_stats, wolf_stats, cow_stats, deer_stats, llama_stats, item_stats, NPC_stats;
	Player* player_entity = nullptr;

	string character_name, current_map;
	int character_id, save_num;

	/////enemy spawning variables/////
	float spawn_cd = 0;
	short spawn_type = 0, left_bound, right_bound, up_bound, down_bound, screen_length, screen_height, spawn_x, spawn_y, spawn_total;         // if %2 = 1 then enemy, else then passive
	const float def_spawn_cd = 5.0;
	const short spawn_dist = 7, entity_render_distance = 5 * 16, object_render_distance = 3 * 16, entity_update_distance = 30 * 16,
				obj_up_offset = 7 * 16, obj_down_offset = 0, obj_left_offset = 2 * 16, obj_right_offest = 2 * 16;   //distance in pixels



	base_stats object_stats[30], * destructable_objects = nullptr;

	Vector2f clicked_on = { -1, -1 };

	render_tile** static_map;

	struct entities_container {
		int limit = 1, curr_idx = 0, find_size_x = 50, find_size_y = 50;
		short** vis = nullptr;
		bool astar_done = 0;
		Entity** entities = nullptr;
		entities_container(int limit = 20, Vector2f starting_position = {0, 0}) {
			entities = new Entity * [limit]({});
			this->limit = limit;
		}
		~entities_container() {
			if (entities != nullptr) {
				for (int i = 0; i < curr_idx; i++) {
					if (entities[i] != nullptr) {
						delete entities[i];
					}
				}
				delete[] entities;
			}
		}

		void add(short type, entity& entity_stats, bool has_legs, render_tile**& static_map, sheet_properties* tile_props_ptr, float& map_x, float& map_y, int& size_x, int& size_y, float& x_offset, float& y_offset, short& disable_dynamic_obj, Entity* player, Vector2f initial_position = { 800, 800 }, bool persistant = 0, double time_to_despawn = 10.0, int drop_id = 0, short npc_type = 0, short dialogue_num = 0, dialogue* dialogues = nullptr) {
			if (curr_idx < limit) {
				switch (type) {
					case 0:
						entities[curr_idx] = new Enemy(entity_stats, has_legs, static_map, tile_props_ptr, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player, persistant, time_to_despawn);
						entities[curr_idx]->setVisArray(&vis, &astar_done, find_size_x, find_size_y);
						entities[curr_idx]->setID(curr_idx + 1);
						break;
					case 1:
						//items
						entities[curr_idx] = new Items(entity_stats, has_legs, static_map, tile_props_ptr, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player, persistant, time_to_despawn, drop_id);
						break;
					case 2:
						entities[curr_idx] = new Passive(entity_stats, has_legs, static_map, tile_props_ptr, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player, persistant, time_to_despawn);
						break;
					case 3:
						entities[curr_idx] = new NPC(entity_stats, has_legs, static_map, tile_props_ptr, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player, persistant, time_to_despawn);
						entities[curr_idx]->set_type(npc_type);
						if(npc_type == 0)
							entities[curr_idx]->set_dialogue(dialogues, dialogue_num);
						break;
				}
				entities[curr_idx]->setPosition(initial_position.x, initial_position.y);
				entities[curr_idx]->update();
				curr_idx++;
			}
		} //types: 0 = enemy, 1 = item, 2 = passive, 3 = NPC

		void rem_ove(int idx) {
			delete entities[idx];
			entities[idx] = nullptr;
			curr_idx--;
			entities[idx] = entities[curr_idx]; 
			entities[curr_idx] = nullptr;
		}

	} enemies, items, passive, NPCs;

	struct animations_container {
		int limit = 1, curr_idx = 0, find_size_x = 50, find_size_y = 50;
		short** vis = nullptr;
		bool astar_done = 0;
		Animation** animations = nullptr;
		animations_container(int limit = 20, Vector2f starting_position = { 0, 0 }) {
			animations = new Animation * [limit]({});
			this->limit = limit;
		}
		~animations_container() {
			if (animations != nullptr) {
				for (int i = 0; i < curr_idx; i++) {
					if (animations[i] != nullptr) {
						delete animations[i];
					}
				}
				delete[] animations;
			}
		}

		void add(IntRect frame, int frame_count, Vector2i position, string animation_name, float animation_scale, Color texture_color, bool loop, float& map_x, float& map_y) {
			if (curr_idx < limit) {
				animations[curr_idx] = new Animation(frame, frame_count, position, animation_name, animation_scale, texture_color, loop, map_x, map_y);
				curr_idx++;
			}
		} //types: 0 = enemy, 1 = item, 2 = passive

		void remove(int idx) {
			delete animations[idx];
			curr_idx--;
			animations[idx] = animations[curr_idx];
			animations[curr_idx] = nullptr;
		}

	}effects;

	struct entity {
		Vector2f pixel_cords = { 0,0 };
		Vector2i tile_cords = { 0,0 };
		IntRect hitbox_rect{ 0, 0, 0, 0 };
		short health = 100, tool = 0;
	};

	struct entity_object {
		Vector2f position = { 0, 0 };   //position on map
		Vector3i tile = { 0, 0, 0 };   //tile details (x-sheet, y-sheet, texture id)
	};

	struct dynamic_objects {

		int size = 1, curr_idx = 0, layer = 0;
		entity_object* at; //dynamic array of objects

		dynamic_objects() {
			at = new entity_object[1];
		}

		dynamic_objects(int size) {
			at = new entity_object[size];
			this->size = size;
		}

		~dynamic_objects() {
			delete[] at;
		}

		void resize(const int size) {
			entity_object* old_dynamic_array = at;
			at = new entity_object[size];
			int copy_size = min(curr_idx, size);
			for (int i = 0; i < copy_size; i++)
				at[i] = old_dynamic_array[i];
			this->size = size;
			delete[] old_dynamic_array;
		}

		void add(const entity_object new_object) {
			if (curr_idx >= size)
				resize(size*2);

			at[curr_idx] = new_object, curr_idx++;
		}

		int arr_size() {
			return curr_idx;
		}
	};

	struct dynamic_objects_array {
		int size = 1, curr_idx = 0;
		dynamic_objects* at;  //dynamic 2D array of objects

		dynamic_objects_array() {
			at = new dynamic_objects[1];
		}

		dynamic_objects_array(const int size) {
			at = new dynamic_objects[size];
			this->size = size;
		}

		~dynamic_objects_array() {
			delete[] at;
		}

		void resize(const int size) {
			dynamic_objects* old_dynamic_array = at;
			at = new dynamic_objects[size];
			int copy_size = min(curr_idx, size);
			for (int i = 0; i < copy_size; i++) {
				at[i].size = old_dynamic_array[i].size;
				at[i].layer = old_dynamic_array[i].layer;
				at[i].curr_idx = old_dynamic_array[i].curr_idx;
				int sub_copy_size = min(at[i].curr_idx, at[i].size);
				for (int j = 0; j < sub_copy_size; j++) {
					at[i].at[j] = old_dynamic_array[i].at[j];
				}
			}
			this->size = size;
			delete[] old_dynamic_array;
		}

		void add(dynamic_objects& new_objct) {
			if (curr_idx >= size) {
				resize(size * 2);
			}
			at[curr_idx].at = new entity_object[new_objct.arr_size()];
			at[curr_idx].size = new_objct.size;
			at[curr_idx].layer = new_objct.layer;
			at[curr_idx].curr_idx = new_objct.curr_idx;
			int copy_size = min(new_objct.curr_idx, new_objct.size);
			for (int i = 0; i < copy_size; i++)
				at[curr_idx].at[i] = new_objct.at[i];
			curr_idx++;
		}

		int arr_size() {
			return curr_idx;
		}
	}dynamic_map;

	struct pointr {
		short tile = -1; Entity* entity = nullptr;
	};

	multimap<float, pointr> dynamic_rendering;



	int size_x = 0, size_y = 0;  //<-- map size
	int x = 0, y = 0;    //<-- location of upper left corner of the map
	float x_offset = 0, y_offset = 0; //<-- offset from upper left corner of the screen to upper left corner of the map
	short disable_dynamic_obj = -1;
	float map_x = 0, map_y = 0;
	float scale = 1, x_scale = 1, y_scale = 1, win_x = 0, win_y = 0, heal_delay = 0;
	Vector2u prev_win = { 0, 0 };
	Texture minimap_tex;
	Sprite tile, hotbar, hotbar_selection, health_indicator, tool_icons[3], minimap, minimap_frame;
	CircleShape player_pointer;
	short item_drops[5], item_drops_count = -1;
	in_order inventory_order;
	unsigned short inventory_count[50]{};

	//private functions:
	void search_front(int, int, int, Vector3i***, bool***, int);
	void load_map(string);
	void load_entities(float);
	void deload_map();
	void initial_stats();
	void initial_game(string, Vector2f);
	void center_cam(Vector2f);
	void render_static_map();
	void render_entities();
	void check_in_inventory(int);
	void update_minimap();
	void render_minimap();
	void entity_spawning();
	bool entity_in_range(Vector2f, short offset = 0);


public:
	//constructors/destructors
	GameState(int, string, Vector2f, string, int, int, double curr_game_time = 0);
	~GameState();
	
	//public functions:
	void update();
	void render();
	void pollevent();
};
