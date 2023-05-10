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
	bool malosh_lazma = 1;
public:
	Vector2i tool_used_on = { -1,-1 }; //x, y

	~Player();

	void a7mar(Color&, float&, Sprite&);
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

