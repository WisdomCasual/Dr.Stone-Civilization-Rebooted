#pragma once
#include"State.h"
using namespace std;
using namespace sf;

class CreativeMode
{
private:
	//variables:
	RenderWindow* sidewindow;
	VideoMode videomode {800, 600};
	Vector2i window_pos{ 0,0 };
	Vector2i mouse_pos = { 0, 0 }, current_tile = { 0,0 };
	vector<Texture*>* textures;
	vector<Texture*>::iterator it;
	Sprite* Tex;
	Event event;
	Vector2i* picked_tile;
	unsigned int a = 0, b = 0;

	//private functions:
	void change_tex();


public:
	//constructors/destructors
	CreativeMode(vector<Texture*>*, Vector2i&);
	~CreativeMode();

	//public functions:
	void update();
	void render();
	void pollevent(bool&);
	void run(bool&);
};