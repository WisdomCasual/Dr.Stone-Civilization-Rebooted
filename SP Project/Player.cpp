#include "Player.h"
#include "GameState.h"

Player::~Player()
{

}

void Player::player_movement(float x_movement,float y_movement,float velocity)
{
	Vector2f v_direction = { x_movement, y_movement };
	x_movement *= velocity * dt, y_movement *=  velocity * dt;
	bool moving = false;

	if (z_scale < 0.7 || (x_movement && legal_tile({ x_movement, 0 }))) {
		moving = true;
		pos.x += x_movement;
	}
	

	if (z_scale < 0.7 || (y_movement && legal_tile({ 0, y_movement }))) {
		moving = true;
		pos.y += y_movement;
	}

	pos.x = clamp<float>(pos.x, 0.f, size_x * 16.f);
	pos.y = clamp<float>(pos.y, 0.f, size_y * 16.f);

	moving &= (z_scale == 1.f);

	direction(delta_movement(), moving);
}

void Player::updatePos()
{
	entity_sprite.setPosition(round(map_x * (scale * z_scale)) + pos.x * (scale * z_scale),
							  round(map_y * (scale * z_scale)) + pos.y * (scale * z_scale) - (1 - z_scale) * 200.f * scale);
}

Vector2f Player::delta_movement()
{
	// returns direction of movement based on arrow/wasd keys --> {[-1,1],[-1,1]}
	Vector2f velocity{ 0,0 };
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
		velocity.x++;
	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
		velocity.x--;
	if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
		velocity.y--;
	if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S))
		velocity.y++;
	if (0 != velocity.x && 0 != velocity.y)
		velocity.x *= 0.707f, velocity.y *= 0.707f;
	return velocity;
}

void Player::knockback(Vector2f direction,float v) {
	knockback_direction = direction;
	knockback_v = v;
}

void Player::damaged()
{
	entity_sprite.setColor(Color(255, 155, 155));
	stun = 0.4f;
}

void Player::Hitbox_align(int animation)
{
	if (current_move == 0) { //UP
		if (animation)
			hit_range = { getRelativePos().x - 30, getRelativePos().y - 35,60,28 };
		else
			hit_range = { getRelativePos().x - 25, getRelativePos().y - 35,60,28 };
	}
	else if (current_move == 1) { //RIGHT
		if (animation)
			hit_range = { getRelativePos().x + 10, getRelativePos().y - 25,40,28 };
		else
			hit_range = { getRelativePos().x + 10, getRelativePos().y - 25,40,22 };
	}
	else if (current_move == 2) { //LEFT
		if (animation)
			hit_range = { getRelativePos().x - 45, getRelativePos().y - 25,35,27 };
		else
			hit_range = { getRelativePos().x - 45, getRelativePos().y - 25,35,27 };
	}
	else if (current_move == 3) { //DOWN
		if (animation)
			hit_range = { getRelativePos().x - 15, getRelativePos().y - 3,48,19 };
		else
			hit_range = { getRelativePos().x - 23, getRelativePos().y-3,55,19 };
	}
	/*test.setFillColor(Color::Magenta);
	test.setSize({hit_range.width*scale,hit_range.height*scale});
	test.setPosition((hit_range.left + map_x) * scale, (hit_range.top + map_y) * scale);*/
	
}

void Player::setObjectStats(base_stats* object_stats, base_stats** destructable_objects, short* item_drops, short* item_drops_count)
{
	this->object_stats = object_stats;
	this->destructable_objects = destructable_objects;
	this->item_drops = item_drops;
	this->item_drops_count = item_drops_count;
}

void Player::setPosition(float x_pos, float y_pos)
{
	pos = { x_pos, y_pos };
	entity_sprite.setPosition(pos);
}

void Player::use_tool()
{
	if (inBalloon)
		return;

	if (Lag >= 0.8 && stun<=0) {
		if (state == 3) { // sword
			random_num = generate_random(0, 1);
			Hitbox_align(random_num);
			action(random_num + 1);
			sounds[random_num].setVolume(game_volume);
			sounds[random_num].play();
		}
		else if (state == 2 || state == 1 || state == 4) { // axe / pickaxe / hand pickup
			mine();
			action(1);
		}
		Lag = 0;
	}
}

