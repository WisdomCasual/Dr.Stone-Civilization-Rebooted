#include "Background.h"

Background::Background()
{
	bg_tex.loadFromFile("textures/background/" + to_string(generate_random(0, 3)) + ".png");
	bg.setTexture(bg_tex);
	bg_x = bg.getGlobalBounds().width, bg_y = bg.getGlobalBounds().height;
}

Background::~Background()
{
}

void Background::scaling()
{
}

void Background::pollevent()
{
}

void Background::update()
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
}

void Background::render()
{
	window->draw(bg);
}
