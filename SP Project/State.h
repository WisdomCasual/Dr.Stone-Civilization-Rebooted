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
	//variables:
	bool back_ground = 0, main_menu = 0, map_builder = 0;
	int bob = 0;
	//constructors/destructors:	
	State();
	virtual ~State();

	//public variables:
	map<string, Texture*> textures; //<-- current loaded Textures
	Vector2f delta_movement();

	//public functions:
	void intial_textures(string);	

	virtual void pollevent(Event, RenderWindow*) = 0;
	virtual void update(float) = 0;
	virtual void render(RenderWindow*) = 0;

};