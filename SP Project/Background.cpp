#include "Background.h"

Background::Background()
{
	bg_tex.loadFromFile("textures/background/" + to_string(generate_random(0, 7)) + ".png");
	bg.setTexture(bg_tex);
	bg_x = bg.getLocalBounds().width, bg_y = bg.getLocalBounds().height;
	bg_img = bg_tex.copyToImage();

	for (int i = bg_img.getSize().x / 6; i < 5 * bg_img.getSize().x / 6; i += bg_img.getSize().x / 16)
		for (int j = bg_img.getSize().y / 6; j < 5 * bg_img.getSize().y / 6; j += bg_img.getSize().y / 16) {
			average_color.x += bg_img.getPixel(i, j).r;
			average_color.y += bg_img.getPixel(i, j).g;
			average_color.z += bg_img.getPixel(i, j).b;
			points_count++;
		}
	average_color /= (float)points_count;
	average_outline_color = average_color;

	average_color_const = 382 - sqrtf( 0.299f * average_color.x * average_color.x + 0.587f * average_color.y * average_color.y + 0.144f * average_color.z * average_color.z);
	average_color_const /= 255.f;

	average_color.x = clamp<int>(average_color.x * average_color_const * average_color_const, 0, 255);
	average_color.y = clamp<int>(average_color.y * average_color_const * average_color_const, 0, 255);
	average_color.z = clamp<int>(average_color.z * average_color_const * average_color_const, 0, 255);


	if(average_color_const > 1)
		average_color_const -= 0.5;

	average_outline_color.x = clamp<int>(average_outline_color.x - 120 * average_color_const, 0, 255);
	average_outline_color.y = clamp<int>(average_outline_color.y - 120 * average_color_const, 0, 255);
	average_outline_color.z = clamp<int>(average_outline_color.z - 120 * average_color_const, 0, 255);
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
