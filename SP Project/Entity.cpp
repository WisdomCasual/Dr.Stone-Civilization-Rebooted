#include "Entity.h"

Entity::Entity(entity& entity_stats,string entity_name)
	: entity_stats(entity_stats)
{
	initial_textures("game/entities/" + entity_name);
	entity_sprite.setTexture(*textures[entity_stats.state]); 
}

Entity::~Entity()
{

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
	entity_sprite.setScale(scale, scale);
}

void Entity::move(Vector2f movement)
{
	entity_sprite.setPosition(entity_sprite.getPosition().x + movement.x, entity_sprite.getPosition().y + movement.y);
}

void Entity::direction(Vector2i direction)
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
