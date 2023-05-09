#include "Items.h"



Items::~Items()
{
}
void Items::move()
{
	if (velocity.x>=0 && velocity.y>=0) {
		float x_movement = velocity.x * dt, y_movement = velocity.y * dt;
		if (legal_tile({ x_movement, 0 }) && legal_tile({ 0, y_movement })) {
			entity_sprite.move(velocity);
		}
        else
	    {
		velocity = { 0,0 };
	    }
	}
	
}

bool Items::intersect()
{
	return (entity_sprite.getGlobalBounds().intersects(player_entity.entity_sprite.getGlobalBounds()));
}

void Items::update()
{
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		pos /= scale;
		if (win_x / 540.0 < win_y / 304.5) scale = win_x / 540.0;
		else scale = win_y / 304.5;
		pos *= scale;
		////////////////

	}

	/////////////////////HitBox Stuff//////////////////////
	Entity_Hitbox = { getRelativePos().x,getRelativePos().y,Size_Hitbox.x,Size_Hitbox.y };
	if (Entity_Hitbox.intersects(MakanElDarb)) {



	}


	current_rect = entity_stats.animations[entity_stats.state][current_move].rect;

	entity_sprite.setTextureRect(IntRect(current_frame * current_rect.left, current_rect.top, current_rect.width, current_rect.height));
	entity_sprite.setOrigin(entity_stats.animations[entity_stats.state][current_move].origin); ///////////////
	updatePos();

	entity_sprite.setTexture(*tile_sheets[3]);
	entity_sprite.setTextureRect(IntRect(13*16, 5 * 16, 16, 16));

}

void Items::pollevent()
{
}

