#include "Background.h"

Background::Background()
{
	bg_tex.loadFromFile("textures/background/" + to_string(generate_random(0, 8)) + ".png");
	bg.setTexture(bg_tex);
	bg_x = bg.getLocalBounds().width, bg_y = bg.getLocalBounds().height;
	bg_img = bg_tex.copyToImage();

	for (int i = (int)bg_img.getSize().x / 6; i < 5 * (int)bg_img.getSize().x / 6; i += (int)bg_img.getSize().x / 16)
		for (int j = (int)bg_img.getSize().y / 6; j < 5 * (int)bg_img.getSize().y / 6; j += (int)bg_img.getSize().y / 16) {
			average_color.x += bg_img.getPixel(i, j).r;
			average_color.y += bg_img.getPixel(i, j).g;
			average_color.z += bg_img.getPixel(i, j).b;
			points_count++;
		}
	average_color /= (float)points_count;
	average_outline_color = average_color;

	average_color_const = 382 - sqrtf( 0.299f * average_color.x * average_color.x + 0.587f * average_color.y * average_color.y + 0.144f * average_color.z * average_color.z);
	average_color_const /= 255.f;

	average_color.x = clamp<float>(average_color.x * average_color_const * average_color_const, 0.f, 255.f);
	average_color.y = clamp<float>(average_color.y * average_color_const * average_color_const, 0.f, 255.f);
	average_color.z = clamp<float>(average_color.z * average_color_const * average_color_const, 0.f, 255.f);


	if(average_color_const > 1)
		average_color_const -= 0.5;

	average_outline_color.x = clamp<float>(average_outline_color.x - 120.f * average_color_const, 0.f, 255.f);
	average_outline_color.y = clamp<float>(average_outline_color.y - 120.f * average_color_const, 0.f, 255.f);
	average_outline_color.z = clamp<float>(average_outline_color.z - 120.f * average_color_const, 0.f, 255.f);
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
	win_x = (float)window->getSize().x, win_y = (float)window->getSize().y;
	if (win_x / bg_x > win_y / bg_y) {
		scale = win_x / bg_x;
		bg.setScale(scale, scale);		
		bg.setPosition({ 0.f , -(bg_y * scale - win_y) / 2.f });
	}
	else {
		scale = win_y / bg_y;
		bg.setScale(scale, scale);
		bg.setPosition({ -(bg_x * scale - win_x) / 2.f , 0.f });
	}
}

void Background::render()
{
	window->draw(bg);
}
