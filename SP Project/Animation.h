#pragma once4
#include"State.h"


#include"Global.h"
using namespace globalvar;

class Animation
{
private:
	Sprite animation_sprite;
	IntRect frame;
	int frame_count, current_frame = 0;
	float &map_x, &map_y;
	float delay, prev_scale = 0;
	bool loop, despawn = 0;


public:
	Animation(IntRect, int, Color, bool, float&, float&);
	~Animation();


	void update(float);
	void render();
};