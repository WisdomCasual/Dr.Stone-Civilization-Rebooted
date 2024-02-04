#include "HotAirBalloon.h"

void HotAirBalloon::player_collision_check()
{
	FloatRect player_hitbox = FloatRect(player_entity.getRelativePos().x - player_entity.current_hitbox.x / 2, player_entity.getRelativePos().y - player_entity.current_hitbox.y / 2, player_entity.current_hitbox.x, player_entity.current_hitbox.y);

	entity_hitbox = FloatRect(getRelativePos().x - current_hitbox.x / 2, getRelativePos().y - current_hitbox.y / 2, current_hitbox.x, current_hitbox.y);

	player_hitbox.left += player_entity.movement.x * player_entity.entity_stats.base_movement_speed * dt;

	if (entity_hitbox.intersects(player_hitbox) || player_entity.inBalloon)
		player_entity.movement.x = 0;

	player_hitbox.left -= player_entity.movement.x * player_entity.entity_stats.base_movement_speed * dt;
	player_hitbox.top += player_entity.movement.y * player_entity.entity_stats.base_movement_speed * dt;

	if (entity_hitbox.intersects(player_hitbox) || player_entity.inBalloon)
		player_entity.movement.y = 0;

	player_hitbox.top -= player_entity.movement.y * player_entity.entity_stats.base_movement_speed * dt;
	player_hitbox.left += player_entity.knockback_direction.x * player_entity.knockback_v * dt;

	if (entity_hitbox.intersects(player_hitbox))
		player_entity.knockback_direction.x = 0;

	player_hitbox.left -= player_entity.knockback_direction.x * player_entity.knockback_v * dt;
	player_hitbox.top += player_entity.knockback_direction.y * player_entity.knockback_v * dt;

	if (entity_hitbox.intersects(player_hitbox))
		player_entity.knockback_direction.y = 0;
}

void HotAirBalloon::updatePos()
{
	Vector2f player_pos = player_entity.getRelativePos();
	player_pos.x += player_entity.movement.x;
	player_pos.y += player_entity.movement.y * (1 - z_scale) * 3.f * player_entity.entity_stats.base_movement_speed * dt;
	player_pos.x = clamp<float>(player_pos.x, 0.f, size_x * 16.f);
	player_pos.y = clamp<float>(player_pos.y, 0.f, size_y * 16.f);

	if (z_scale < 1.f)
		pos = Vector2f(player_pos.x, player_pos.y + 2);

	entity_sprite.setPosition(round(map_x * (scale * z_scale)) + pos.x * (scale * z_scale),
		round(map_y * (scale * z_scale)) + pos.y * (scale * z_scale) - (1 - z_scale) * 200.f * scale);

	bucket_back.setPosition(round(map_x * (scale * z_scale)) + pos.x * (scale * z_scale),
		round(map_y * (scale * z_scale)) + pos.y * (scale * z_scale) - (1 - z_scale) * 200.f * scale);
}

HotAirBalloon::HotAirBalloon(entity& entity_stats, bool has_legs, render_tile**& static_map, sheet_properties* tile_props_ptr, float& map_x, float& map_y, int& size_x, int& size_y, float& x_offset, float& y_offset, Vector2i& destroy_object_location, Entity* player, bool persistant, double time_to_despawn, int id)
	: Entity(entity_stats, has_legs, static_map, tile_props_ptr, map_x, map_y, size_x, size_y, x_offset, y_offset, destroy_object_location, player, persistant, time_to_despawn, id)
{
	bucket_back.setTexture(*entity_stats.textures[1]);
	bucket_back.setOrigin(entity_stats.animations[state][current_move].origin);
	entity_sprite.setOrigin(entity_stats.animations[state][current_move].origin);
}

void HotAirBalloon::update(float scale, float z_scale)
{
	this->z_scale = z_scale;

	if (this->scale != scale) {
		this->scale = scale;
		////////////////
		entity_sprite.setScale(this->scale * entity_stats.scale_const, this->scale * entity_stats.scale_const);
		bucket_back.setScale(this->scale * entity_stats.scale_const, this->scale * entity_stats.scale_const);
	}

	updatePos();

	if (magnitude(player_entity.getRelativePos() - getRelativePos()) <= 48) {
		if (player_entity.inBalloon) {
			if(z_scale == 1.f)
				player_entity.interaction_notification("Press 'F' to Exit\nPress 'G' to Fly");
			else
				player_entity.interaction_notification("Press 'G' to Land");
		}
		else
			player_entity.interaction_notification("Press 'F' to Ride");
		if (player_entity.interact && z_scale == 1.f) {
			player_entity.interact = 0;

			if (player_entity.inBalloon) {
				player_entity.inBalloon = false;
				player_entity.setPosition(pos.x, pos.y + 20);
			}
			else {
				player_entity.inBalloon = true;
				player_entity.setPosition(pos.x, pos.y);
			}
		}
	}

	if(z_scale == 1.f)
		player_collision_check();
}

void HotAirBalloon::render(Shader* shader)
{
	if (rendered_back)
		window->draw(entity_sprite, shader);
	else
		window->draw(bucket_back, shader);

	rendered_back = !rendered_back;
}