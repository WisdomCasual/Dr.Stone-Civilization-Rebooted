#include "NPC.h"
#include "DIALOGUESTATE.H"

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

	player_hitbox.top -= player_entity.movement.y * player_entity.entity_stats.base_movement_speed * dt;
	player_hitbox.left += player_entity.knockback_direction.x * player_entity.knockback_v * dt;

	if (entity_hitbox.intersects(player_hitbox))
		player_entity.knockback_direction.x = 0;

	player_hitbox.left -= player_entity.knockback_direction.x * player_entity.knockback_v * dt;
	player_hitbox.top += player_entity.knockback_direction.y * player_entity.knockback_v * dt;

	if (entity_hitbox.intersects(player_hitbox))
		player_entity.knockback_direction.y = 0;
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
	if (npc_dialogues != nullptr)
		delete[] npc_dialogues;
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

void NPC::set_type(short type)
{
	npc_type = type;
}

void NPC::set_dialogue(dialogue* dialogues, short n)
{
	dialogues_num = n;
	if (npc_dialogues != nullptr)
		delete[] npc_dialogues;
	npc_dialogues = new dialogue[n];
	for (int i = 0; i < n; i++)
		npc_dialogues[i] = dialogues[i];
}

void NPC::start_dialogue(dialogue* curr_dialogue, short n)
{
	this->curr_dialogue = curr_dialogue;
	curr_dialogue_num = n;

	Vector2f dialogue_dir = (player_entity.getRelativePos() - getRelativePos()) / magnitude(player_entity.getRelativePos() - getRelativePos()), prev_hitbox = current_hitbox;
	if (legal_direction({0,0}, roundf(dialogue_dir.x), roundf(dialogue_dir.y)) && !collide_with_player({0, 0})) {
		direction(Vector2f(roundf(dialogue_dir.x), roundf(dialogue_dir.y)));
		update_looks();
	}
	else {
		current_hitbox = prev_hitbox;
	}
	in_dialogue = 1;
}

void NPC::type_behaviour()
{
	switch (npc_type) {
		default: {
			short curr_tile_x = short(getRelativePos().x / 16), curr_tile_y = short(getRelativePos().y / 16), new_tile_x, new_tile_y;
			if (static_map[curr_tile_x][curr_tile_y].tile_props & 3840) {
				if (dist.x == 0.f && dist.y == 0.f) {
					if (motion_cd <= 0) {
						if (static_map[curr_tile_x][curr_tile_y].tile_props & 3072) {
							path_follow(curr_tile_x, curr_tile_y, -curr_movement.x, -curr_movement.y, prev_tile_x, prev_tile_y);
						}
						else {
							for (int i = 0; i < 4; i++) {
								//cout << i << '\n';
								new_tile_x = curr_tile_x + dx[i], new_tile_y = curr_tile_y + dy[i];
								if (new_tile_x < size_x && new_tile_x >= 0 && new_tile_y < size_y && new_tile_y >= 0) {
									if ((static_map[new_tile_x][new_tile_y].tile_props & 3840) && (new_tile_x != prev_tile_x || new_tile_y != prev_tile_y)) {
										//cout << curr_tile_x << ' ' << curr_tile_y << ' ' << prev_tile_x << ' ' << prev_tile_y << ' ' << dx[i] << ' ' << dy[i] << '\n';
										path_follow(curr_tile_x, curr_tile_y, dx[i], dy[i], new_tile_x, new_tile_y);
										break;
									}
								}
							}
						}
					}
					else {
						will_move = 0, motion_cd -= dt;
						//cout << motion_cd << '\n';
					}
				}

			}
			break;
		}
	}
}

void NPC::path_follow(short curr_tile_x, short curr_tile_y, short dx, short dy, short new_tile_x, short new_tile_y)
{
	curr_movement = { float(dx), float(dy) };
	short j = 0, travel_x = new_tile_x + dx * j, travel_y = new_tile_y + dy * j;
	while (travel_x < size_x && travel_x >= 0 && travel_y >= 0 && travel_y < size_y && (static_map[travel_x][travel_y].tile_props & 3840)) {
		j++;
		if (static_map[travel_x][travel_y].tile_props & 2560) {
			motion_cd = 1.f;
			break;
		}
		travel_x = new_tile_x + dx * j, travel_y = new_tile_y + dy * j;
	}
	travel_x = new_tile_x + dx * j, travel_y = new_tile_y + dy * j;
	prev_tile_x = travel_x - 2 * dx, prev_tile_y = travel_y - 2 * dy;
	dist.x = (travel_x - new_tile_x) * 16, dist.y = (travel_y - new_tile_y) * 16;
	direction(curr_movement);
	move_speed = entity_stats.base_movement_speed / 2;
	will_move = 1;
	//cout << dist_x << ' ' << dist_y << '\n';

}

void NPC::update_looks()
{
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
	current_rect = entity_stats.animations[state][current_move].rect;

	entity_sprite.setTextureRect(IntRect(current_rect.left + current_frame * current_rect.width, current_rect.top, current_rect.width, current_rect.height));
	entity_sprite.setOrigin(entity_stats.animations[state][current_move].origin); ///////////////
}

void NPC::update()
{
	if (game_time - despawn_timer > time_to_despawn && !persistant) {
			despawn = 1;
			return;
		}
	despawn_timer = game_time;
	if (in_dialogue) {
		states->insert({ DialogueID,new DialogueState(curr_dialogue,{win_x / 2,win_y / 2},scale / 2, curr_dialogue_num) });
		states->at(DialogueID)->update();
		in_dialogue = 0;
		//cout << "\n------------------------------------\n";
		return;
	}
	//cout << "A";
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
		else scale = win_y / 304.5;
		////////////////
		entity_sprite.setScale(scale * entity_stats.scale_const, scale * entity_stats.scale_const);
	}

	update_looks();
	updatePos();
	type_behaviour();
	player_collision_check();
	if (magnitude(player_entity.getRelativePos() - getRelativePos()) <= 48) {
		player_entity.interaction_notification("Talk");

		if (player_entity.interact) {
			player_entity.interact = 0;
			switch (npc_type) {
			default: {
				single_dialogue[0] = npc_dialogues[generate_random(0, dialogues_num-1)];
				start_dialogue(single_dialogue, 1);
			}
			}
		}
	}
	if (will_move) {
		Vector2f movement = { (abs(dt * move_speed * curr_movement.x) < abs(dist.x)) ? dt * move_speed * curr_movement.x : dist.x,
							(abs(dt * move_speed * curr_movement.y) < abs(dist.y)) ? dt * move_speed * curr_movement.y : dist.y }, prev_hitbox = current_hitbox;;
		if (legal_direction((movement) / 16.f, roundf(curr_movement.x), roundf(curr_movement.y)) && !collide_with_player(dt * move_speed * curr_movement)) {
			dist.x -= movement.x, dist.y -= movement.y;
			move(movement);

			direction(curr_movement);
		}
		else
			current_hitbox = prev_hitbox;
	}

}