void Player::mine()
{
	Vector2f check_block = { getRelativePos().x + current_direction.x * 16 , getRelativePos().y  + current_direction.y * 16};
	Vector2i destroy_location = {-10,-10}, core_location = { -10,-10 };

	if (static_map[(int)(check_block.x / 16)][(int)(check_block.y / 16)].tile_props & 32) {
		destroy_location = { (int)(check_block.x / 16),  (int)(check_block.y / 16) };
		core_location = destroy_location;
	}
	else if (static_map[(int)(check_block.x / 16)][(int)(check_block.y / 16)].tile_props & 32)
		destroy_location = { (int)(check_block.x / 16), (int)(check_block.y / 16) };

	else if (current_direction.x) {
		if (static_map[(int)(check_block.x / 16)][(int)((check_block.y + 4) / 16)].tile_props & 32) {
			destroy_location = { (int)(check_block.x / 16),  (int)((check_block.y + 4) / 16) };
			core_location = destroy_location;
		}
		else if (static_map[(int)(check_block.x / 16)][(int)((check_block.y - 4) / 16)].tile_props & 32) {
			destroy_location = { (int)(check_block.x / 16),  (int)((check_block.y - 4) / 16) };
			core_location = destroy_location;
		}
		else if (static_map[(int)(check_block.x / 16)][(int)((check_block.y + 4) / 16)].tile_props & 128)
			destroy_location = { (int)(check_block.x / 16), (int)((check_block.y + 4) / 16) };

		else if (static_map[(int)(check_block.x / 16)][(int)((check_block.y - 4) / 16)].tile_props & 128)
			destroy_location = { (int)(check_block.x / 16), (int)((check_block.y - 4) / 16) };
	}

	else if (current_direction.y) {
		if (static_map[(int)((check_block.x + 4) / 16)][(int)(check_block.y / 16)].tile_props & 32) {
			destroy_location = { (int)((check_block.x + 4) / 16),  (int)(check_block.y / 16) };
			core_location = destroy_location;
		}
		else if (static_map[(int)((check_block.x - 4) / 16)][(int)(check_block.y / 16)].tile_props & 32) {
			destroy_location = { (int)((check_block.x - 4) / 16),  (int)(check_block.y / 16) };
			core_location = destroy_location;
		}

		else if (static_map[(int)((check_block.x + 4) / 16)][(int)(check_block.y / 16)].tile_props & 128)
			destroy_location = { (int)((check_block.x + 4) / 16),  (int)(check_block.y / 16) };

		else if (static_map[(int)((check_block.x - 4) / 16)][(int)(check_block.y / 16)].tile_props & 128)
			destroy_location = { (int)((check_block.x - 4) / 16),  (int)(check_block.y / 16) };
	}

	if (destroy_location.x > -10) {
		for (int i = 0; i < 3; i++)
			for (int j = 1; j < 4; j++)
				if (static_map[destroy_location.x + dx[i]][destroy_location.y + dy[j]].tile_props & 32) {
					core_location = { destroy_location.x + dx[i], destroy_location.y + dy[j] };
					break;
				}

		if (static_map[core_location.x][core_location.y].tool_type == state) {
			if (state == 2) {
				random_num = generate_random(2, 5);
				sounds[random_num].setVolume(game_volume);
				sounds[random_num].play();
			}
			else if (state == 1) {
				random_num = generate_random(6, 9);
				sounds[random_num].setVolume(game_volume);
				sounds[random_num].play();
			}

			tool_used_on = { destroy_location.x * 16 + 8, destroy_location.y * 16 + 8};

		   (*destructable_objects)[static_map[core_location.x][core_location.y].object_ID].health -= destruction_power;
		   if ((*destructable_objects)[static_map[core_location.x][core_location.y].object_ID].health < 0) {
			   //resets stats
			   (*destructable_objects)[static_map[core_location.x][core_location.y].object_ID] = object_stats[(*destructable_objects)[static_map[core_location.x][core_location.y].object_ID].idx];
			   //destroys object
			   *item_drops_count = (*destructable_objects)[static_map[core_location.x][core_location.y].object_ID].drops_no;
			   for (int i = 0; i < *item_drops_count; i++) {
				   item_drops[i] = (*destructable_objects)[static_map[core_location.x][core_location.y].object_ID].item_drops[i];
			   }
			   destroy_object_location = core_location;
		   }
		}
	}
}

