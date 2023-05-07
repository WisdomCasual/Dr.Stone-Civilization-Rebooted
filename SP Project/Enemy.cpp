#include "Enemy.h"


Enemy::~Enemy()
{
}

void Enemy::updatePos()
{
	entity_sprite.setPosition((pos.x + map_x) * scale, (pos.y + map_y) * scale);
}

bool Enemy::visionLines(Entity& target)
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
		return 1;
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
			float targ_y = target.getRelativePos().y + corners[j] * ((float)target.current_hitbox.y * target.sprite_scale / (2 * scale)),
				targ_x = target.getRelativePos().x + corners[i] * ((float)target.current_hitbox.x * target.sprite_scale / (2 * scale));
			delta_y = targ_y - getRelativePos().y,
				delta_x = targ_x - getRelativePos().x;

			if (abs(delta_y) > abs(delta_x)) {
				swap(delta_y, delta_x);
				for_y[k] = 1;
			}
			if (delta_x == 0)
				return 1;
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

				if (static_map[int((m[i] * (j - initial_pos[i].y) + initial_pos[i].x) / 16)][int((j) / 16)].tile_props & 64) {
					hit++;
					break;
				}
			}
		}
		else {
			for (float j = initial_pos[i].x; j < target_pos[i].x; j += 16) {

				if (static_map[int((j) / 16)][int((m[i] * (j - initial_pos[i].x) + initial_pos[i].y) / 16)].tile_props & 64) {
					hit++;
					break;
				}
			}
		}
	}
	return hit != 5;
}

bool Enemy::entityFound(Entity& target)
{
	Vector2f afov = (entity_stats.state != 1) ? Vector2f(fov.x * 16, fov.y * PI / 180) : Vector2f(fov.x * 32, fov.y * PI / 180);
	float d = magnitude(target.getRelativePos() - getRelativePos()), atheta = theta * PI / 180;
	if (d <= sound_range * 16) {
		return 1;
	}
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
	return 0;
}

bool Enemy::legal_direction(Vector2f tile_pos, short dx, short dy)
{
	short move_num = 0;
		if (dy < 0) {
			move_num = 0;
		}
		else if (dx > 0) {
			move_num = 1;
		}
		else if (dx < 0) {
			move_num = 2;
		}
		else if (dy > 0) {
			move_num = 3;
		}
		return legal_tile(tile_pos, entity_stats.animations[entity_stats.state][move_num].hitbox_rect);
}

