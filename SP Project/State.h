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
	//public functions:
	void intial_textures(string);

	virtual void pollevent(Event, RenderWindow*) = 0;
	virtual void update(float) = 0;
	virtual void render(RenderWindow*) = 0;

};