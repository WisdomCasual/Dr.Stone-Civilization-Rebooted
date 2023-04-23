#pragma once
#include"State.h"

#include"Global.h"
	using namespace globalvar;

struct animation {
	short frames = 0; IntRect rect{ 0, 0, 0, 0 }, hitbox_rect{ 0, 0, 0, 0 };
};
struct entity {
	//states[]->moves[]->animation{}
	animation** animations;
	short health = 100, damage = 10, move_speed = 1, state = 0;
};

struct Entity : public State
{
private:
	entity& entity_stats;
	Sprite entity_sprite;
	float  delay = 0, animation_delay = 0.1;
	short current_move = 3, current_frame = 0, prev_state = -1;
	IntRect current_rect = { 0,0,0,0 };

public:
	Entity(entity&, string = "");

	~Entity();

	Vector2f getPosition();
	void setPosition(int x_pos, int y_pos);
	void setScale(float);
	void move(Vector2f);
	void direction(Vector2i);
	void update();
	void pollevent();
	void render();

};

