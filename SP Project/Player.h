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
	void player_movement(float,float,float);
	void updatePos();
	Vector2f delta_movement();

	//Private variables
	bool fake_constructor = 1;

	base_stats* object_stats, **destructable_objects;
	short *item_drops, * item_drops_count;

public:
	Vector2i tool_used_on = { -1,-1 }; //x, y
	short destruction_power = 1;

	~Player();

	void knockback(Vector2f,float);
	void damaged(Color&, float&, Sprite&);
	void Hitbox_align(int);
	void setObjectStats(base_stats*, base_stats**, short*, short*);
	virtual void setPosition(float, float);
	void use_tool();
	void mine();
	void move(Vector2f);
	void setDamage(short);
	void setHealth(short);
	Vector2i block_interaction();
	void interaction_notification(string interaction_type = "Interact");
	void update();
	void pollevent();

};

