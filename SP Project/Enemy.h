#pragma once
#include "Entity.h"
struct Enemy :
    public Entity
{
	using::Entity::Entity;
private:
	Vector2f fov = { 10.f, 120.f }, curr_movement = { 0.f, 0.f }, last_seen = { 0.f, 0.f }, last_sign; //fov = (magnitude, angle)
	bool will_move = 0, use_astar = 0;
	float theta = 0, motion_delay = 4;

public:

	void updatePos();
	casted_bool visionLines(Entity&);
	casted_bool entityFound(Entity&);
	void stateMachine();
	void Edrab();
	void update();
	void pollevent();
};

