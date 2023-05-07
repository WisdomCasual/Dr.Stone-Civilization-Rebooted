#include "Entity.h"

Entity::Entity(entity& entity_stats, string entity_name, render_tile**& static_map, sheet_properties* tile_props_ptr, float& map_x, float& map_y, int& size_x, int& size_y, float& x_offset, float& y_offset, short& disable_dynamic_obj, Entity* player)
	: entity_stats(entity_stats), map_x(map_x), map_y(map_y), size_x(size_x), size_y(size_y), static_map(static_map), player_entity(*player), x_offset(x_offset), y_offset(y_offset), disable_dynamic_obj(disable_dynamic_obj)
{
	this->size_x = size_x, this->size_y = size_y;

	tile_props = tile_props_ptr;

	win_x = window->getSize().x, win_y = window->getSize().y;
	if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
	else scale = win_y / 304.5;

	initial_textures("game/entities/" + entity_name);
	entity_sprite.setTexture(*textures[entity_stats.state]); 
	srand(time(0));
}

Entity::~Entity()
{
	for (int i = 0; i < entity_stats.states_no; i++)
		delete[] entity_stats.animations[i];
	delete[] entity_stats.animations;
}

Vector2f Entity::getRelativePos()
{
	return (Vector2f(-map_x + getPosition().x / scale, -map_y + getPosition().y / scale));
}

void Entity::change_state(int new_state)
{
	if (!active_action)
		entity_stats.state = new_state;
}

Vector2f Entity::getPosition()
{
	return entity_sprite.getPosition();
}

void Entity::setPosition(float x_pos, float y_pos)
{
	pos = { x_pos, y_pos };
}

void Entity::set_movement_speed(short speed)
{
	movement_speed = speed; //I AM SPEED

}

void Entity::setScale(float scale)
{
	sprite_scale = scale;
	entity_sprite.setScale(scale, scale);
}

void Entity::move(Vector2f movement)
{
	if (!active_action) {
		pos += movement;
	}
}

void Entity::action(int action_id)
{
	if (!active_action) {
		current_frame = 0, delay = 0;
		active_action = action_id;
		current_move += action_id * 4;
	}
}

bool Entity::legal_tile(Vector2f movement, Vector2f curr_hitbox)
{

	current_hitbox = (curr_hitbox.x == -1.f) ? entity_stats.animations[entity_stats.state][current_move].hitbox_rect : curr_hitbox;
	int x_cords[2] = { (int)(getRelativePos().x - (float)current_hitbox.x * sprite_scale / (2 * scale) + movement.x) / 16
					, (int)(getRelativePos().x + (float)current_hitbox.x * sprite_scale / (2 * scale) + movement.x) / 16 },

		y_cords[2] = { (int)(getRelativePos().y - (float)current_hitbox.y * sprite_scale / (2 * scale) + movement.y) / 16
					 , (int)(getRelativePos().y + (float)current_hitbox.y * sprite_scale / (2 * scale) + movement.y) / 16 };
	
	Vector2i hitbox_checker;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			hitbox_checker = { x_cords[i], y_cords[j] };
			while (hitbox_checker.x >= x_cords[0] && hitbox_checker.x <= x_cords[1] && hitbox_checker.y >= y_cords[0] && hitbox_checker.y <= y_cords[1]) {
				if (hitbox_checker.x < 0 || hitbox_checker.y < 0 || hitbox_checker.x >= size_x || hitbox_checker.y >= size_y || static_map[hitbox_checker.x][hitbox_checker.y].tile_props & 2)
					return false;
				hitbox_checker.x += hitbox_check[i][j].x, hitbox_checker.y += hitbox_check[i][j].y;
			}
		}
	}

	return true;
}

void Entity::direction(Vector2f direction)
{
	if (!active_action) {
		if (direction.y < 0) {
			//back
			current_move = 0;
			current_direction = { 0,-1 };
		}
		else if (direction.x > 0) {
			//right
			current_move = 1;
			current_direction = { 1,0 };
		}
		else if (direction.x < 0) {
			//left
			current_move = 2;
			current_direction = { -1,0 };
		}
		else if (direction.y > 0) {
			//front
			current_move = 3;
			current_direction = { 0,1 };
		}
		else {
			current_frame = 0, delay = 0;
			return;
		}

		if (delay > animation_delay) {
			delay = 0;
			current_frame++;
			current_frame %= entity_stats.animations[entity_stats.state][current_move].frames;
		}
		else
			delay += dt;
	}
}


void Entity::pollevent()
{
}

void Entity::render()
{
	window->draw(entity_sprite);
}
