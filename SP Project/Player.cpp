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

void Player::Edrab(int Shakl)
{
	if (current_move == 0) { //U
		if (!Shakl)
			MakanElDarb = { getRelativePos().x - 35, getRelativePos().y - 33,52,20 };
		else
			MakanElDarb = { getRelativePos().x - 28, getRelativePos().y - 33,56,20 };
	}
	else if (current_move == 1) { //R
		//MakanElDarb = { getRelativePos().x, getRelativePos().y - RangeElDarb.y / 2,RangeElDarb.x,RangeElDarb.y };
	}
	else if (current_move == 2) { //L
		//MakanElDarb = { getRelativePos().x - RangeElDarb.x, getRelativePos().y - RangeElDarb.y / 2,RangeElDarb.x,RangeElDarb.y };
	}
	else if (current_move == 3) { //D
		//MakanElDarb = { getRelativePos().x - RangeElDarb.x / 2, getRelativePos().y,RangeElDarb.x,RangeElDarb.y };
	}
	hashofak.setFillColor(Color::Magenta);
	hashofak.setSize({MakanElDarb.width*scale,MakanElDarb.height*scale});
	hashofak.setPosition((MakanElDarb.left + map_x) * scale, (MakanElDarb.top + map_y) * scale);
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
			int ElShakl = 0;//rand() % 2;
			Edrab(ElShakl);
			action(ElShakl+1);
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
		if(static_map[destroy_location.x][destroy_location.y].tool_type == entity_stats.state)
			destroy_object(destroy_location);
	}
}

void Player::destroy_object(Vector2i tile_location)
{
	for (int i = 0; i < 3; i++)
		for (int j = 1; j < 4; j++) {
			Vector2i destroy_area{ tile_location.x + dx[i], tile_location.y + dy[j] };

			if (static_map[destroy_area.x][destroy_area.y].tile_props & 16) {
				disable_dynamic_obj = static_map[destroy_area.x][destroy_area.y].dynamic_idx;
			}
			else if (static_map[destroy_area.x][destroy_area.y].tile_props & 32) {
				if(!dx[i] && !dy[j])
					bigbang(destroy_area, 1);
			}
			else if (static_map[destroy_area.x][destroy_area.y].tile_props & 1) {
				bigbang(destroy_area, 1);
			}
		}
}

void Player::bigbang(Vector2i destroy_tile, bool destroy = 0)
{
	short last = static_map[destroy_tile.x][destroy_tile.y].size - 1;
	Vector3i last_tile = static_map[destroy_tile.x][destroy_tile.y].layers[last];
	static_map[destroy_tile.x][destroy_tile.y].disable_top = destroy;
	static_map[destroy_tile.x][destroy_tile.y].tile_props ^= tile_props[last_tile.z].properties[last_tile.x][last_tile.y].props;
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
		win_x = window->getSize().x, win_y = window->getSize().y;
		if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
		else scale = win_y / 304.5;
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
	MakanElDarb = { -10,-10,1,1 };
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
