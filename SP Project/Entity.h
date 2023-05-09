#pragma once
#include"State.h"

#include"Global.h"
using namespace globalvar;

struct render_tile {
	Vector3i* layers;
	short size, tile_props = 0, object_type = -1, tool_type = -1, dynamic_idx = -1;
	bool disable_top = 0;
};

struct animation {
	short frames = 0; IntRect rect{ 0, 0, 0, 0 }; Vector2f hitbox_rect = { 0, 0 }, origin = { 0,0 };
};

struct entity {
	//states[]->moves[]->animation{}
	animation** animations;
	short max_health = 100, base_damage = 10, base_movement_speed = 100, states_no;
	float scale_const = 1;
	~entity() {

		if (animations != nullptr) {
			for (int i = 0; i < states_no; i++)
				if (animations[i] != nullptr)
					delete[] animations[i];
			delete[] animations;
		}
	}
};

struct comparison_tile {
		float cost = 0;
		int x = 0, y = 0;
	};

struct path_tile {
		int x = 0, y = 0;
		Vector2f direction = {-1.f, -1.f};
	};

struct comparison_array {

		int size = 1, curr_idx = 0, layer = 0;
		comparison_tile*at; //dynamic array of objects

		comparison_array() {
			at = new comparison_tile[1];
		}

		comparison_array(int size) {
			at = new comparison_tile[size];
			this->size = size;
		}

		~comparison_array() {
			delete[] at;
		}

		void resize(const int size) {
			comparison_tile* old_dynamic_array = at;
			at = new comparison_tile[size];
			int copy_size = min(curr_idx, size);
			for (int i = 0; i < copy_size; i++)
				at[i] = old_dynamic_array[i];
			this->size = size;
			delete[] old_dynamic_array;
		}

		void add(const comparison_tile new_object) {
			if (curr_idx >= size)
				resize(size * 2);

			at[curr_idx] = new_object, curr_idx++;
		}

		void remove() {
			curr_idx--;
		}

		int arr_size() {
			return curr_idx;
		}
	};

struct tabor_el_3e4 {
	private:


		int size = 0;
		comparison_array hp;
	public:

		tabor_el_3e4() {
			hp.resize(128);
		}
		/* heap algorithm */
		void sheftup(int indx) {
			int parent = (indx - 1) / 2;
			if (hp.at[parent].cost > hp.at[indx].cost) {
				swap(hp.at[parent], hp.at[indx]);
				sheftup(parent);
			}
		}
		void sheftdown(int indx) {
			int min_valu = indx, left = (indx * 2) + 1, right = (indx * 2) + 2;
			if (left < size && hp.at[left].cost < hp.at[min_valu].cost)
				min_valu = left;

			if (right < size && hp.at[right].cost < hp.at[min_valu].cost)
				min_valu = right;


			if (min_valu != indx) {

				swap(hp.at[min_valu], hp.at[indx]);
				sheftdown(min_valu);

			}
		}


		/* insert function */


		void Ed5ol(comparison_tile dist) {
			size++;
			hp.add(dist);
			sheftup(size - 1);
		}

		/* empty function */
		bool Fare8() {
			if (!size)
				return 1;
			else return 0;
		}

		/* delete top function */
		void Astika() {
			hp.at[0] = hp.at[size - 1];
			hp.remove();
			size--;
			sheftdown(0);
		}

		/* clear function */
		void shinra_tensi() {
			size = 0;
		}



		comparison_tile top() {
			if (!Fare8()) {

				return hp.at[0];
			}
			return comparison_tile(-1, -1, -1);
		}
	};

struct Entity : public State
{


public:

	//Public Variables for all sub-classes
	entity& entity_stats;
	Entity& player_entity;
	Sprite entity_sprite;
	render_tile**& static_map;
	float  delay = 0, animation_delay = 0.06, & map_x, & map_y, scale = 1, sprite_scale = 1, win_x = 0, win_y = 0;
	int &size_x, &size_y;
	float& x_offset, & y_offset;
	short current_move = 3, current_frame = 0, prev_state = -1, &disable_dynamic_obj;
	IntRect current_rect = { 0,0,0,0 };
	Vector2f current_hitbox = { 0,0 };
	Vector2u prev_win = { 0, 0 };
	const Vector2i hitbox_check[2][2] = { {{1, 0}, {0, -1}}, {{0, 1}, {-1, 0}} };
	Vector2i current_direction = { 0,1 };
	int active_action = 0, move_for = 0;
	Vector2f pos = { 0, 0 };
	const float corners[2] = { 1, -1 };
	sheet_properties* tile_props;
	
	bool despawn = 0;
	short state = 0, health = 100, damage = 10;

	//////////////////7agat el darb//////////////////
	FloatRect MakanElDarb, Entity_Hitbox;
	Vector2f RangeElDarb = { 5,5 }, Size_Hitbox = { 10,10 };
	float Lag = 0;



	//Public functions
	Entity(entity&, string, render_tile**&, sheet_properties*, float&, float&, int&, int&, float&, float&, short&, Entity* player = nullptr);

	virtual ~Entity();

	Vector2f getPosition();
	Vector2f getRelativePos();
	void change_state(int); 
	void setPosition(float x_pos, float y_pos);
	void set_movement_speed(short speed);
	void setScale(float);
	bool legal_tile(Vector2f, Vector2f curr_hitbox = { -1, -1 });
	virtual void move(Vector2f);
	void action(int);
	void direction(Vector2f);
	virtual void update() = 0;
	virtual void pollevent();
	void render();

};