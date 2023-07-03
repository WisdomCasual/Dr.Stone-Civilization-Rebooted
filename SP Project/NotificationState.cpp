#include "NotificationState.h"
void NotificationState::render_strings()
{
	y_pos = notification_BG.getPosition().y;
	text_y = y_pos + 60 * (scale * .8) / 2.0;
	
	float i = (notification_lines / 2.0 - 2) * -1, dis = (92 / notification_lines) * scale*.9 ;
	text.setFillColor(Color::Black);
	for (int j = 0; j < notification_lines - 3; j++, i++) {
		draw_text(notification_strings[j], text_x, text_y + i * dis, (3 + 55 / notification_lines) * scale);
	}

}
void NotificationState::update_velocity()
{
	float pos_y = notification_BG.getPosition().y;
	//cout << notification_delay << endl;
	if (notification_delay && pos_y > win_y - 60 * scale) {
		//cout << "here" << endl;
		pos_y -= velocity * dt * pos_scale;
		if (pos_y < win_y - 60 * scale)
			pos_y = win_y - 60 * scale;
		notification_BG.setPosition(notification_BG.getPosition().x, pos_y);
	}
	else if(notification_delay > 0){
		notification_delay -= dt;
		if (notification_delay < 0)
			notification_delay = 0;
	}
	else {
		pos_y += velocity * dt * pos_scale;
		notification_BG.setPosition(notification_BG.getPosition().x, pos_y);
	}
	
	if (pos_y > win_y)
	{
		delete states->at(NotificationID);
		states->erase(NotificationID);
		return;
	}

}
NotificationState::NotificationState()
{
	scale = min(window->getSize().x / 350.0, window->getSize().y / 350.0);
	pos_scale = scale;
	if (window->getSize().x > 1280) scale *= 0.75;
	initial_textures("notification");
	notification_BG.setTexture(*textures[0]);
	notification_BG.setOrigin(145,0);
	x_pos = window->getSize().x, y_pos = window->getSize().y;
	notification_BG.setPosition(x_pos, y_pos);
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
		y_pos /= pos_scale;
		win_x = window->getSize().x, win_y = window->getSize().y;
		if (win_x / 350.0 < win_y / 350.0) scale = win_x / 350.0;
		else scale = win_y / 350.0;
		pos_scale = scale;
		if (win_x > 1280) scale *= 0.75;
		notification_BG.setScale(scale*.8, scale*.8);
		x_pos = win_x - 10 * pos_scale;
		y_pos *= pos_scale;
		text_x = x_pos - 145 * (scale*.8) / 2.0;
		text_y = y_pos + 60 * (scale*.8) / 2.0;
		notification_BG.setPosition(x_pos, y_pos);
	}
	if (prev(states->end())->first == NotificationID) {
		prev(prev(states->end()))->second->update();
		update_velocity();
	}
	//don't put anything after//
}

void NotificationState::render()
{
	window->draw(notification_BG);
	render_strings();
}