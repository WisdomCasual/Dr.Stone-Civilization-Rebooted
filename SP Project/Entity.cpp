#include "Entity.h"

Entity::Entity(entity& entity_stats, string entity_name, render_tile**& static_map, float& map_x, float& map_y, Entity* player)
	: entity_stats(entity_stats), map_x(map_x), map_y(map_y), static_map(static_map), player_entity(*player)
{
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

void Entity::updatePos()
{
	entity_sprite.setPosition(pos.x  + map_x*scale, pos.y + map_y*scale);
}

void Entity::change_state(int new_state)
{
	if (!is_in_action())
		entity_stats.state = new_state;
}

Entity::casted_bool Entity::visionLines(Entity& target)
{
	float m[5], delta_y = target.getRelativePos().y - getRelativePos().y,
				delta_x = target.getRelativePos().x - getRelativePos().x;
	Vector2f initial_pos[5], target_pos[5];

	short hit = 0;

	bool for_y[5] = {};
	if (abs(delta_y) > abs(delta_x)) {
		swap(delta_y, delta_x);
		for_y[0] = 1;
	}
	if (delta_x == 0)
		return {1, 1};
	if ((delta_x > 0)) {
		initial_pos[0] = getRelativePos();
		target_pos[0] = target.getRelativePos();
	}
	else {
		initial_pos[0] = target.getRelativePos();
		target_pos[0] = getRelativePos();
	}

	m[0] = delta_y / delta_x;


	int k = 1;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			float targ_y = target.getRelativePos().y + corners[j] * ((float)current_hitbox.y * sprite_scale / 4),
				  targ_x = target.getRelativePos().x + corners[i] * ((float)current_hitbox.x * sprite_scale / 6);
			delta_y = targ_y - getRelativePos().y,
			delta_x = targ_x - getRelativePos().x;

			if (abs(delta_y) > abs(delta_x)) {
				swap(delta_y, delta_x);
				for_y[k] = 1;
			}
			if (delta_x == 0)
				return { 1, 1 };
			if ((delta_x > 0)) {
				initial_pos[k] = getRelativePos();
				target_pos[k] = {targ_x, targ_y};
			}
			else {
				initial_pos[k] = { targ_x, targ_y };
				target_pos[k] = getRelativePos();
			}

			m[k] = delta_y / delta_x;;
			k++;
		}
	}

	for (int i = 0; i < 5; i++) {
		if (for_y[i]) {
			for (float j = initial_pos[i].y; j < target_pos[i].y; j += 16) {

				if (static_map[int((m[i] * (j - initial_pos[i].y) + initial_pos[i].x) / 16)][int((j) / 16)].tile_props & 2) {
					hit++;
					break;
				}
			}
		}
		else {
			for (float j = initial_pos[i].x; j < target_pos[i].x; j += 16) {

				if (static_map[int((j) / 16)][int((m[i] * (j - initial_pos[i].x) + initial_pos[i].y) / 16)].tile_props & 2) {
					hit++;
					break;
				}
			}
		}
	}
	return { hit != 5, !hit };
}

