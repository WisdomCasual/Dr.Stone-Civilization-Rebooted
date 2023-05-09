#include "Items.h"



Items::~Items()
{
}
void Items::move()
{
	if (time<=2) {
		float x_movement = velocity.x * dt, y_movement = velocity.y * dt;
		if (legal_tile({ x_movement, 0 }) && legal_tile({ 0, y_movement })) {
			entity_sprite.move(velocity);
		}
        else
	    {
		velocity = { 0,0 };
		time = 2;
	    }
		velocity.x -= dt_v_x * dt;
		velocity.y -= dt_v_y * dt;
		time += dt;
	}
	
}

void Items::intersect(){
	FloatRect item_hitbox = { entity_sprite.getPosition().x-8*sprite_scale,entity_sprite.getPosition().y-8*sprite_scale,16*sprite_scale,16* sprite_scale };
	 if (item_hitbox.intersects(FloatRect(player_entity.getPosition().x-(player_entity.current_hitbox.x*player_entity.sprite_scale)/2.0,player_entity.getPosition().y - (player_entity.current_hitbox.y * player_entity.sprite_scale) / 2.0, player_entity.current_hitbox.x * player_entity.sprite_scale, player_entity.current_hitbox.y * player_entity.sprite_scale)))
	 {
      
	 }
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

   //  updatePos();
     intersect();
}

void Items::pollevent()
{
}

