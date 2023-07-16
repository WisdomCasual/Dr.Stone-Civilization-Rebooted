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
	entity_sprite.setPosition(round(map_x * scale) + pos.x * scale, round(map_y * scale) + pos.y * scale);
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

	if (this->curr_dialogue != nullptr)
		delete[] this->curr_dialogue;
	this->curr_dialogue = new dialogue[n];
	for (int i = 0; i < n; i++)
		this->curr_dialogue[i] = curr_dialogue[i];
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
		case 2:             //petrefied
			change_state(1);
			break;
		case 3: {  //wandering
			change_state(0);
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
					NPC_curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
					for (int i = 0; i < 7 && !legal_direction(Vector2f(0.f, 0.f), (short)round(NPC_curr_movement.x), (short)round(NPC_curr_movement.y)); i++) {
						theta += 45;
						NPC_curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
					}
				}
			}
			if (game_time - despawn_timer > time_to_despawn) {

				npc_type = (id == 3) ? 5 : 4;           //MAP  /  trader
				will_move = 0;
				direction({ 0.f, 0.f });
				switch (id) {
					default:
						setPosition(800.f, 800.f);
				}
				return;
			}
			break;

		}
		//case 1: {  //quest
		//	switch (quest_idx) {
		//		case 4: {
		//			if (id == 0) {
		//				if (inventory_count[0] >= 12 && inventory_count[1] >= 8) {
		//					*quest_location = getRelativePos();
		//					
		//				}
		//				else {
		//					quest_location->x = -1.f;
		//				}
		//			}
		//				
		//		}
		//	}
		//}
		default: {          //path following (standing still if no path)
			short curr_tile_x = short(getRelativePos().x / 16), curr_tile_y = short(getRelativePos().y / 16), new_tile_x, new_tile_y;
			if (static_map[curr_tile_x][curr_tile_y].tile_props & 3840) {
				if (dist.x == 0.f && dist.y == 0.f) {
					if (motion_cd <= 0) {
						if (static_map[curr_tile_x][curr_tile_y].tile_props & 3072) {
							path_follow(curr_tile_x, curr_tile_y, -NPC_curr_movement.x, -NPC_curr_movement.y, NPC_prev_tile_x, NPC_prev_tile_y);
						}
						else {
							for (int i = 0; i < 4; i++) {
								//cout << i << '\n';
								new_tile_x = curr_tile_x + dx[i], new_tile_y = curr_tile_y + dy[i];
								if (new_tile_x < size_x && new_tile_x >= 0 && new_tile_y < size_y && new_tile_y >= 0) {
									if ((static_map[new_tile_x][new_tile_y].tile_props & 3840) && (new_tile_x != NPC_prev_tile_x || new_tile_y != NPC_prev_tile_y)) {
										//cout << curr_tile_x << ' ' << curr_tile_y << ' ' << NPC_prev_tile_x << ' ' << NPC_prev_tile_y << ' ' << dx[i] << ' ' << dy[i] << '\n';
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
	NPC_curr_movement = { float(dx), float(dy) };
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
	NPC_prev_tile_x = travel_x - 2 * dx, NPC_prev_tile_y = travel_y - 2 * dy;
	dist.x = (travel_x - new_tile_x) * 16, dist.y = (travel_y - new_tile_y) * 16;
	direction(NPC_curr_movement);
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

void NPC::initialize_NPC(string* map, in_order* order, unsigned short* count, short character_id, Vector2f *quest_location, TradingInfo* trade)
{
	travel_map = map, inventory_order = order, inventory_count = count, player_character_id = character_id, this->quest_location = quest_location, this->trade = trade;
}

void NPC::update()
{
	if (in_dialogue) {
		states->insert({ DialogueID,new DialogueState(curr_dialogue,{0, 140}, 1, curr_dialogue_num) });
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
	despawn_timer = game_time;
	if (magnitude(player_entity.getRelativePos() - getRelativePos()) <= 48) {
		string interaction_message;
		switch (npc_type) {
			case 2:
				interaction_message = "save";
				break;
			default:
				interaction_message = "talk";
		}
		player_entity.interaction_notification(interaction_message);

		if (player_entity.interact) {
			player_entity.interact = 0;
			switch (npc_type) {
				case 1: {      //quest
					switch (quest_idx) {
						if (id == 1) {      //Senku
							case 4: {
								if (inventory_count[0] >= 12 && inventory_count[1] >= 8) {
									dialogue enough[2] = { {"Kaseki", "Great work! You can go now, await further orders", 0, 5}, {character_name, "You got it", 0, 1 + player_character_id} };
									start_dialogue(enough, 2);
									quest_idx++;
									inventory_count[0] -= 12, inventory_count[1] -= 8;
									if (!inventory_count[0]) inventory_order->erase(0);
									if (!inventory_count[1]) inventory_order->erase(1);
									npc_type = 0;
								}
								else {
									dialogue not_enough[1] = { {"Kaseki", "What are you doing here? Go get me the resources", 0, 5} };
									start_dialogue(not_enough, 1);
								}
								break;
							}
						}


					}
					break;
				}

				case 2: { //petrified
					short saved_dialogue_size = 0;
					dialogue* saved_dialogue;
					switch (id) {
					default: {
						saved_dialogue_size = 1;
						saved_dialogue = new dialogue[1];
						if(id == 1)
							saved_dialogue[0] = { "???" ,"Thank you for saving me!", 0, 5 };
						else
							saved_dialogue[0] = { "???" ,"Thank you for saving me!", 0, 6 };
					}

					}
					start_dialogue(saved_dialogue, saved_dialogue_size);
					delete[] saved_dialogue;
					npc_type = 3;     //wandering

					break;
				}
				case 4: {        //trading
					states->insert({ TradingID, new TradingState(inventory_order, inventory_count, *trade) });
					states->at(TradingID)->update();
					break;
				}
				case 5: { ///MAP
					states->insert({ 9, new WorldMapState(*travel_map) });
					states->at(WorldMapID)->update();

					break;
				}
				default: {
					single_dialogue[0] = npc_dialogues[generate_random(0, dialogues_num-1)];
					start_dialogue(single_dialogue, 1);
				}
			}
		}
	}
	if (will_move) {
		switch (npc_type) {
			case 3: {        //wandering
				short dir[2] = { 45, -45 };
				bool legal_x, legal_y;
				if (stun > 0) {
					legal_x = legal_tile({ dt * move_speed * NPC_curr_movement.x, 0 }),
						legal_y = legal_tile({ 0, dt * move_speed * NPC_curr_movement.y });
				}
				else {
					legal_x = legal_direction({ dt * move_speed * NPC_curr_movement.x, 0 }, (short)round(NPC_curr_movement.x), (short)round(NPC_curr_movement.y)),
						legal_y = legal_direction({ 0, dt * move_speed * NPC_curr_movement.y }, (short)round(NPC_curr_movement.x), (short)round(NPC_curr_movement.y));
				}
				if (legal_x)
					move({ dt * move_speed * NPC_curr_movement.x, 0 });
				if (legal_y)
					move({ 0, dt * move_speed * NPC_curr_movement.y });
				if (stun <= 0) {
					if (legal_x || legal_y)
						direction({ roundf(NPC_curr_movement.x), roundf(NPC_curr_movement.y) });
				}
				if ((!legal_x || !legal_y) && stun <= 0) {
					short move_offset = dir[generate_random(0, 1)];
					theta += move_offset;
					NPC_curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
					for (int i = 0; i < 7 && !legal_direction(Vector2f(0.f, 0.f), (short)round(NPC_curr_movement.x), (short)round(NPC_curr_movement.y)); i++) {
						theta += move_offset;
						NPC_curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
					}
				}
				break;
			}
			default: {
				Vector2f movement = { (abs(dt * move_speed * NPC_curr_movement.x) < abs(dist.x)) ? dt * move_speed * NPC_curr_movement.x : dist.x,
									(abs(dt * move_speed * NPC_curr_movement.y) < abs(dist.y)) ? dt * move_speed * NPC_curr_movement.y : dist.y }, prev_hitbox = current_hitbox;;
				if (legal_direction((movement) / 16.f, roundf(NPC_curr_movement.x), roundf(NPC_curr_movement.y)) && !collide_with_player(dt * move_speed * NPC_curr_movement)) {
					dist.x -= movement.x, dist.y -= movement.y;
					move(movement);

					direction(NPC_curr_movement);
				}
				else
					current_hitbox = prev_hitbox;
			}
		}
	}

}
