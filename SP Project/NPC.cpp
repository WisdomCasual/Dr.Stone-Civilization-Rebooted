#include "NPC.h"

void NPC::player_collision_check()
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
}

bool NPC::collide_with_player(Vector2f movement)
{
	FloatRect player_hitbox = FloatRect(player_entity.getRelativePos().x - player_entity.current_hitbox.x / 2, player_entity.getRelativePos().y - player_entity.current_hitbox.y / 2, player_entity.current_hitbox.x, player_entity.current_hitbox.y);

	FloatRect entity_hitbox = FloatRect(getRelativePos().x + movement.x * move_speed * dt - current_hitbox.x / 2, getRelativePos().y + movement.y * move_speed * dt - current_hitbox.y / 2, current_hitbox.x, current_hitbox.y);

	if (entity_hitbox.intersects(player_hitbox))
		return true;
	else
		return false;
}

NPC::~NPC()
{

}

void NPC::updatePos()
{
	entity_sprite.setPosition((pos.x + map_x) * scale, (pos.y + map_y) * scale);
}

bool NPC::legal_direction(Vector2f tile_pos, short dx, short dy)
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

void NPC::stateMachine()
{
	switch (action_state) {
	case 1: {
		//dialogue behaviour?
		break;
	}
	default:
		//non-dialogue behaviour?
		break;
	}
}

void NPC::update()
{
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
	current_rect = entity_stats.animations[state][current_move].rect;

	entity_sprite.setTextureRect(IntRect(current_rect.left + current_frame * current_rect.width, current_rect.top, current_rect.width, current_rect.height));
	entity_sprite.setOrigin(entity_stats.animations[state][current_move].origin); ///////////////
	updatePos();
	stateMachine();

	player_collision_check();
	if (player_entity.interact && magnitude(player_entity.getRelativePos() - getRelativePos()) <= 32) {
		player_entity.interact = 0;
		cout << "HI!";
	}
	if (will_move && !collide_with_player(dt * move_speed * curr_movement)) {
		move(dt * move_speed * curr_movement);
		direction({ roundf(curr_movement.x), roundf(curr_movement.y) });
	}

	if (game_time - despawn_timer > time_to_despawn && !persistant) {
		despawn = 1;
		return;
	}
	despawn_timer = game_time;
}
