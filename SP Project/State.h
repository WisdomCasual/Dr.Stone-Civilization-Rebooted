#pragma once
#include<bits\stdc++.h>

#include<SFML\System.hpp>
#include<SFML\Window.hpp>
#include<SFML\Graphics.hpp>
#include<SFML\Audio.hpp>

using namespace std;
using namespace sf;

class State
{
private:

public:
	//constructors/destructors:	
	State();
	virtual ~State();

	//public variables:
	vector <Texture*> textures; //<-- current loaded Textures
	Vector2f delta_movement();
	struct tex_tile {
		int x, y, tex_id;
	};
	Font font;
	Text fps_text;
	float delay = 0;
	int fps = 0, frame_count = 0, frame_sum = 0;
	bool fps_active = 0;

	//public functions:
	void intial_textures(string);
	void intial_fps();
	void calc_fps(float);

	virtual void pollevent(Event, RenderWindow*) = 0;
	virtual void update(float) = 0;
	virtual void render(RenderWindow*) = 0;

};