Entity::casted_bool Entity::entityFound(Entity& target)
{
	
	Vector2f afov = (entity_stats.state != 1) ? Vector2f(fov.x * 16 * scale, fov.y * PI / 180) : Vector2f(fov.x * 32 * scale, fov.y * PI / 180);
	float d = magnitude(target.getRelativePos() - getRelativePos()), atheta = theta * PI/180;
	if (d <= afov.x) {
		Vector2f A = toCartesian(Vector2f(afov.x, afov.y / 2 + atheta)),
			B = { toCartesian(Vector2f(afov.x, (2 * PI - (afov.y / 2) + atheta))) },
			p = target.getRelativePos() - getRelativePos();
		float a1 = 2 * atan2f(magnitude(magnitude(p) * A - magnitude(A) * p), magnitude(magnitude(p) * A + magnitude(A) * p)),
			  a2 = 2 * atan2f(magnitude(magnitude(p) * B - magnitude(B) * p), magnitude(magnitude(p) * B + magnitude(B) * p));
		if (a1 + a2 <= afov.y) {
			return visionLines(target);
		}
	}
	return {0, 0};
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
	if (!active_action) {
		if(is_player)
			entity_sprite.move(movement);
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

void Entity::stateMachine()
{

	casted_bool checker = entityFound(player_entity);
	if (entity_stats.state != 1 && checker.vision) {
		entity_stats.state = 1;
		motion_delay = 0;
	}
	switch (entity_stats.state) {
	case 1: {
		Vector2f delta_pos = player_entity.getRelativePos() - getRelativePos();
		if (checker.path) {
			use_astar = 0;
			theta = atan2f(delta_pos.y, delta_pos.x) * 180 / PI;
			curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
			will_move = 1;
		}
		else {
			use_astar = 1;
		}
		if (!entityFound(player_entity).vision) {
			if (use_astar) {

			}
			else {
				delta_pos = last_seen - getRelativePos();
				theta = atan2f(delta_pos.y, delta_pos.x) * 180 / PI;
				curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
			}
				last_seen = Vector2f((int(last_seen.x / 16 / scale) * 16 + 8) * scale, (int(last_seen.y / 16 / scale) * 16 + 8) * scale);
				last_sign = getRelativePos() - last_seen;
				entity_stats.state = 2;
			break;
		}
		last_seen = player_entity.getRelativePos();
		break;
	}
	case 2:
		if ((getRelativePos().x - last_seen.x < 0 != last_sign.x < 0) && (getRelativePos().y - last_seen.y < 0 != last_sign.y < 0)) {
			will_move = 0;
		}
		if (!will_move) {
			if (motion_delay >= 4) {
				motion_delay = 3.5;
				entity_stats.state = 0;
			}
			motion_delay += dt;
		}
		break;
	default:
		motion_delay += dt;

		if (motion_delay >= move_for && will_move) {
			will_move = 0;
			direction({ 0, 0 });
		}
		if (motion_delay >= 4) {
			motion_delay = 0;
			theta = (rand() % 36) * 10;
			will_move = rand() % 4;
			move_for = 3 + rand() % 2;
			direction({ 0, 0 });
			curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
		}
		break;
	}
}

void Entity::Edrab()
{
	if (current_move == 0) {//U
		if (Lag == 3) {
			MakanElDarb = { getPosition().x - RangeElDarb.x / 2, getPosition().y - RangeElDarb.y,RangeElDarb.x,RangeElDarb.y };

			Lag = 0;
		}
		else Lag += dt;
	}
	if (current_move == 1) {//R
		if (Lag == 3) {
			MakanElDarb = { getPosition().x, getPosition().y - RangeElDarb.y / 2,RangeElDarb.x,RangeElDarb.y };
			Lag = 0;
		}
		else Lag += dt;
		
	}
	if (current_move == 2) {//L
		if (Lag == 3) {
			MakanElDarb = { getPosition().x - RangeElDarb.x, getPosition().y - RangeElDarb.y / 2,RangeElDarb.x,RangeElDarb.y };
			Lag = 0;
		}
		else Lag += dt;
	}
	if (current_move == 3) {//D
		if (Lag == 3) {
			MakanElDarb = { getPosition().x - RangeElDarb.x / 2, getPosition().y,RangeElDarb.x,RangeElDarb.y };

			Lag = 0;
		}
		else Lag += dt;
	}
}

void Entity::update()
{
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		entity_sprite.setPosition(entity_sprite.getPosition().x / scale, entity_sprite.getPosition().y / scale);
		win_x = window->getSize().x, win_y = window->getSize().y;
		if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
		else scale = win_y / 304.5;
		entity_sprite.setPosition(entity_sprite.getPosition().x * scale, entity_sprite.getPosition().y * scale);
		////////////////

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


	if (!is_player)
		stateMachine();

	if (will_move) {
		bool legal_x = legal_tile({ curr_movement.x, 0 }), legal_y = legal_tile({ 0, curr_movement.y });
		if (legal_x)
			move({ dt * 33 * curr_movement.x * scale, 0});
		if (legal_y)
			move({ 0, dt * 33 * curr_movement.y * scale });
		if (!legal_x && !legal_y)
			theta += -1 * (rand() % 2) * 30;
		else
			direction({ round(curr_movement.x), round(curr_movement.y) });
	}
}


void Entity::pollevent()
{
}

void Entity::render()
{
	window->draw(entity_sprite);
}
