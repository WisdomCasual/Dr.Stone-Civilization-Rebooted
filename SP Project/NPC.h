#pragma once
#include "Entity.h"
//#include "DialogueState.h"
struct NPC :
	public Entity
{
	using::Entity::Entity;
private:
	Vector2f dist = { 0, 0 };
	bool will_move = 0, in_dialogue = 0;
	float motion_delay = 4.f, move_speed = 100.f, switch_delay = 0.f, motion_cd = 0.f, theta = 0.f;
	dialogue* npc_dialogues = nullptr, single_dialogue[1] = {}, * curr_dialogue = nullptr;
	string *travel_map = nullptr;
	short dialogues_num, curr_dialogue_num = 0;
	in_order *inventory_order = nullptr;
	unsigned short *inventory_count = nullptr;

	//private functions
	void player_collision_check();
	bool collide_with_player(Vector2f);

public:
	~NPC();
	void updatePos();
	bool legal_direction(Vector2f, short, short);
	void set_type(short);
	void set_dialogue(dialogue*, short);
	void start_dialogue(dialogue*, short);
	void type_behaviour();
	void path_follow(short , short , short , short , short , short );
	void update_looks();
	void initialize_NPC(string*, in_order*, unsigned short*);
	void update();
};