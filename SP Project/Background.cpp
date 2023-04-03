#include "Background.h"

Background::Background()
{
	initial_textures("background");
	bg.setTexture(*textures[0]);
	bg_x = bg.getGlobalBounds().width, bg_y = bg.getGlobalBounds().height;
}

Background::~Background()
{
}

void Background::pollevent(Event, RenderWindow*, int* terminator, deque<State*>* states)
{
}

void Background::update(float dt, RenderWindow* window, int* terminator, deque<State*>* states)
{
}

void Background::render(RenderWindow* window)
{
	win_x = window->getSize().x, win_y = window->getSize().y;
	if (win_x / bg_x > win_y / bg_y) {
		scale = win_x / bg_x;
		bg.setScale(scale, scale);		
		bg.setPosition({ 0 , -(bg_y * scale - win_y) / 2 });
	}
	else {
		scale = win_y / bg_y;
		bg.setScale(scale, scale);
		bg.setPosition({ -(bg_x * scale - win_x) / 2 , 0 });
	}
	window->draw(bg);
}
