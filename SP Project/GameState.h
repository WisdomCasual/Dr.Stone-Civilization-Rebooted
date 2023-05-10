#pragma once
#include"State.h"
#include"Game.h"
#include"Entity.h"
#include"Enemy.h"
#include"Player.h"
#include"Items.h"
#include "Passive.h"
#include"Global.h"
#include"Animation.h"
//#include"EntityStats.h"

#define default_enemy 0, enemy_stats, "character 1", static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, &player_entity
#define default_passive 2, passive_stats, "character 3", static_map, tile_props, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, &player_entity

using namespace globalvar;

struct GameState : public State
{
private:
	//variables:

	entity player_stats, enemy_stats, passive_stats, lion_stats, wolf_stats, cow_stats, deer_stats, llama_stats;
	Player player_entity;

	

	Vector2f clicked_on = { -1, -1 };

	render_tile** static_map;

	//base_stats* destructable_objects = nullptr;

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

		void add(short type, entity& entity_stats, string entity_name, render_tile**& static_map, sheet_properties* tile_props_ptr, float& map_x, float& map_y, int& size_x, int& size_y, float& x_offset, float& y_offset, short& disable_dynamic_obj, Entity* player, Vector2f initial_position = { 800, 800 }, Texture** tile_textures =nullptr, Vector3i tile_info={0,0,0}) {
			if (curr_idx < limit) {
				switch (type) {
					case 0:
						entities[curr_idx] = new Enemy(entity_stats, entity_name, static_map, tile_props_ptr, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player);
						entities[curr_idx]->setVisArray(&vis, &astar_done, find_size_x, find_size_y);
						entities[curr_idx]->setID(curr_idx + 1);
						break;
					case 1:
						//items
						entities[curr_idx] = new Items(entity_stats, entity_name, static_map, tile_props_ptr, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player);
						break;
					case 2:
						entities[curr_idx] = new Passive(entity_stats, entity_name, static_map, tile_props_ptr, map_x, map_y, size_x, size_y, x_offset, y_offset, disable_dynamic_obj, player);
						break;
				}
				entities[curr_idx]->setPosition(initial_position.x, initial_position.y);
				curr_idx++;
			}
		} //types: 0 = enemy, 1 = item, 2 = passive

		void remove(int idx) {
			delete entities[idx];
			curr_idx--;
			entities[idx] = entities[curr_idx];
			entities[curr_idx] = nullptr;
		}

	} enemies, items, passive;


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

		void add(IntRect frame, int frame_count, Vector2i position, string animation_name, Color texture_color, bool loop, float& map_x, float& map_y) {
			if (curr_idx < limit) {
				animations[curr_idx] = new Animation(frame, frame_count, position, animation_name, texture_color, loop, map_x, map_y);
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
	float scale = 1, x_scale = 1, y_scale = 1, win_x = 0, win_y = 0;
	Vector2u prev_win = { 0, 0 };
	Sprite tile;


	//private functions:
	void search_front(int, int, int, Vector3i***, bool***, int);
	void load_map(string);
	void load_entities(float);
	void deload_map();
	void initial_game(string, Vector2f);
	void center_cam(Vector2f);
	void render_static_map();
	void render_entities();


public:
	//constructors/destructors
	GameState(int, string, Vector2f);
	~GameState();
	
	//public functions:
	void update();
	void render();
	void pollevent();
};
