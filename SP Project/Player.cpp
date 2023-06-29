#include "Player.h"
#include "GameState.h"

Player::~Player()
{

}

void Player::move_cam(float x_movement, float y_movement)
{
	map_x -= x_movement, map_y -= y_movement;
	x_offset = -map_x / 16, y_offset = -map_y / 16;
}

void Player::player_movement(float x_movement,float y_movement,float velocity, bool no_direction_change)
{
	Vector2f v_direction = { x_movement,y_movement };
	x_movement *= velocity * dt, y_movement *=  velocity * dt;
	bool moving = false;
	if (x_movement && legal_tile({ x_movement, 0 })) {
		moving = true;
		if ((entity_sprite.getPosition().x + x_movement * scale >= 150 * scale || v_direction.x > 0) && (entity_sprite.getPosition().x + x_movement * scale < win_x - 150 * scale || v_direction.x <= 0))
			move({ x_movement * scale,  0 });
		else if ((-map_x + x_movement >= 0 || v_direction.x > 0) && (-map_x * scale + win_x <= (size_x - 1) * 16 * scale || v_direction.x < 0))
			move_cam(x_movement, 0);
		else if (entity_sprite.getPosition().x + x_movement * scale >= 0 && entity_sprite.getPosition().x + x_movement * scale < win_x - 5)
			move({ x_movement * scale,  0 });
	}

	if (y_movement && legal_tile({ 0, y_movement })) {
		moving = true;
		if ((entity_sprite.getPosition().y + y_movement * scale >= 100 * scale || v_direction.y > 0) && (entity_sprite.getPosition().y + y_movement * scale < win_y - 100 * scale || v_direction.y <= 0))
			move({ 0, y_movement * scale });

		else if ((-map_y + y_movement >= 0 || v_direction.y > 0) && (-map_y * scale + win_y <= (size_y - 1) * 16 * scale || v_direction.y < 0))
			move_cam(0, y_movement);

		else if (entity_sprite.getPosition().y + y_movement * scale >= 0 && entity_sprite.getPosition().y + y_movement * scale < win_y - 5)
			move({ 0, y_movement * scale });
	}

	if(!no_direction_change)
		direction(v_direction, moving);
}

void Player::knockback(Vector2f direction,float v) {
	knockback_direction = direction;
	knockback_v = v;
}

void Player::damaged(Color& original, float& delay, Sprite& Entity)
{
	original = Entity.getColor();
	Entity.setColor(Color(original.r, 155, 155));
	delay = 0.4;
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
	test.setPosition((hit_range.left + map_x) * scale, (hit_range.top + map_y) * scale);
	*/
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
	if (Lag >= 0.8&&stun<=0) {
		if (state == 2) { // sword
			int hit_animation = rand() % 2;
			Hitbox_align(hit_animation);
			action(hit_animation+1);
		}
		else if (state == 1 || state == 0) { // axe / pickaxe
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
			if (core_location.x == -10) {
				for (int j = 1; j < 4; j++)
					if (static_map[destroy_location.x + dx[i]][destroy_location.y + dy[j]].tile_props & 32) {
						core_location = { destroy_location.x + dx[i], destroy_location.y + dy[j] };
						break;
					}
			}
			else break;

		if (static_map[core_location.x][core_location.y].tool_type == state) {
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

void Player::update()
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
	/////////////////////HitBox Stuff//////////////////////
	if (fake_constructor) {
		fake_constructor = 0;
		og_player_color = entity_sprite.getColor();
	}
	Entity_Hitbox = { getRelativePos().x - current_hitbox.x / 2,getRelativePos().y - current_hitbox.y / 2,current_hitbox.x,current_hitbox.y };
	if (Lag >= 0.4) {
		hit_range = { -10,-10,1,1 };
	}
	if (Lag < 0.8)
		Lag += dt;

	if (stun <= 0) {
		//cout << "here\n";
		//cout << stun << endl;
		entity_sprite.setColor(Color(og_player_color));
		knockback_v = 0;
	}
	else if (stun > 0) {
		//cout << stun << endl;
		stun -= dt;
		player_movement(knockback_direction.x, knockback_direction.y, knockback_v, true);
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
		player_movement(movement.x, movement.y, entity_stats.base_movement_speed);
}

void Player::pollevent()
{
}