void Player::move(Vector2f movement)
{
	entity_sprite.move(movement);
	pos += movement;
}

void Player::setDamage(short new_damage)
{
	entity_stats.base_damage = new_damage;
	damage = new_damage;
}

void Player::setHealth(short new_health)
{
	entity_stats.max_health = new_health;
	health = new_health;
}

Vector2i Player::block_interaction()
{
	if (inBalloon)
		return Vector2i(-1, -1);
	if (interact) {
		for (int i = 0; i < 2; i++) {
			short new_tile_x = short(getRelativePos().x / 16.f + i * current_direction.x), new_tile_y = short(getRelativePos().y / 16.f + i * current_direction.y);
			if (new_tile_x >= 0 && new_tile_y >= 0 && new_tile_x < size_x && new_tile_y < size_y && (static_map[new_tile_x][new_tile_y].tile_props & 4096)) {
				interact = 0;
				//cout << "a";
				return Vector2i(new_tile_x, new_tile_y);
			}

		}
	}
	return Vector2i(-1, -1);
}

void Player::interaction_notification(string interaction_message)
{

	string notification_s[] = {interaction_message};
	game.notification(notification_s, 1, dt);
}

void Player::update(float scale, float z_scale)
{
	this->z_scale = z_scale;

	if (this->scale != scale) {
		this->scale = scale;
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

	updatePos();

	combat_tag -= dt;

	Entity_Hitbox = { getRelativePos().x - current_hitbox.x / 2,getRelativePos().y - current_hitbox.y / 2,current_hitbox.x,current_hitbox.y };
	if (Lag >= 0.4) {
		hit_range = { -10,-10,1,1 };
	}
	if (Lag < 0.8)
		Lag += dt;

	if (state == 0 && !sounds[10].getStatus() && !sounds[11].getStatus() && !sounds[12].getStatus() && !sounds[13].getStatus()) {
		random_num = generate_random(10, 13);
		sounds[random_num].setVolume(game_volume);
		sounds[random_num].play();
	}


	if (stun <= 0) {
		//cout << "here\n";
		//cout << stun << endl;
		entity_sprite.setColor(Color(255, 255, 255));
		knockback_v = 0;
	}
	else if (stun > 0) {
		//cout << stun << endl;
		stun -= dt;
		player_movement(knockback_direction.x, knockback_direction.y, knockback_v);
		//cout << knockback_v <<'\t'<<knockback_direction.x<<'\t'<<knockback_direction.y << endl;
		knockback_v -= dt*400;
		if (knockback_v < 0)knockback_v = 0;
		//cout << knockback_v << endl;
	}
	if (cooldown > 0)cooldown -= dt;
	//////////////////////////////////////////////////////
	current_rect = entity_stats.animations[state][current_move].rect;

	entity_sprite.setTextureRect(IntRect(current_rect.left + current_frame * current_rect.width, current_rect.top, current_rect.width, current_rect.height));
	entity_sprite.setOrigin(entity_stats.animations[state][current_move].origin);
	if(!active_action&&stun<=0)
		player_movement(movement.x, movement.y, entity_stats.base_movement_speed * (inBalloon? (1 - z_scale) * 3.f : 1.f));
	if(!inBalloon){
		for (int i = 0; i < 2; i++) {
			short new_tile_x = short(getRelativePos().x / 16.f + i * current_direction.x), new_tile_y = short(getRelativePos().y / 16.f + i * current_direction.y);
			if (new_tile_x >= 0 && new_tile_y >= 0 && new_tile_x < size_x && new_tile_y < size_y && (static_map[new_tile_x][new_tile_y].tile_props & 4096)) {
				interaction_notification();
			}
		}
	}
}

void Player::pollevent()
{
}