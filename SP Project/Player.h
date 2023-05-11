#pragma once
#include"Entity.h"

struct base_stats {
	short health = 100;
	short item_drops[5];
	short drops_no = 0;
	short idx = 0;
	short damage = 0;
	short speed = 0;
};

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
	base_stats* object_stats, **destructable_objects;
	short *item_drops, * item_drops_count;

public:
	Vector2i tool_used_on = { -1,-1 }; //x, y

	~Player();

	void knockback();
	void a7mar(Color&, float&, Sprite&);
	void Edrab(int);
	void setObjectStats(base_stats*, base_stats**, short*, short*);
	virtual void setPosition(float, float);
	void use_tool();
	void mine();
	void destroy_object(Vector2i);
	void bigbang(Vector2i, bool);
	void move(Vector2f);
	void update();
	void pollevent();

};

