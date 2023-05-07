#include "Enemy.h"


Enemy::~Enemy()
{
}

void Enemy::Mawgood()
{
	if (abs(player_entity.getRelativePos().x - getRelativePos().x)<=RangeElWgood&& abs(player_entity.getRelativePos().y - getRelativePos().y) <= RangeElWgood) {
		
	}
}

void Enemy::updatePos()
{
	entity_sprite.setPosition(pos.x + map_x * scale, pos.y + map_y * scale);
	Mawgood();
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

path_tile* Enemy::aStar(Vector2i target)
{
	path_tile* mp = new path_tile[size_x * size_y];
	bool** vis = new bool * [size_x];
	bool found_path = 0;
	float g_val = 0;
	for (int i = 0; i < size_x; i++) {
		vis[i] = new bool[size_y]({});
	}
	priority_queue<pair<float, pair<int, int>>> pathes;
	pair<float, pair<int, int>> curr_tile;
	pathes.push({ 0, {target.x, target.y} });
	vis[target.x][target.y] = 1;
	mp[target.y*size_x + target.x] = { -1, -1 };
	while (!pathes.empty()) {
		curr_tile = pathes.top();
		pathes.pop();
		curr_tile.first += g_val;
		if (curr_tile.second.first == int(getRelativePos().x / 16) && curr_tile.second.second == int(getRelativePos().y / 16)) {
			found_path = 1;
			break;
		}
		if (curr_tile.first < -35) {
			found_path = 0;
			break;
		}
		int new_x, new_y;
		for (int i = 0; i < 4; i++) {
			new_x = curr_tile.second.first + dx[i],
			new_y = curr_tile.second.second + dy[i];
			float delta_x, delta_y;
			if (new_x >= 0 && new_x < size_x && new_y >= 0 && new_y < size_y) {
				if (!vis[new_x][new_y] && !(static_map[new_x][new_y].tile_props & 2)) {
					if (legal_tile(Vector2f((new_x * 16), (new_y * 16)) - getRelativePos())) {
						vis[new_x][new_y] = 1;
						delta_x = abs(new_x - int(getRelativePos().x / 16)),
							delta_y = abs(new_y - int(getRelativePos().y / 16));
						//g_val = max(delta_x, delta_y) + min(delta_x, delta_y) * 0.4142f;
						g_val = sqrtf(delta_y * delta_y + delta_x * delta_x);
						mp[new_y * size_x + new_x] = { curr_tile.second.first, curr_tile.second.second };
						pathes.push({ curr_tile.first - 1 - g_val, {new_x, new_y} });
					}
				}
			}
		}
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				new_x = curr_tile.second.first + corners[i],
				new_y = curr_tile.second.second + corners[j];
				float delta_x, delta_y;
				if (new_x >= 0 && new_x < size_x && new_y >= 0 && new_y < size_y) {
					if (!vis[new_x][new_y] && !(static_map[new_x][new_y].tile_props & 2) &&
						!(static_map[new_x][curr_tile.second.second].tile_props & 2) && !(static_map[curr_tile.second.first][new_y].tile_props & 2)) {
							if (legal_tile(Vector2f((new_x * 16), (new_y * 16)) - getRelativePos())) {
								vis[new_x][new_y] = 1;
								delta_x = abs(new_x - int(getRelativePos().x / 16)),
									delta_y = abs(new_y - int(getRelativePos().y / 16));
								//g_val = max(delta_x, delta_y) + min(delta_x, delta_y) * 0.4142f;
								g_val = sqrtf(delta_y * delta_y + delta_x * delta_x);
								mp[new_y * size_x + new_x] = { curr_tile.second.first, curr_tile.second.second };
								pathes.push({ curr_tile.first - 1.4142f - g_val, {new_x, new_y} });
							}
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


		target_tile = pathFollow(mp);

		if (target_tile.x == -1.f) {
			will_move = 0;
			entity_stats.state = 0;
			break;
		}
		Vector2f delta_pos = target_tile - getRelativePos();
		theta = atan2f(delta_pos.y, delta_pos.x) * 180 / PI;
		curr_movement = Vector2f(cos(theta * PI / 180), sin(theta * PI / 180));
		will_move = 1;

		if (!checker) {
			last_seen = Vector2i(int(last_seen_cord.x / 16), int(last_seen_cord.y / 16));
			if (mp != nullptr) {
				delete[] mp;
			}
			mp = aStar(last_seen);
			entity_stats.state = 2;
			break;
		}
		last_seen_cord = player_entity.getRelativePos();
		break;
	}
	case 2: {
		target_tile = pathFollow(mp);
		if (target_tile.x == -1.f) {
			will_move = 0;
		}
		else {
			Vector2f delta_pos = target_tile - getRelativePos();
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
		pos /= scale;
		if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
		else scale = win_y / 304.5;
		pos *= scale;
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

		cout << "Moot ya motwa7esh\n";

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
	Mawgood();
}

void Enemy::pollevent()
{
}
