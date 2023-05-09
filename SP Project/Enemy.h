#pragma once
#include "Entity.h"

struct Enemy :
    public Entity
{
	using::Entity::Entity;
private:
	Vector2f fov = { 12.f, 120.f }, curr_movement = { 0.f, 0.f }; //fov = (magnitude, angle)
	Vector2i prev_target_tile = { 0, 0 }, last_seen = { 0, 0 }, path_delta = {-1, -1}; //astar pos + map_delta = actual position 
	Vector2f target_tile = { -1.f, -1.f }, last_seen_cord = { 0.f, 0.f }, delta_sign = {-1, -1};
	bool will_move = 0, prev_check = 0;
	float theta = 0, motion_delay = 4, sound_range = 5.f, move_speed = 100, RangeElWgood = 90.0;
	path_tile* mp = nullptr;
	short find_size_x = 50, find_size_y = 50, id = 1, *** vis = nullptr;
	bool* astar_done = nullptr;



public:
	~Enemy();
	void Mawgood();
	void updatePos();
	bool visionLines(Entity&);
	bool entityFound(Entity&);
	bool legal_direction(Vector2f, short, short);
	path_tile* aStar(Vector2i);
	void pathFinding(Entity&, path_tile*& mp);
	Vector2f pathFollow(path_tile*&);
	void stateMachine();
	void setID(short);
	void setVisArray(short***, bool* astar_done, short new_find_size_x = 50, short new_find_size_y = 50);
	void Edrab();
	void update();
};

