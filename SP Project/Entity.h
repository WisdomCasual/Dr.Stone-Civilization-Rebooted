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
	short health = 100, damage = 10, state = 2, states_no;
};

struct Entity : public State
{
private:
	entity& entity_stats;
	Sprite entity_sprite;
	render_tile**& static_map;
	float  delay = 0, animation_delay = 0.06, &map_x, &map_y, scale = 1, sprite_scale = 1, win_x = 0, win_y = 0;
	short current_move = 3, current_frame = 0, prev_state = -1;
	IntRect current_rect = { 0,0,0,0 };
	Vector2f current_hitbox = { 0,0 };
	Vector2u prev_win = { 0, 0 };
	const Vector2i hitbox_check[2][2] = { {{1, 0}, {0, -1}}, {{0, 1}, {-1, 0}} };
	bool inbetween_state = 0;

public:
	Entity(entity&, string, render_tile**&, float&, float&);

	~Entity();

	Vector2f getPosition();
	void setPosition(int x_pos, int y_pos);
	void setScale(float);
	bool legal_tile(Vector2f);
	void move(Vector2f);
	void direction(Vector2f);
	void update();
	void pollevent();
	void render();

};

