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

	//Private variables
	
public:

	~Player();

	void Edrab(int);
	void setPosition(float, float);
	void use_tool();
	void mine();
	void destroy_object(Vector2i);
	void bigbang(Vector2i, bool);
	void move(Vector2f);
	void update();
	void pollevent();

};

