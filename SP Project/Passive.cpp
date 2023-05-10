#include "Passive.h"

Passive::~Passive()
{
}

void Passive::updatePos()
{
	entity_sprite.setPosition((pos.x + map_x) * scale, (pos.y + map_y) * scale);
}

bool Passive::legal_direction(Vector2f tile_pos, short dx, short dy)
{
	short move_num = 0;
	if (dy < 0) {
		move_num = 0;
	}
	else if (dx > 0) {
		move_num = 1;
	}
	else if (dx < 0) {
		move_num = 2;
	}
	else if (dy > 0) {
		move_num = 3;
	}
	return legal_tile(tile_pos, entity_stats.animations[state][move_num].hitbox_rect);
}

void Passive::stateMachine()
{
	switch (state) {
	case 1: {
		motion_delay += dt, switch_delay += dt;
		if (motion_delay >= 1) {
			move_speed = entity_stats.base_movement_speed;
			motion_delay = 0;
			theta = (rand() % 8) * 45;
			will_move = 1;
			direction({ 0, 0 });
			curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
			while (!legal_direction(Vector2f(0.f, 0.f), (short)round(curr_movement.x), (short)round(curr_movement.y))) {
				theta += 45;
				curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
			}
		}
		if (switch_delay >= 8)
			state = 0, motion_delay = 2, will_move = 0;
		break;
	}
	default:
		motion_delay += dt;

		if (motion_delay >= move_for && will_move) {
			will_move = 0;
			direction({ 0, 0 });
		}
		if (motion_delay >= 4) {
			move_speed = entity_stats.base_movement_speed / 2;
			motion_delay = 0;
			theta = (rand() % 8) * 45;
			will_move = rand() % 4;
			move_for = 3 + rand() % 2;
			direction({ 0, 0 });
			curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
			while (!legal_direction(Vector2f(0.f, 0.f), (short)round(curr_movement.x), (short)round(curr_movement.y))) {
				theta += 45;
				curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
			}
		}
		break;
	}
}

void Passive::update()
{
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
		else scale = win_y / 304.5;
		////////////////

	}

	if (state != prev_state) {
		prev_state = state;
		entity_sprite.setTexture(*textures[state]);
	}

	if (active_action) {
		if (delay > animation_delay) {
			delay = 0;
			current_frame++;
			if (current_frame % entity_stats.animations[state][current_move].frames == 0) {
				current_frame = 0, current_move -= 4 * active_action, active_action = 0;
			}
		}
		else
			delay += dt;
	}
	/////////////////////HitBox Stuff//////////////////////
	Entity_Hitbox = { getRelativePos().x - current_hitbox.x / 2,getRelativePos().y - current_hitbox.y / 2,current_hitbox.x,current_hitbox.y };
	//cout << Entity_Hitbox.left << '\t' << Entity_Hitbox.top << '\t' << player_entity.MakanElDarb.left << '\t' << player_entity.MakanElDarb.top<<endl;
	if (player_entity.MakanElDarb.intersects(Entity_Hitbox)) {
		state = 1;
		switch_delay = 0;
		motion_delay = 2;
	}


	current_rect = entity_stats.animations[state][current_move].rect;

	entity_sprite.setTextureRect(IntRect(current_frame * current_rect.left, current_rect.top, current_rect.width, current_rect.height));
	entity_sprite.setOrigin(entity_stats.animations[state][current_move].origin); ///////////////
	updatePos();
	stateMachine();



	if (will_move) {
		short dir[2] = { 45, -45 };
		bool legal_x = legal_tile({ curr_movement.x, 0 }), legal_y = legal_tile({ 0, curr_movement.y });
		if (legal_x)
			move({ dt * move_speed * curr_movement.x, 0 });
		if (legal_y)
			move({ 0, dt * move_speed * curr_movement.y });
		if (!legal_x || !legal_y) {
			short move_offset = dir[rand() % 2];
			theta += move_offset;
			curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
			while (!legal_direction(Vector2f(0.f, 0.f), (short)round(curr_movement.x), (short)round(curr_movement.y))) {
				theta += move_offset;
				curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
			}
		}
		direction({ round(curr_movement.x), round(curr_movement.y) });
	}
}