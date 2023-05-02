#include "Entity.h"

Entity::Entity(entity& entity_stats,string entity_name, render_tile**& static_map, float& map_x, float& map_y)
	: entity_stats(entity_stats), map_x(map_x), map_y(map_y), static_map(static_map)
{
	initial_textures("game/entities/" + entity_name);
	entity_sprite.setTexture(*textures[entity_stats.state]); 
}

Entity::~Entity()
{
	for (int i = 0; i < entity_stats.states_no; i++)
		delete[] entity_stats.animations[i];
	delete[] entity_stats.animations;
}

Vector2f Entity::getRelativePos()
{
	return Vector2f(-map_x + getPosition().x / scale, -map_y + getPosition().y / scale);
}

void Entity::updatePos() {
	entity_sprite.setPosition(pos.x  + map_x*scale, pos.y + map_y*scale);
}

void Entity::change_state(int new_state) {
	if (!is_in_action()) {
		entity_stats.state = new_state;
		current_frame = 0, delay = 0;
	}
}

bool Entity::entityFound(Entity& target)
{
	theta = 45;
	Vector2f afov = Vector2f(fov.x * 16 * scale, fov.y * PI / 180);
	float d = magnitude(target.getPosition() - getPosition()), atheta = theta * PI/180;
	if (d <= afov.x) {
		Vector2f A = toCartesian(Vector2f(afov.x, afov.y / 2 + atheta)),
			B = { toCartesian(Vector2f(afov.x, (2 * PI - (afov.y / 2) + atheta))) },
			p = target.getPosition() - getPosition();
		float a1 = 2 * atan2(magnitude(magnitude(p) * A - magnitude(A) * p), magnitude(magnitude(p) * A + magnitude(A) * p)),
			  a2 = 2 * atan2(magnitude(magnitude(p) * B - magnitude(B) * p), magnitude(magnitude(p) * B + magnitude(B) * p));
		if (a1 + a2 <= afov.y)
			return 1;
	}
	return 0;
}

Vector2f Entity::getPosition()
{
	return entity_sprite.getPosition();
}

bool Entity::is_in_action()
{
	return active_action;
}

void Entity::setPosition(float x_pos, float y_pos)
{
	pos = { x_pos, y_pos };
	if(is_player)
		entity_sprite.setPosition(pos);
}

void Entity::setScale(float scale)
{
	sprite_scale = scale;
	entity_sprite.setScale(scale, scale);
}

void Entity::setPlayerState(bool state)
{
	is_player = state;
}

void Entity::move(Vector2f movement)
{
	if (!active_action)
		entity_sprite.move(movement);
}

void Entity::action(int action_id)
{
	if (!active_action) {
		current_frame = 0, delay = 0;
		active_action = action_id;
		current_move += action_id * 4;
	}
}

bool Entity::legal_tile(Vector2f movement)
{
	/////////////////////////// HITBOXESSSSSSSSSSSSSSS (hot russian babes only) //////////////////////////////////////



	current_hitbox = entity_stats.animations[entity_stats.state][current_move].hitbox_rect;

	int x_cords[2] = { -map_x / 16 + (entity_sprite.getPosition().x - (float)current_hitbox.x * sprite_scale / 2 + movement.x * scale) / (16 * scale)
					 , -map_x / 16 + (entity_sprite.getPosition().x + (float)current_hitbox.x * sprite_scale / 2 + movement.x * scale) / (16 * scale) },
	 
		y_cords[2] = { -map_y / 16 + (entity_sprite.getPosition().y - (float)current_hitbox.y * sprite_scale / 2 + movement.y * scale) / (16 * scale)
					 , -map_y / 16 + (entity_sprite.getPosition().y  + (float)current_hitbox.y * sprite_scale / 2 + movement.y * scale) / (16 * scale) };

	Vector2i hitbox_checker;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			hitbox_checker = { x_cords[i], y_cords[j] };
			while (hitbox_checker.x >= x_cords[0] && hitbox_checker.x <= x_cords[1] && hitbox_checker.y >= y_cords[0] && hitbox_checker.y <= y_cords[1]) {
				if (static_map[hitbox_checker.x][hitbox_checker.y].tile_props & 2)
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
		}
		else if (direction.x > 0) {
			//right
			current_move = 1;
		}
		else if (direction.x < 0) {
			//left
			current_move = 2;
		}
		else if (direction.y > 0) {
			//front
			current_move = 3;
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

void Entity::update()
{
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
		else scale = win_y / 304.5;
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

	current_rect = entity_stats.animations[entity_stats.state][current_move].rect;

	entity_sprite.setTextureRect(IntRect(current_frame * current_rect.left, current_rect.top, current_rect.width, current_rect.height));
	entity_sprite.setOrigin(entity_stats.animations[entity_stats.state][current_move].origin); ///////////////
}

void Entity::pollevent()
{
}

void Entity::render()
{
	window->draw(entity_sprite);
}
