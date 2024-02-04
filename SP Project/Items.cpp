#include "Items.h"

Items::~Items()
{
}

void Items::move()
{
	if (time <= 1.0 && !interact) {
		float x_movement = velocity.x * dt, y_movement = velocity.y * dt;
		if (legal_tile({ x_movement, 0 }, current_hitbox))
			 {
			pos.x += x_movement;
		}
		if (legal_tile({ 0, y_movement }, current_hitbox)) {
			pos.y += y_movement;
		}

		velocity.x -= dt_v_x * dt;
		velocity.y -= dt_v_y * dt;
		time += dt;
	}
	
}

void Items::intersect(){
	if (time > 1.0 || interact) {
		FloatRect item_hitbox = { getRelativePos().x - 8 * entity_stats.scale_const ,getRelativePos().y - 8 * entity_stats.scale_const,16 * entity_stats.scale_const,16 * entity_stats.scale_const };
		if (item_hitbox.intersects(FloatRect(player_entity.getRelativePos().x - (player_entity.current_hitbox.x * player_entity.entity_stats.scale_const) / 2.f, player_entity.getRelativePos().y - (player_entity.current_hitbox.y * player_entity.entity_stats.scale_const) / 2.f, player_entity.current_hitbox.x * player_entity.entity_stats.scale_const, player_entity.current_hitbox.y * player_entity.entity_stats.scale_const)))
		{
			//destroys item
			despawn = 1;
		}
	}
}

void Items::update(float scale, float z_scale)
{
	this->z_scale = z_scale;

	if (game_time - despawn_timer > time_to_despawn && !persistant) {
		despawn = 1;
		return;
	}
	
	if (this->scale != scale * z_scale) {
		this->scale = scale * z_scale;
		//////////////////////////
		entity_sprite.setScale(this->scale, this->scale);
	}

	entity_sprite.setPosition(round(map_x * this->scale) + pos.x * this->scale, round(map_y * this->scale) + pos.y * this->scale);

	
    intersect();
	move();
}

void Items::pollevent()
{
}

