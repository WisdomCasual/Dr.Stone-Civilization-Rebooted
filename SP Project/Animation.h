#pragma once4
#include"State.h"


#include"Global.h"
using namespace globalvar;

class Animation
{
private:
	Texture animation_tex;
	Sprite animation_sprite;
	IntRect frame;
	Vector2i pos;
	int frame_count, current_frame = 0;
	float &map_x, &map_y;
	float delay = 0, prev_scale = 0;
	bool loop;


public:
	//public variables:
	bool despawn = 0;

	//constructors / destructors
	Animation(IntRect, int, Vector2i, string, Color, bool, float&, float&);
	~Animation();

	//functions
	void update(float);
	void render();
};