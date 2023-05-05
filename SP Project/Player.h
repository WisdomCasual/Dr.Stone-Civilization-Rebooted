#pragma once
#include "Entity.h"


struct Player :
    public Entity
{
	using::Entity::Entity;
private:
	

	//private functions
	void move_cam(float, float);
	void player_movement();

public:

	~Player();

	void Edrab();
	void setPosition(float, float);
	void move(Vector2f);
	void update();
	void pollevent();

};

