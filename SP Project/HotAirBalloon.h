#pragma once
#include "Entity.h"
class HotAirBalloon :
    public Entity
{
	using::Entity::Entity;
private:
	void player_collision_check();
	void updatePos();
	bool rendered_back = false;
public:
	Sprite bucket_back;
	HotAirBalloon(entity&, bool, render_tile**&, sheet_properties*, float&, float&, int&, int&, float&, float&, Vector2i&, Entity* player = nullptr, bool persistant = 0, double time_to_despawn = 30.0, int id = 0);
	void update(float scale, float z_scale);
	void render(Shader*);
};

