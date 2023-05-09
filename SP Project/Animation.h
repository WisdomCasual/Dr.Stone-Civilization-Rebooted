#pragma once4
#include"State.h"


#include"Global.h"
using namespace globalvar;

class Animation
{
private:

	Sprite animation_sprite;




public:
	Animation();
	~Animation();


	void update();
	void render();
};