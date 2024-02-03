#include "Entity.h"

Entity::Entity(entity& entity_stats, bool has_legs, render_tile**& static_map, sheet_properties* tile_props_ptr, float& map_x, float& map_y, int& size_x, int& size_y, float& x_offset, float& y_offset, Vector2i& destroy_object_location, Entity* player, bool persistant, double time_to_despawn, int id)
	: entity_stats(entity_stats), map_x(map_x), map_y(map_y), size_x(size_x), size_y(size_y), static_map(static_map), player_entity(*player), x_offset(x_offset), y_offset(y_offset), destroy_object_location(destroy_object_location), time_to_despawn(time_to_despawn), persistant(persistant)
{
	this->size_x = size_x;
	this->size_y = size_y;
	this->id = id;

	despawn_timer = game_time;
	tile_props = tile_props_ptr;

	if (!has_legs) {
		entity_sprite.setTexture(*entity_stats.textures[state]);
		entity_sprite.setTextureRect(IntRect(id * 16, 0, 16, 16));
		entity_sprite.setOrigin(8, 8);
		current_hitbox = { 16, 16 };
		health = 1;
	}
	else {
		if(!entity_stats.is_player)
			current_hitbox = entity_stats.animations[state][current_move].hitbox_rect;
		entity_sprite.setTexture(*entity_stats.textures[state]);
		health = entity_stats.max_health, damage = entity_stats.base_damage;
	}

	if (entity_stats.buffers_count) {
		sounds = new Sound[entity_stats.buffers_count];

		for (int i = 0; i < entity_stats.buffers_count; i++)
			sounds[i].setBuffer(*entity_stats.soundbuffers[i]);

	}
	
	despawn_timer = game_time;
	animation_delay = 1 / entity_stats.base_animation_speed;
}

Entity::~Entity()
{
	if (sounds != nullptr)
		delete[] sounds;
}

Vector2f Entity::getRelativePos()
{
	return pos;
}

void Entity::change_state(int new_state)
{
	if (!active_action)
		state = new_state;
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
	entity_stats.base_movement_speed = speed; //I AM SPEED
}

void Entity::setScale(float new_scale_const)
{
	entity_stats.scale_const = new_scale_const;
	entity_sprite.setScale(new_scale_const * scale, new_scale_const * scale);
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
	current_hitbox = (curr_hitbox.x == -1.f) ? entity_stats.animations[state][current_move].hitbox_rect: curr_hitbox;
	current_hitbox *= entity_stats.scale_const;

	int x_cords[2] = { (int)floor((getRelativePos().x - current_hitbox.x / 2.f + movement.x) / 16.f)
					, (int)floor((getRelativePos().x + current_hitbox.x / 2.f + movement.x) / 16.f) },

		y_cords[2] = { (int)floor((getRelativePos().y - current_hitbox.y / 2.f + movement.y) / 16.f)
					 , (int)floor((getRelativePos().y + current_hitbox.y / 2.f + movement.y) / 16.f) };

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

void Entity::direction(Vector2f direction, bool moving)
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

		if (moving) {
			while (delay > animation_delay) {
				delay -= animation_delay;
				current_frame++;
				current_frame %= entity_stats.animations[state][current_move].frames;
			}
			delay += dt;
		}
		else
			current_frame = 0;
	}
}

void Entity::pollevent()
{
}

void Entity::render(Shader* shader)
{
	//window->draw(test);
	window->draw(entity_sprite, shader);

	//draws entity hitboxes for passive and hostile entities:
	
	//RectangleShape hitbox_debug;
	//hitbox_debug.setSize({ entity_hitbox.width * scale, entity_hitbox.height * scale });
	//hitbox_debug.setPosition({ entity_hitbox.left * scale + map_x * scale, entity_hitbox.top * scale + map_y * scale });
	//hitbox_debug.setFillColor(Color(255,0,0,150));
	//window->draw(hitbox_debug);
}