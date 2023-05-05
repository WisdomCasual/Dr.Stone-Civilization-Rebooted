#include "Enemy.h"


Enemy::~Enemy()
{
}

void Enemy::updatePos()
{
	entity_sprite.setPosition(pos.x + map_x * scale, pos.y + map_y * scale);
}

casted_bool Enemy::visionLines(Entity& target)
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
		return { 1, 1 };
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
				target_pos[k] = { targ_x, targ_y };
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

casted_bool Enemy::entityFound(Entity& target)
{
	Vector2f afov = (entity_stats.state != 1) ? Vector2f(fov.x * 16 * scale, fov.y * PI / 180) : Vector2f(fov.x * 32 * scale, fov.y * PI / 180);
	float d = magnitude(target.getRelativePos() - getRelativePos()), atheta = theta * PI / 180;
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
	return { 0, 0 };
}

void Enemy::stateMachine()
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

void Enemy::Edrab()
{
	if (current_move == 0) {//U
		if (Lag == 200) {
			MakanElDarb = { getPosition().x - RangeElDarb.x / 2, getPosition().y - RangeElDarb.y,RangeElDarb.x,RangeElDarb.y };
			Lag = 0;
		}
		else Lag += dt;
	}
	if (current_move == 1) {//R
		if (Lag == 200) {
			MakanElDarb = { getPosition().x, getPosition().y - RangeElDarb.y / 2,RangeElDarb.x,RangeElDarb.y };
			Lag = 0;
		}
		else Lag += dt;

	}
	if (current_move == 2) {//L
		if (Lag == 200) {
			MakanElDarb = { getPosition().x - RangeElDarb.x, getPosition().y - RangeElDarb.y / 2,RangeElDarb.x,RangeElDarb.y };
			Lag = 0;
		}
		else Lag += dt;
	}
	if (current_move == 200) {//D
		if (Lag == 3) {
			MakanElDarb = { getPosition().x - RangeElDarb.x / 2, getPosition().y,RangeElDarb.x,RangeElDarb.y };

			Lag = 0;
		}
		else Lag += dt;
	}
}

void Enemy::update()
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
	/////////////////////HitBox Stuff//////////////////////
	Entity_Hitbox = { getRelativePos().x,getRelativePos().y,Size_Hitbox.x,Size_Hitbox.y };
	if (Entity_Hitbox.intersects(MakanElDarb)) {



	}


	current_rect = entity_stats.animations[entity_stats.state][current_move].rect;

	entity_sprite.setTextureRect(IntRect(current_frame * current_rect.left, current_rect.top, current_rect.width, current_rect.height));
	entity_sprite.setOrigin(entity_stats.animations[entity_stats.state][current_move].origin); ///////////////
	updatePos();
	stateMachine();



	if (will_move) {
		bool legal_x = legal_tile({ curr_movement.x, 0 }), legal_y = legal_tile({ 0, curr_movement.y });
		if (legal_x)
			move({ dt * 33 * curr_movement.x * scale, 0 });
		if (legal_y)
			move({ 0, dt * 33 * curr_movement.y * scale });
		if (!legal_x && !legal_y)
			theta += -1 * (rand() % 2) * 30;
		else
			direction({ round(curr_movement.x), round(curr_movement.y) });
	}
}

void Enemy::pollevent()
{
}
