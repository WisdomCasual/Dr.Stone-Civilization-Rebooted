#pragma once
#include"State.h"

#include"Global.h"
using namespace globalvar;

struct Background : public State
{
private:
	Sprite bg;
	Texture bg_tex;
	Image bg_img;
	float win_x = 0, win_y = 0; //<-- window dimentions
	float bg_x = 0, bg_y = 0;   //<-- image dimentions
	float scale = 0, average_color_const = 0;
	int points_count = 0;

public:
	Background();
	~Background();
	void scaling();
	void pollevent();
	void update();
	void render();
};

