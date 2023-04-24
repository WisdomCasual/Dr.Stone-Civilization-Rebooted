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

Vector2f Entity::getPosition()
{
	return entity_sprite.getPosition();
}

void Entity::setPosition(int x_pos, int y_pos)
{
	entity_sprite.setPosition(x_pos, y_pos);
}

void Entity::setScale(float scale)
{
	sprite_scale = scale;
	entity_sprite.setScale(scale, scale);
}

void Entity::move(Vector2f movement)
{
	entity_sprite.move(movement);
}

bool Entity::legal_tile(Vector2f movement)
{
	/////////////////////////// HITBOXESSSSSSSSSSSSSSS (hot russian babes only) //////////////////////////////////////



	current_hitbox = entity_stats.animations[entity_stats.state][current_move].hitbox_rect;

	int x_cords[2] = { -map_x / 16 + (entity_sprite.getPosition().x - (float)current_hitbox.width * sprite_scale / 2 + movement.x * scale) / (16 * scale)
					 , -map_x / 16 + (entity_sprite.getPosition().x + (float)current_hitbox.width * sprite_scale / 2 + movement.x * scale) / (16 * scale) },
	 
		y_cords[2] = { -map_y / 16 + (entity_sprite.getPosition().y - (float)current_hitbox.height * sprite_scale / 2 + movement.y * scale) / (16 * scale)
					 , -map_y / 16 + (entity_sprite.getPosition().y  + (float)current_hitbox.height * sprite_scale / 2 + movement.y * scale) / (16 * scale) };

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

	current_rect = entity_stats.animations[entity_stats.state][current_move].rect;

	entity_sprite.setTextureRect(IntRect(current_frame * current_rect.left, current_rect.top, current_rect.width, current_rect.height));
	entity_sprite.setOrigin(current_rect.width/2, current_rect.height*3/4); ///////////////
}

void Entity::pollevent()
{
}

void Entity::render()
{
	window->draw(entity_sprite);
}
