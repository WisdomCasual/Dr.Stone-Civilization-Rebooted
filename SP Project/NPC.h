#pragma once
#include "Entity.h"
struct NPC :
	public Entity
{
	using::Entity::Entity;
private:
	Vector2f  curr_movement = { 0.f, 0.f }; //fov = (magnitude, angle)
	bool will_move = 0;
	float theta = 0, motion_delay = 4, move_speed = 100, switch_delay = 0;

	//private functions
	void player_collision_check();
	bool collide_with_player(Vector2f);

public:
	~NPC();
	void updatePos();
	bool legal_direction(Vector2f, short, short);
	void stateMachine();
	void update();
};