#include "Player.h"

Player::~Player()
{

}

void Player::move_cam(float x_movement, float y_movement)
{
	map_x -= x_movement, map_y -= y_movement;
	x_offset = -map_x / 16, y_offset = -map_y / 16;
}

void Player::player_movement()
{
	if (!active_action) {
		float x_movement = delta_movement().x * movement_speed * dt, y_movement = delta_movement().y * movement_speed * dt;
		Vector2f dir = { 0,0 };
		if (legal_tile({ x_movement, 0 })) {
			dir.x = delta_movement().x;
			if ((entity_sprite.getPosition().x + x_movement * scale >= 150 * scale || delta_movement().x > 0) && (entity_sprite.getPosition().x + x_movement * scale < win_x - 150 * scale || delta_movement().x <= 0))
				move({ x_movement * scale,  0 });
			else if ((-map_x + x_movement >= 0 || delta_movement().x > 0) && (-map_x * scale + win_x <= (size_x - 1) * 16 * scale || delta_movement().x < 0))
				move_cam(x_movement, 0);
			else if (entity_sprite.getPosition().x + x_movement * scale >= 0 && entity_sprite.getPosition().x + x_movement * scale < win_x - 5)
				move({ x_movement * scale,  0 });
		}

		if (legal_tile({ 0, y_movement })) {
			dir.y = delta_movement().y;
			if ((entity_sprite.getPosition().y + y_movement * scale >= 100 * scale || delta_movement().y > 0) && (entity_sprite.getPosition().y + y_movement * scale < win_y - 100 * scale || delta_movement().y <= 0))
				move({ 0, y_movement * scale });

			else if ((-map_y + y_movement >= 0 || delta_movement().y > 0) && (-map_y * scale + win_y <= (size_y - 1) * 16 * scale || delta_movement().y < 0))
				move_cam(0, y_movement);

			else if (entity_sprite.getPosition().y + y_movement * scale >= 0 && entity_sprite.getPosition().y + y_movement * scale < win_y - 5)
				move({ 0, y_movement * scale });
		}

		direction(dir);
	}
}

void Player::Edrab()
{
	if (current_move == 0) { //U
		MakanElDarb = { getPosition().x - RangeElDarb.x / 2, getPosition().y - RangeElDarb.y,RangeElDarb.x,RangeElDarb.y };
	}
	else if (current_move == 1) { //R
		MakanElDarb = { getPosition().x, getPosition().y - RangeElDarb.y / 2,RangeElDarb.x,RangeElDarb.y };
	}
	else if (current_move == 2) { //L
		MakanElDarb = { getPosition().x - RangeElDarb.x, getPosition().y - RangeElDarb.y / 2,RangeElDarb.x,RangeElDarb.y };
	}
	else if (current_move == 3) { //D
		MakanElDarb = { getPosition().x - RangeElDarb.x / 2, getPosition().y,RangeElDarb.x,RangeElDarb.y };
	}
}

void Player::setPosition(float x_pos, float y_pos)
{
	pos = { x_pos, y_pos };
	entity_sprite.setPosition(pos);
}

void Player::use_tool()
{
	if (Lag >= 0.8) {
		if (entity_stats.state == 2) { // sword
			Edrab();
			action(rand() % 2 + 1);
		}
		else if (entity_stats.state == 1 || entity_stats.state == 0) { // axe / pickaxe
			mine();
			action(1);
		}
		Lag = 0;
	}
}

void Player::mine()
{
	Vector2f check_block = { getRelativePos().x + current_direction.x * 16 , getRelativePos().y  + current_direction.y * 16};
	Vector2i destroy_location = {-10,-10};

	if (static_map[(int)(check_block.x / 16)][(int)(check_block.y / 16)].tile_props & 32)
		destroy_location = { (int)(check_block.x / 16),  (int)(check_block.y / 16) };
	else if (current_direction.x) {
		if (static_map[(int)(check_block.x / 16)][(int)((check_block.y + 4) / 16)].tile_props & 32)
			destroy_location = { (int)(check_block.x / 16),  (int)((check_block.y + 4) / 16) };
		else if (static_map[(int)(check_block.x / 16)][(int)((check_block.y - 4) / 16)].tile_props & 32)
			destroy_location = { (int)(check_block.x / 16),  (int)((check_block.y - 4) / 16) };
	}
	else if (current_direction.y) {
		if (static_map[(int)((check_block.x + 4) / 16)][(int)(check_block.y / 16)].tile_props & 32)
			destroy_location = { (int)((check_block.x + 4) / 16),  (int)((check_block.y + 4) / 16) };
		else if (static_map[(int)((check_block.x - 4) / 16)][(int)(check_block.y / 16)].tile_props & 32)
			destroy_location = { (int)((check_block.x - 4) / 16),  (int)((check_block.y + 4) / 16) };
	}
	if (destroy_location.x > -10) {
		destroy_object(destroy_location);
	}
}

void Player::destroy_object(Vector2i tile_location)
{
	static_map[tile_location.x][tile_location.y].tile_props;
	cout << "destroooy\n";

	//vis[layr][x][y] = 1;
	//for (int i = 0; i < 4; i++) {
	//	int new_x = x + dx[i], new_y = y + dy[i];
	//	if (new_x < size_x && new_y < size_y && new_x >= 0 && new_y >= 0 && temp_front[layr][new_x][new_y].x && !vis[layr][new_x][new_y]) {
	//		temp_front[layr][new_x][new_y].x--;
	//		dynamic_map.at[idx].add({ Vector2f(new_x, new_y), temp_front[layr][new_x][new_y] });
	//		destroy_object(new_x, new_y, layr, temp_front, vis, idx);
	//	}
	//}
}

void Player::move(Vector2f movement)
{

	if (!active_action) {
		entity_sprite.move(movement);
		pos += movement;
	}
}

void Player::update()
{
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		entity_sprite.setPosition(entity_sprite.getPosition().x / scale, entity_sprite.getPosition().y / scale);
		win_x = window->getSize().x, win_y = window->getSize().y;
		if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
		else scale = win_y / 304.5;
		entity_sprite.setPosition(entity_sprite.getPosition().x * scale, entity_sprite.getPosition().y * scale);
		////////////////
		setScale(scale * 0.65);
	}

	if (entity_stats.state != prev_state) {
		prev_state = entity_stats.state;
		entity_sprite.setTexture(*textures[entity_stats.state]);
	}

	if (active_action) {
		if (delay > animation_delay) {
			delay = 0;
			current_frame++;
			if (current_frame % entity_stats.animations[entity_stats.state][current_move].frames == 0) {
				current_frame = 0, current_move -= 4 * active_action, active_action = 0;
			}
		}
		else
			delay += dt;
	}
	/////////////////////HitBox Stuff//////////////////////
	Entity_Hitbox = { getRelativePos().x,getRelativePos().y,Size_Hitbox.x,Size_Hitbox.y };
	if (Entity_Hitbox.intersects(MakanElDarb)) {



	}
	if (Lag < 0.8)
		Lag += dt;
	current_rect = entity_stats.animations[entity_stats.state][current_move].rect;

	entity_sprite.setTextureRect(IntRect(current_frame * current_rect.left, current_rect.top, current_rect.width, current_rect.height));
	entity_sprite.setOrigin(entity_stats.animations[entity_stats.state][current_move].origin);
	player_movement();
}

void Player::pollevent()
{
}
