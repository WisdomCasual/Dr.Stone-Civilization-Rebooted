#include "Animation.h"

Animation::Animation(IntRect frame, int frame_count, Vector2i position, string animation_name, float animation_scale, Color texture_color, bool loop, float& map_x, float& map_y)
	: map_x(map_x), map_y(map_y)
{
	this->frame = frame;
	this->frame_count = frame_count;
	this->animation_scale = animation_scale;
	this->loop = loop;
	pos = position;

	animation_tex.loadFromFile("textures/game/effects/" + animation_name + ".png");
	animation_sprite.setTexture(animation_tex);
	animation_sprite.setTextureRect(frame);
	animation_sprite.setColor(texture_color);
	animation_sprite.setOrigin(animation_sprite.getLocalBounds().width / 2.0, animation_sprite.getLocalBounds().height / 2.0);
}

Animation::~Animation()
{

}

void Animation::update(float scale)
{
	if (prev_scale != scale) {
		prev_scale = scale;
		animation_sprite.setScale(scale * animation_scale, scale * animation_scale);
	}
	
	while (delay >= 0.025) {
		delay -= 0.025;
		current_frame++;
		if (current_frame >= frame_count) {
			if (loop)
				current_frame = 0;
			else {
				despawn = 1;
			}

		}
		animation_sprite.setTextureRect(IntRect(frame.left + frame.width * current_frame, frame.top, frame.width, frame.height));
	}
	delay += dt;
}

void Animation::render(Shader* shader)
{
	animation_sprite.setPosition(round(map_x * prev_scale) + pos.x * prev_scale, round(map_y * prev_scale) + pos.y * prev_scale);
	window->draw(animation_sprite, shader);
}
