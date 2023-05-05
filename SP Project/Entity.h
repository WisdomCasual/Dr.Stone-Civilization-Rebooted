#pragma once
#include"State.h"

#include"Global.h"
using namespace globalvar;

struct render_tile {
	Vector3i* layers;
	short size, tile_props = 0;
};
struct animation {
	short frames = 0; IntRect rect{ 0, 0, 0, 0 }; Vector2f hitbox_rect = { 0, 0 }, origin = { 0,0 };
};
struct entity {
	//states[]->moves[]->animation{}
	animation** animations;
	short health = 100, damage = 10, state = 0, states_no;
};

struct Entity : public State
{
private:
	entity &entity_stats;
	Entity& player_entity;
	Sprite entity_sprite;
	render_tile**& static_map;
	float  delay = 0, animation_delay = 0.06, &map_x, &map_y, scale = 1, sprite_scale = 1, win_x = 0, win_y = 0;
	short current_move = 3, current_frame = 0, prev_state = -1;
	IntRect current_rect = { 0,0,0,0 };


	Vector2f current_hitbox = { 0,0 };
	Vector2u prev_win = { 0, 0 };
	const Vector2i hitbox_check[2][2] = { {{1, 0}, {0, -1}}, {{0, 1}, {-1, 0}} };
	int active_action = 0, move_for;
	Vector2f pos = { 0, 0 }, fov = { 10.f, 120.f }, curr_movement = { 0.f, 0.f }, last_seen = {0.f, 0.f}, last_sign; //fov = (magnitude, angle)
	bool is_player = 0, will_move = 0, use_astar = 0;
	float theta = 0;
	//////////////////7agat el darb//////////////////
	FloatRect MakanElDarb, Entity_Hitbox;
	Vector2f RangeElDarb = { 5,5 }, Size_Hitbox = { 10,10 };
	float Lag = 0, motion_delay = 4;
	const float corners[2] = { 1, -1 };

	struct casted_bool {
		bool vision = 0, path = 0;
	};

	struct comparison_tile {
		float cost = 0, x = 0, y = 0;
	};

	struct path_tile {
		float x = 0, y = 0;
		path_tile* parent = nullptr;
	};


	struct path_array {

		int size = 1, curr_idx = 0, layer = 0;
		comparison_tile* at; //dynamic array of objects

		path_array() {
			at = new comparison_tile[1];
		}

		path_array(int size) {
			at = new comparison_tile[size];
			this->size = size;
		}

		~path_array() {
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

		int arr_size() {
			return curr_idx;
		}
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

        /* delet top function */
        void Astika() {
            hp.at[0] = hp.at[size - 1];
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
    } a_star;


public:
	Entity(entity&, string, render_tile**&, float&, float&, Entity* player = nullptr);

	~Entity();

	Vector2f getPosition();
	Vector2f getRelativePos();
	bool is_in_action();
	void updatePos();
	void change_state(int);
	casted_bool visionLines(Entity&);
	casted_bool entityFound(Entity&);
	void setPlayerState(bool state); //1 = player, 0 = non-player
	void setPosition(float x_pos, float y_pos);
	void setScale(float);
	bool legal_tile(Vector2f);
	void move(Vector2f);
	void action(int);
	void direction(Vector2f);
	void stateMachine();
	void Edrab();
	void update();
	void pollevent();
	void render();

};

