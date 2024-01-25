#include "NotificationState.h"
void NotificationState::render_strings()
{
	y_pos = notification_BG.getPosition().y;
	text_y = y_pos + 60 * (scale * 0.8f) / 2.f;
	
	float i = (notification_lines / 2.f - 2) * -1, dis = (92 / notification_lines) * scale * 0.9f ;
	text.setFillColor(Color::Black);
	for (int j = 0; j < notification_lines - 3; j++, i++) {
		draw_text(notification_strings[j], text_x, text_y + i * dis, (3 + 55 / notification_lines) * scale);
	}

}

void NotificationState::update_velocity()
{
	float pos_y = notification_BG.getPosition().y;
	if (notification_delay && pos_y > win_y - 60 * scale) {
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
	scale = min(window->getSize().x / 350.f, window->getSize().y / 350.f);
	pos_scale = scale;
	if (window->getSize().x > 1280) scale *= 0.75f;
	initial_textures("notification");
	notification_BG.setTexture(*textures[0]);
	notification_BG.setOrigin(145,0);
	x_pos = (float)window->getSize().x, y_pos = (float)window->getSize().y;
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
		win_x = (float)window->getSize().x, win_y = (float)window->getSize().y;

		if (win_x / 350.f < win_y / 350.f) scale = win_x / 350.f;
		else scale = win_y / 350.f;
		pos_scale = scale;

		if (VideoMode::getDesktopMode().width < win_x * 1.7f || VideoMode::getDesktopMode().height < win_y * 1.7f) scale *= 0.75f;

		notification_BG.setScale(scale * 0.8f, scale * 0.8f);
		x_pos = win_x - 10 * pos_scale;
		y_pos *= pos_scale;
		text_x = x_pos - 145 * (scale * 0.8f) / 2.f;
		text_y = y_pos + 60 * (scale * 0.8f) / 2.f;
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
	if (states->rbegin()->first != DialogueID) {
		window->draw(notification_BG);
		render_strings();
	}
}