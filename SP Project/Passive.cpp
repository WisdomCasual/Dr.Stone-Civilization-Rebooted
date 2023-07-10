#include "Passive.h"

void Passive::player_collision_check()
{
	FloatRect player_hitbox = FloatRect(player_entity.getRelativePos().x - player_entity.current_hitbox.x / 2, player_entity.getRelativePos().y - player_entity.current_hitbox.y / 2, player_entity.current_hitbox.x, player_entity.current_hitbox.y);

	entity_hitbox = FloatRect(getRelativePos().x - current_hitbox.x / 2, getRelativePos().y - current_hitbox.y / 2, current_hitbox.x, current_hitbox.y);

	player_hitbox.left += player_entity.movement.x * player_entity.entity_stats.base_movement_speed * dt;

	if (entity_hitbox.intersects(player_hitbox))
		player_entity.movement.x = 0;

	player_hitbox.left -= player_entity.movement.x * player_entity.entity_stats.base_movement_speed * dt;
	player_hitbox.top += player_entity.movement.y * player_entity.entity_stats.base_movement_speed * dt;

	if (entity_hitbox.intersects(player_hitbox))
		player_entity.movement.y = 0;

	player_hitbox.top -= player_entity.movement.y * player_entity.entity_stats.base_movement_speed * dt;
	player_hitbox.left += player_entity.knockback_direction.x * player_entity.knockback_v * dt;

	if (entity_hitbox.intersects(player_hitbox))
		player_entity.knockback_direction.x = 0;

	player_hitbox.left -= player_entity.knockback_direction.x * player_entity.knockback_v * dt;
	player_hitbox.top += player_entity.knockback_direction.y * player_entity.knockback_v * dt;

	if (entity_hitbox.intersects(player_hitbox))
		player_entity.knockback_direction.y = 0;
}

bool Passive::collide_with_player(Vector2f movement)
{
	FloatRect player_hitbox = FloatRect(player_entity.getRelativePos().x - player_entity.current_hitbox.x / 2, player_entity.getRelativePos().y - player_entity.current_hitbox.y / 2, player_entity.current_hitbox.x, player_entity.current_hitbox.y);

	FloatRect entity_hitbox = FloatRect(getRelativePos().x + movement.x * move_speed * dt - current_hitbox.x / 2, getRelativePos().y + movement.y * move_speed * dt - current_hitbox.y / 2, current_hitbox.x, current_hitbox.y);

	if (entity_hitbox.intersects(player_hitbox))
		return true;
	else
		return false;
}

Passive::~Passive()
{

}

void Passive::passive_knockback(Vector2f direction, float velocity)
{
	knockback_de = direction;
	curr_movement = curr_movement * move_speed + knockback_de * velocity;
	move_speed = 1;
}

void Passive::damaged()
{
	random_num = generate_random(0, 2);
	sounds[random_num].setVolume(game_volume);
	sounds[random_num].setPosition((pos.x - player_entity.getRelativePos().x) / 4, (pos.y - player_entity.getRelativePos().y) / 4, 0);
	sounds[random_num].play();

	entity_sprite.setColor(Color(255, 155, 155));
	stun = 0.4;
}

void Passive::updatePos()
{
	entity_sprite.setPosition(round(map_x * scale) + pos.x * scale, round(map_y * scale) + pos.y * scale);
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
	return legal_tile(tile_pos, entity_stats.animations[state][move_num].hitbox_rect) && !collide_with_player(Vector2f(dx, dy));
}

void Passive::stateMachine()
{
	switch (action_state) {
	case 1: {
		motion_delay += dt, switch_delay += dt;
		if (motion_delay >= 1) {
			move_speed = entity_stats.base_movement_speed;
			motion_delay = 0;
			theta = (generate_random(0, 7)) * 45;
			will_move = 1;
			direction({ 0, 0 });
			curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
			for (int i = 0; i < 7 && !legal_direction(Vector2f(0.f, 0.f), (short)round(curr_movement.x), (short)round(curr_movement.y)); i++) {
				theta += 45;
				curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
			}
		}
		if (switch_delay >= 8)
			action_state = 0, motion_delay = 2, will_move = 0;
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
			will_move = generate_random(0, 3);
			move_for = generate_random(3, 4);
			if (will_move) {
				theta = (generate_random(0, 7)) * 45;
				direction({ 0, 0 });
				curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
				for (int i = 0; i < 7 && !legal_direction(Vector2f(0.f, 0.f), (short)round(curr_movement.x), (short)round(curr_movement.y)); i++) {
					theta += 45;
					curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
				}
			}
		}
		break;
	}
}

