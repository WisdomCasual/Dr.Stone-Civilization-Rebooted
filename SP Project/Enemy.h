#pragma once
#include "Entity.h"
struct Enemy :
    public Entity
{
	using::Entity::Entity;
private:
	Vector2f fov = { 12.f, 120.f }, curr_movement = { 0.f, 0.f }; //fov = (magnitude, angle)
	Vector2i prev_target_tile = { 0, 0 }, last_seen = { 0, 0 };
	Vector2f target_tile = { -1.f, -1.f }, last_seen_cord = { 0.f, 0.f };
	bool will_move = 0, prev_check = 0;
	float theta = 0, motion_delay = 4, sound_range = 5.f,RangeElWgood=90.0;
	path_tile* mp = nullptr;


public:
	~Enemy();
	void Mawgood();
	void updatePos();
	bool visionLines(Entity&);
	bool entityFound(Entity&);
	path_tile* aStar(Vector2i);
	void pathFinding(Entity&, path_tile*& mp);
	Vector2f pathFollow(path_tile*&);
	void stateMachine();
	void Edrab();
	void update();
	void pollevent();
};

