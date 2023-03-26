#include "MapBuilderState.h"


MapBuilderState::MapBuilderState(int a, int b) : size_x(a), size_y(b)
{
	//loads "game" textures
	intial_textures("game");

	//gives intial water texture to all tiles 
	for (int i = 0; i < size_x; i++)
		for (int j = 0; j < size_y; j++)
			Map[i][j] = {21, 49};
}

MapBuilderState::~MapBuilderState()
{

}

void MapBuilderState::grid(RenderWindow* window, int x_win, int y_win)
{
	//draws grid lines, "G" to activate/deactivate
	if (active_grid) {
		RectangleShape rect;
		rect.setSize(Vector2f(3, y_win));
		rect.setFillColor(Color::Black);
		for (int i = 0; i < x_win - x % (int)(scale * 16); i += 16 * scale) {
			rect.setPosition(i + x % (int)(scale * 16), 0);
			window->draw(rect);
		}
		rect.setSize(Vector2f(x_win, 3));
		for (int i = 0; i < y_win - y % (int)(scale * 16); i += 16 * scale) {
			rect.setPosition(0, i + y % (int)(scale * 16));
			window->draw(rect);
		}
	}
}

void MapBuilderState::update(float dt)
{
	//moves the Camera, increase cam screen with "Left Ctrl"
	cam_speed = 150; this->dt = dt;
	if (Keyboard::isKeyPressed(Keyboard::LControl))
		cam_speed=500;
	Vector2f movement = delta_movement();
	x -= dt * cam_speed * movement.x * scale;
	y -= dt * cam_speed * movement.y * scale;
	x_offset = - x / (scale * 16);
	y_offset = -y / (scale * 16);

}

void MapBuilderState::render(RenderWindow* window)
{
	int x_win = window->getSize().x, y_win = window->getSize().y; //<-- window boundaries
	x_mid = x_offset + (x_win / (16 * scale) / 2), y_mid = y_offset + (y_win / (16 * scale) / 2); // updates offset

	//make as function//////////////////////
	//renders the map in active screen area only
	Sprite tile;
	tile.setTexture(*textures["Outdoors_Spring"]);
	tile.setScale(scale, scale);
    for (int i = (x_offset>0)? x_offset : 0; i < (x_win+((x_offset+1)*16*scale)) / (16*scale) && i < size_x; i++)
		for (int j = (y_offset > 0) ? y_offset : 0; j < (y_win + ((y_offset+1) * 16 * scale)) / (16 * scale) && j < size_y; j ++) {
			tile.setTextureRect(IntRect(Map[i][j].first * 16, Map[i][j].second * 16, 16, 16));
			tile.setPosition(x + (16 * scale * i), y + (16 * scale * j));
			window->draw(tile);
		}
	////////////////////////

	grid(window, x_win, y_win);
}

void MapBuilderState::pollevent(Event event, RenderWindow* window)
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			window->close(); break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				window->close(); break;

			//activate deactivate grid lines
			case Keyboard::G:
				active_grid = !active_grid; break;
			}

		//camera zoom
		case Event::MouseWheelMoved:
			if (event.type == Event::MouseWheelMoved)
				if (scale + event.mouseWheel.delta > 1 && scale + event.mouseWheel.delta < 20) {
					x -= event.mouseWheel.delta * 16 * x_mid;
					y -= event.mouseWheel.delta * 16 * y_mid;
					scale += event.mouseWheel.delta;
				} break;
		}
	}
}