void Passive::update()
{
	if (game_time - despawn_timer > time_to_despawn && !persistant) {
		despawn = 1;
		return;
	}
	despawn_timer = game_time;
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
		else scale = win_y / 304.5;
		////////////////
		entity_sprite.setScale(scale * entity_stats.scale_const, scale * entity_stats.scale_const);
	}

	if (state != prev_state) {
		prev_state = state;
		entity_sprite.setTexture(*entity_stats.textures[state]);
	}

	if (active_action) {
		while (delay > animation_delay) {
			delay -= animation_delay;
			current_frame++;
			if (current_frame % entity_stats.animations[state][current_move].frames == 0) {
				current_frame = 0, current_move -= 4 * active_action, active_action = 0;
			}
		}
		delay += dt;
	}
	/////////////////////HitBox Stuff//////////////////////
	/////////////WARNING TOUCH AT YOUR OWN RISK////////////
	Entity_Hitbox = { getRelativePos().x - current_hitbox.x / 2,getRelativePos().y - current_hitbox.y / 2,current_hitbox.x,current_hitbox.y };
	//cout << Entity_Hitbox.left << '\t' << Entity_Hitbox.top << '\t' << player_entity.hit_range.left << '\t' << player_entity.hit_range.top<<endl;
	if (player_entity.hit_range.intersects(Entity_Hitbox)) {
		if (cooldown<=0) {
			passive_knockback(Vector2f(player_entity.current_direction), 120);
			damaged();
			player_entity.combat_tag = combat_status_time;
			action_state = 1;
			switch_delay = 0;
			motion_delay = 2;
			cooldown = 1;
			health -= player_entity.damage;
		}
	}
	if (health <= 0) {
		despawn = 1;
	}
	if (cooldown)cooldown -= dt;
	//cout << health << endl;
	if (stun <= 0) {
		entity_sprite.setColor(Color(255, 255, 255));
		knockback_ve = 0;
		stateMachine();
	}
	else {
		stun -= dt;
		will_move = 1;
		curr_movement -= knockback_de * dt * 15.f;
	}
	///////////////////////////////////////////////////////
	current_rect = entity_stats.animations[state][current_move].rect;

	entity_sprite.setTextureRect(IntRect(current_rect.left + current_frame * current_rect.width, current_rect.top, current_rect.width, current_rect.height));
	entity_sprite.setOrigin(entity_stats.animations[state][current_move].origin); ///////////////
	updatePos();

	player_collision_check();

	if (will_move) {
		short dir[2] = { 45, -45 };
		bool legal_x, legal_y;
		if (stun > 0) {
			legal_x = legal_tile({ dt * move_speed * curr_movement.x, 0 }),
				legal_y = legal_tile({ 0, dt * move_speed * curr_movement.y });
		}
		else {
			legal_x = legal_direction({ dt * move_speed * curr_movement.x, 0 }, (short)round(curr_movement.x), (short)round(curr_movement.y)),
				legal_y = legal_direction({ 0, dt * move_speed * curr_movement.y }, (short)round(curr_movement.x), (short)round(curr_movement.y));
		}
		if (legal_x)
			move({ dt * move_speed * curr_movement.x, 0 });
		if (legal_y)
			move({ 0, dt * move_speed * curr_movement.y });
		if (stun<=0) {
			if (legal_x || legal_y)
				direction({ roundf(curr_movement.x), roundf(curr_movement.y) });
		}
		if ((!legal_x || !legal_y) && stun <=0) {
			short move_offset = dir[generate_random(0, 1)];
			theta += move_offset;
			curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
			for (int i = 0; i < 7 && !legal_direction(Vector2f(0.f, 0.f), (short)round(curr_movement.x), (short)round(curr_movement.y)); i++) {
				theta += move_offset;
				curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
			}
		}
	}

}
