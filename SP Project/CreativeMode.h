#pragma once
#include"State.h"
using namespace std;
using namespace sf;

struct CreativeMode
{
private:
	//variables:
	RenderWindow* sidewindow;
	VideoMode videomode {800, 600};
	Vector2i window_pos{ 0,0 };
	Vector2i mouse_pos = { 0, 0 }, current_tile = { 0,0 };
	vector<Texture*>* textures;
	Sprite* Tex;
	Event event;
	State::tex_tile* picked_tile;
	RectangleShape hover_rect, selected_rect, select_rect;
	unsigned int a = 0, b = 0;
	int curr_tex_set = 0;
	float scale = 1.15;
	bool selecting = 0;

	//private functions:
	void change_tex();
	void grid_lines();
	void initial_rectangles();
	void hover_tile();
	void selected();
	void selection();

public:
	//constructors/destructors
	CreativeMode(vector<Texture*>*, State::tex_tile&);
	~CreativeMode();

	//public functions:
	void update();
	void render();
	void pollevent(bool&);
	void run(bool&);
};