path_tile* Enemy::aStar(Vector2i target)
{
	path_tile* mp = new path_tile[size_x * size_y];
	bool** vis = new bool * [size_x];
	bool found_path = 0;
	float g_val = 0;
	Vector2i mntile = { -1, -1 }, to_pos = {-1, -1};
	bool is_legal = 0;
	for (int i = 0; i < 4 && !is_legal; i++) {
		is_legal = legal_direction(Vector2f((target.x * 16 + 8), (target.y * 16 + 8)) - getRelativePos(), dx[i], dy[i]);
	}
	if (!is_legal) {
		float delta_x, delta_y, mn = 1e9, g_temp;
		int new_x, new_y;
		short hitbox_count = 0;
		Vector2i found_box = { -1, -1 }, found_empty = { -1, -1 };
		for (int i = 0; i < 4; i++) {
			new_x = target.x + dx[i], new_y = target.y + dy[i];
			if ((static_map[new_x][new_y].tile_props & 2))
				hitbox_count++, found_box = { dx[i], dy[i] };
			else
				found_empty = { dx[i], dy[i] };
		}
		switch (hitbox_count) {
		case 2:
			if ((static_map[target.x - found_box.x][target.y - found_box.y].tile_props & 2)) {
				new_x = target.x - found_box.y, new_y = target.y - found_box.x;
				delta_x = abs(new_x - to_pos.x),
					delta_y = abs(new_y - to_pos.y);
				mn = sqrtf(delta_y * delta_y + delta_x * delta_x);
				mntile = { new_x, new_y };
				new_x = target.x + found_box.y, new_y = target.y + found_box.x;
				delta_x = abs(new_x - to_pos.x),
					delta_y = abs(new_y - to_pos.y);
				g_temp = sqrtf(delta_y * delta_y + delta_x * delta_x);
				if (g_temp < mn)
					mntile = { new_x, new_y };
			}

			else {
				if ((static_map[target.x + found_box.y][target.y + found_box.x].tile_props & 2)) {
					mntile = { target.x - found_box.y - found_box.x, target.y - found_box.y - found_box.x };
					break;
				}
				mntile = { target.x + found_box.y - found_box.x, target.y - found_box.y + found_box.x };
			}

			break;
		case 3:
			mntile = found_empty;
			break;
		default:
			mntile = target - found_box;
			break;
		}
	}
	for (int i = 0; i < size_x; i++) {
		vis[i] = new bool[size_y]({});
	}
	tabor_el_3e4 pathes;
	comparison_tile curr_tile;
	to_pos = Vector2i(int(getRelativePos().x / 16), int(getRelativePos().y / 16));
	vis[target.x][target.y] = 1;
	mp[target.y*size_x + target.x] = { -1, -1 };
	if (mntile.x == -1.f) {
		pathes.Ed5ol({ 0, target.x, target.y });
	}
	else {
		mp[mntile.y * size_x + mntile.x] = { target.x, target.y};
		vis[mntile.x][mntile.y] = 1;
		pathes.Ed5ol({ 0, mntile.x, mntile.y });
	}
	while (!pathes.Fare8()) {
		curr_tile = pathes.top();
		pathes.Astika();
		curr_tile.cost -= g_val;
		if (curr_tile.x == to_pos.x && curr_tile.y == to_pos.y) {
			found_path = 1;
			break;
		}
		if (curr_tile.cost > 35) {
			found_path = 0;
			break;
		}
		int new_x, new_y;
		for (int i = 0; i < 4; i++) {
			new_x = curr_tile.x + dx[i],
			new_y = curr_tile.y + dy[i];
			float delta_x, delta_y;
			if (new_x >= 0 && new_x < size_x && new_y >= 0 && new_y < size_y) {
				if (!vis[new_x][new_y] && (legal_direction(Vector2f((new_x * 16 + 8), (new_y * 16 + 8)) - getRelativePos(), -dx[i], -dy[i]) ||
					new_x == to_pos.x && new_y == to_pos.y)) {
						vis[new_x][new_y] = 1;
						delta_x = abs(new_x - to_pos.x),
							delta_y = abs(new_y - to_pos.y);
						//g_val = max(delta_x, delta_y) + min(delta_x, delta_y) * 0.4142f;
						g_val = sqrtf(delta_y * delta_y + delta_x * delta_x);
						mp[new_y * size_x + new_x] = { curr_tile.x, curr_tile.y };
						pathes.Ed5ol({ curr_tile.cost + 1 + g_val, new_x, new_y });
				}
			}
		}
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				new_x = curr_tile.x + corners[i],
				new_y = curr_tile.y + corners[j];
				float delta_x, delta_y;
				if (new_x >= 0 && new_x < size_x && new_y >= 0 && new_y < size_y) {
					if (!vis[new_x][new_y] && (legal_direction(Vector2f((new_x * 16 + 8), (new_y * 16 + 8)) - getRelativePos(), -corners[i], -corners[j]) ||
						new_x == to_pos.x && new_y == to_pos.y) &&
						legal_direction(Vector2f((new_x * 16 + 8), (curr_tile.y * 16 + 8)) - getRelativePos(), -corners[i], -corners[j]) &&
						legal_direction(Vector2f((curr_tile.x * 16 + 8), (new_y * 16 + 8)) - getRelativePos(), -corners[i], -corners[j])) {
							vis[new_x][new_y] = 1;
							delta_x = abs(new_x - to_pos.x),
							delta_y = abs(new_y - to_pos.y);
							//g_val = max(delta_x, delta_y) + min(delta_x, delta_y) * 0.4142f;
							g_val = sqrtf(delta_y * delta_y + delta_x * delta_x);
							mp[new_y * size_x + new_x] = { curr_tile.x, curr_tile.y};
							pathes.Ed5ol({ curr_tile.cost + 1.4142f + g_val, new_x, new_y });
					}
				}
			}
		}
		
	}
	for (int i = 0; i < size_x; i++) {
		delete[] vis[i];
	}
	delete[] vis;
	if (found_path) {
		return mp;
	}
	return nullptr;
}

