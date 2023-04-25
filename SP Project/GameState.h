#pragma once
#include"State.h"
#include"Game.h"
#include "Entity.h"

#include"Global.h"
using namespace globalvar;

struct GameState : public State
{
private:
	//variables:

	entity player_stats;
	Entity player_entity;

	Vector2f clicked_on = { -1, -1 };

	render_tile** static_map, **temp_front;

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

		int size = 1, curr_idx = 0;
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

		void resize(int size) {
			entity_object* new_dynamic_array = new entity_object[size];
			for (int i = 0; i < curr_idx; i++)
				new_dynamic_array[i] = at[i];
			this->size = size;
			delete[] at;
			at = new_dynamic_array;
		}

		void add(entity_object new_object) {
			if (curr_idx >= size)
				resize(size*2);

			at[curr_idx] = new_object, curr_idx++;
		}

		int arr_size() {
			return curr_idx + 1;
		}
	};

	struct dynamic_objects_array {
		int size = 1, curr_idx = 0;
		dynamic_objects* at;  //dynamic 2D array of objects

		dynamic_objects_array() {
			at = new dynamic_objects[1];
		}

		dynamic_objects_array(int size) {
			at = new dynamic_objects[size];
			this->size = size;
		}

		~dynamic_objects_array() {
			delete[] at;
		}

		void resize(int size) {
			dynamic_objects* new_dynamic_array = new dynamic_objects[size];
			for (int i = 0; i < curr_idx; i++)
				new_dynamic_array[i] = at[i];
			this->size = size;
			delete[] at;
			at = new_dynamic_array;
		}

		void add(dynamic_objects new_object) {
			if (curr_idx >= size)
				resize(size * 2);

			at[curr_idx] = new_object, curr_idx++;
		}

		int arr_size() {
			return curr_idx + 1;
		}
	} dynamic_map;

	struct pointr {
		dynamic_objects* tile = nullptr; Entity* entity = nullptr;
	};

	//not working
	//set<pair<float, pointr>> dynamic_items; // tile , entity



	int size_x = 0, size_y = 0;  //<-- map size
	int x = 0, y = 0;    //<-- location of upper left corner of the map
	float x_offset = 0, y_offset = 0; //<-- offset from upper left corner of the screen to upper left corner of the map
	float map_x = 0, map_y = 0;
	float scale = 1, x_scale = 1, y_scale = 1, win_x = 0, win_y = 0;
	Vector2u prev_win = { 0, 0 };
	Sprite tile;


	//private functions:
	void load_map(string);
	void deload_map();
	void initial_game();
	void render_static_map();
	void move_cam(float, float);
	void player_movement();

public:
	//constructors/destructors
	GameState();
	~GameState();
	
	//public functions:
	void update();
	void render();
	void pollevent();
};
