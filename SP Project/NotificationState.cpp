#include "NotificationState.h"
void NotificationState::render_strings()
{
	y = notification_BG.getPosition().y;
	text_y = y + 60 * (scale * .8) / 2.0;
	
	float i = (notification_lines / 2.0 - 2) * -1, dis = (92 / notification_lines) * scale*.9 ;
	text.setFillColor(Color::Black);
	for (int j = 0; j < notification_lines - 3; j++, i++) {
		draw_text(notification_strings[j], text_x, text_y + i * dis, (3 + 75 / notification_lines) * scale);
	}

}
void NotificationState::update_velocity()
{
	float pos_y = notification_BG.getPosition().y;
	if (pos_y>=win_y-60*scale || notification_delay>3)
		velocity.y += g * dt*scale;
	else {
		notification_delay += dt;
		velocity.y = 0;
	}
	
	notification_BG.move(velocity);

	if (pos_y > win_y )
	{
		delete states->at(NotificationID);
		states->erase(NotificationID);
	}

}
NotificationState::NotificationState()
{
	initial_textures("notification");
	notification_BG.setTexture(*textures[0]);
	notification_BG.setOrigin(145,0);
	x = window->getSize().x, y = window->getSize().y;
	notification_BG.setPosition(x, y);
}

NotificationState::~NotificationState()
{
}


void NotificationState::pollevent()
{
	prev(prev(states->end()))->second->pollevent();
}

void NotificationState::update()
{

	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		if (win_x / 350.0 < win_y / 350.0) scale = win_x / 350.0;
		else scale = win_y / 350.0;
		if (win_x > 1280) scale *= 0.75;
		notification_BG.setScale(scale*.8, scale*.8);
		x = win_x-10*scale;
		text_x = x - 145 * (scale*.8) / 2.0;
		text_y = y + 60 * (scale*.8) / 2.0;
		notification_BG.setPosition(x, y);
	}
	update_velocity();
	prev(prev(states->end()))->second->update();
}

void NotificationState::render()
{
	
	window->draw(notification_BG);
	render_strings();
}