void Enemy::pathFinding(Entity& target, path_tile*& mp)
{
	if (mp != nullptr) {
		delete[] mp;
	}
	mp = aStar({ int(target.getRelativePos().x / 16), int(target.getRelativePos().y / 16) });
}

Vector2f Enemy::pathFollow(path_tile*& mp)
{
	if (mp == nullptr) {
		return { -1.f, -1.f };
	}
	path_tile temp = mp[size_x * int(getRelativePos().y / 16) + int(getRelativePos().x / 16)];
	if (temp.x == -1) {
		return { -1.f, -1.f };
	}
	else {
		return(Vector2f(temp.x * 16 + 8, temp.y * 16 + 8));
	}
}

void Enemy::stateMachine()
{
	bool checker = entityFound(player_entity);
	if (entity_stats.state != 1 && checker) {
		Vector2i enemy_tile = { int(player_entity.getRelativePos().x / 16), int(player_entity.getRelativePos().y / 16) };
		if (checker != prev_check || enemy_tile != prev_target_tile) {
			pathFinding(player_entity, mp);
			target_tile = pathFollow(mp);
			delta_sign = target_tile - getRelativePos();
			if (target_tile.x != -1.f) {
				entity_stats.state = 1;
				motion_delay = 2;
			}
		}
		prev_target_tile = enemy_tile;
	}
	prev_check = checker;
	switch (entity_stats.state) {
	case 1: {
		Vector2i enemy_tile = { int(player_entity.getRelativePos().x / 16), int(player_entity.getRelativePos().y / 16) };
		if (enemy_tile != prev_target_tile) {
			pathFinding(player_entity, mp);
			target_tile = pathFollow(mp);
		}
		prev_target_tile = enemy_tile;


		Vector2f delta_pos = target_tile - getRelativePos();
		Vector2f compar = { roundf(delta_pos.x), roundf(delta_pos.y) };
		if ((compar.x == 0 || (delta_pos.x < 0) != (delta_sign.x < 0)) && (compar.y == 0 || (delta_pos.y < 0) != (delta_sign.y < 0))) {
			target_tile = pathFollow(mp);
			delta_sign = target_tile - getRelativePos();
			delta_pos = delta_sign;
		}

		if (target_tile.x == -1.f) {
			will_move = 0;
			entity_stats.state = 0;
			break;
		}
		theta = atan2f(delta_pos.y, delta_pos.x) * 180 / PI;
		curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
		will_move = 1;

		if (!checker) {
			last_seen = Vector2i(int(last_seen_cord.x / 16), int(last_seen_cord.y / 16));
			if (mp != nullptr) {
				delete[] mp;
			}
			mp = aStar(last_seen);
			target_tile = pathFollow(mp);
			delta_sign = target_tile - getRelativePos();
			entity_stats.state = 2;
			break;
		}
		last_seen_cord = player_entity.getRelativePos();
		break;
	}
	case 2: {
		Vector2f delta_pos = target_tile - getRelativePos();
		Vector2f compar = { roundf(delta_pos.x), roundf(delta_pos.y) };
		if ((compar.x == 0 || (delta_pos.x < 0) != (delta_sign.x < 0)) && (compar.y == 0 || (delta_pos.y < 0) != (delta_sign.y < 0))) {
			target_tile = pathFollow(mp);
			delta_sign = target_tile - getRelativePos();
			delta_pos = delta_sign;
		}
		if (target_tile.x == -1.f) {
			will_move = 0;
		}
		else {
			theta = atan2f(delta_pos.y, delta_pos.x) * 180 / PI;
			curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
		}

		if (!will_move) {
			if (motion_delay >= 4) {
				motion_delay = 3.5;
				entity_stats.state = 0;
			}
			motion_delay += dt;
		}
		break;
	}
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
		win_x = window->getSize().x, win_y = window->getSize().y;
		if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
		else scale = win_y / 304.5;
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
			move({ dt * 33 * curr_movement.x, 0 });
		if (legal_y)
			move({ 0, dt * 33 * curr_movement.y});
		if (!legal_x && !legal_y)
			theta += -1 * (rand() % 2) * 30;
		else
			direction({ round(curr_movement.x), round(curr_movement.y) });
	}
}

void Enemy::pollevent()
{
}
