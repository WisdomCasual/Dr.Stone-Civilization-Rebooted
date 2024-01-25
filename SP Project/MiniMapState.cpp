#include "MiniMapState.h"

void MiniMapState::fade_in()
{
	if (transparency < 255) {
		if (transparency + 1500 * dt > 255)
			transparency = 255;
		else
			transparency += 1500 * dt;

		minimap.setColor(Color(255, 255, 255, (int)transparency));
		player_pointer.setFillColor(Color(255, 0, 0, (int)transparency));
		waypoint_pointer.setFillColor(Color(0, 0, 255, (int)transparency));
		quest_pointer.setFillColor(Color(255, 200, 0, (int)transparency));

		if (darkness < 154) {
			if (darkness + 154 * dt * 6 > 154)
				darkness = 154;
			else
				darkness += 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, (int)darkness));
		}
	}
}

bool MiniMapState::fade_out()
{
	if (transparency > 0) {
		if (transparency - 1500 * dt < 0)
			transparency = 0;
		else
			transparency -= 1500 * dt;

		minimap.setColor(Color(255, 255, 255, (int)transparency));
		player_pointer.setFillColor(Color(255, 0, 0, (int)transparency));
		waypoint_pointer.setFillColor(Color(0, 0, 255, (int)transparency));
		quest_pointer.setFillColor(Color(255, 200, 0, (int)transparency));

		if (darkness > 0) {
			if (darkness - 154 * dt * 6 < 0)
				darkness = 0;
			else
				darkness -= 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, (int)darkness));
		}
		return false;
	}
	else
		return true;
}

MiniMapState::MiniMapState(Texture& map_tex, Vector2f player_position, Vector2f& waypoint, Vector2f& quest_location)
	: waypoint(waypoint), quest_location(quest_location)
{
	this->player_position = player_position;
	minimap.setTexture(map_tex);
	minimap.setOrigin(minimap.getLocalBounds().width / 2, minimap.getLocalBounds().height / 2);
	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));
}

MiniMapState::~MiniMapState()
{

}

void MiniMapState::update()
{
	active_cursor = true; ///////////////<------------

	mouse_pos = Mouse::getPosition(*window);
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = (float)window->getSize().x, win_y = (float)window->getSize().y;
		x = win_x / 2.f, y = win_y / 2.f;
		x_pic = minimap.getLocalBounds().width;
		y_pic = minimap.getLocalBounds().height;
		if (win_x / x_pic < win_y / y_pic) map_scale = win_x / x_pic;
		else map_scale = win_y / y_pic;
		if (win_x / 540.f < win_y / 304.f) scale = win_x / 540.f;
		else scale = win_y / 304.5f;
		/////////////////////////////////////////////////////////

		tint.setSize({ win_x, win_y });
		minimap.setPosition(x, y);
		minimap.setScale(map_scale * 0.8f, map_scale * 0.8f);

		player_pointer.setRadius(1.5f * scale);
		player_pointer.setOrigin(0.75f * scale, 0.75f * scale);
		player_pointer.setPosition(x - (x_pic / 2.f - player_position.x / 8.f) * map_scale * 0.8f, y - (y_pic / 2.f - player_position.y / 8.f) * map_scale * 0.8f);

		waypoint_pointer.setRadius(1.5f * scale);
		waypoint_pointer.setOrigin(0.75f * scale, 0.75f * scale);

		quest_pointer.setRadius(1.5f * scale);
		quest_pointer.setOrigin(0.75f * scale, 0.75f * scale);
		quest_pointer.setPosition(x - (x_pic / 2.f - quest_location.x / 8.f) * map_scale * 0.8f, y - (y_pic / 2.f - quest_location.y / 8.f) * map_scale * 0.8f);

	}
	if(waypoint.x > -1)
		waypoint_pointer.setPosition(x - (x_pic / 2.f - waypoint.x) * map_scale * 0.8f, y - (y_pic / 2.f - waypoint.y) * map_scale * 0.8f);

	if (exit) {
		if (fade_out()) {
			exit = 0;
			delete states->at(MiniMapID);
			states->erase(MiniMapID);
			return;
		}
	}
	else
		fade_in();
}

void MiniMapState::render()
{
	window->draw(tint);
	text.setFillColor(Color(255, 255, 255, (int)transparency));
	draw_text("Press M to exit", x, y + (y_pic / 2 * map_scale * 0.9f), scale * 16);
	window->draw(minimap);

	if(quest_location.x > -1)
		window->draw(quest_pointer);

	if(waypoint.x > -1)
		window->draw(waypoint_pointer);

	window->draw(player_pointer);
}

void MiniMapState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
			case Keyboard::M:
				exit = 1; break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			}
			break;
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case Mouse::Left:
				clicked_on = window->mapPixelToCoords(Mouse::getPosition(*window));
				waypoint.x = (clicked_on.x - (x - (x_pic * map_scale * 0.4f))) / (map_scale * 0.8f);
				waypoint.y = (clicked_on.y - (y - (y_pic * map_scale * 0.4f))) / (map_scale * 0.8f);
				if (!(waypoint.x >= 0 && waypoint.x <= minimap.getLocalBounds().width && waypoint.y >= 0 && waypoint.y <= minimap.getLocalBounds().height))
					waypoint = { -1, -1 };
				break;
			case Mouse::Right:
				waypoint = { -1, -1 };
			}
		}
	}
}