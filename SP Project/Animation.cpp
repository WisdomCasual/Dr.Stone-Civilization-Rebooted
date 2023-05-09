#include "Animation.h"

Animation::Animation(IntRect frame, int frame_count, Color texture_color, bool loop, float& map_x, float& map_y)
	: map_x(map_x), map_y(map_y)
{
	this->frame = frame;
	this->frame_count = frame_count;
	this->loop = loop;

	animation_sprite.setTextureRect(frame);
	animation_sprite.setOrigin(8, 8);

}

Animation::~Animation()
{

}

void Animation::update(float scale)
{
	if (prev_scale != scale) {
		prev_scale = scale;
		animation_sprite.setScale(scale, scale);
	}

	if (delay >= 200) {
		delay = 0;
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
	else
		delay += dt;
}

void Animation::render()
{
	window->draw(animation_sprite);
}
