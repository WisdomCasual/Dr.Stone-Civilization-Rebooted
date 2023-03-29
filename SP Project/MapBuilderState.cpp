#include "MapBuilderState.h"


MapBuilderState::MapBuilderState(int a, int b) : size_x(a), size_y(b)
{	
	//loads "game" textures
	initial_textures("game");
	
	initial_fps();
	info.setFont(font);
	info.setCharacterSize(40);
}

void MapBuilderState::update_info_text()
{
	//displays picked tile properties
	info.setString("\n        Selected Tile ID " + to_string(picked_tile.x) + " " + to_string(picked_tile.y) + " - spritesheet ID " + to_string(picked_tile.tex_id)
		+ "\n        Blocked (b) " + to_string(props[picked_tile.x][picked_tile.y].blocked) + " - hitbox (h) " + to_string(props[picked_tile.x][picked_tile.y].hitbox)
		+ " - Layer (1,2,3) " + to_string(props[picked_tile.x][picked_tile.y].layer+1));
}

void MapBuilderState::dash_cam()
{
	//moves the Camera, increase cam screen with "Left Ctrl"
	cam_speed = 150;
	if (Keyboard::isKeyPressed(Keyboard::LControl))
		cam_speed = 500;
	Vector2f movement = delta_movement();
	x -= dt * cam_speed * movement.x * scale;
	y -= dt * cam_speed * movement.y * scale;
	x_offset = -x / (scale * 16);
	y_offset = -y / (scale * 16);
}

MapBuilderState::~MapBuilderState()
{
	delete this->tex_picker;
}

void MapBuilderState::grid(RenderWindow* window, int x_win, int y_win)
{
	//draws grid lines, "G" to activate/deactivate
	if (active_grid) {
		RectangleShape rect;
		rect.setSize(Vector2f(2, y_win));
		rect.setFillColor(Color::Black);
		for (int i = 0; i < x_win - x % (int)(scale * 16); i += 16 * scale) {
			rect.setPosition(i + x % (int)(scale * 16), 0);
			window->draw(rect);
		}
		rect.setSize(Vector2f(x_win, 2));
		for (int i = 0; i < y_win - y % (int)(scale * 16); i += 16 * scale) {
			rect.setPosition(0, i + y % (int)(scale * 16));
			window->draw(rect);
		}
	}
}

void MapBuilderState::render_tiles(RenderWindow* window, int x_win, int y_win, int layer)
{
	//renders the map in active screen area only
	Sprite tile;
	tile.setTexture(*textures[1]);
	tile.setScale(scale, scale);
	for (int i = (x_offset > 0) ? x_offset : 0; i < (x_win + ((x_offset + 1) * 16 * scale)) / (16 * scale) && i < size_x; i++)
		for (int j = (y_offset > 0) ? y_offset : 0; j < (y_win + ((y_offset + 1) * 16 * scale)) / (16 * scale) && j < size_y; j++) {
			tile.setTextureRect(IntRect(tiles[i][j].layers[layer].x * 16, tiles[i][j].layers[layer].y * 16, 16, 16));
			tile.setPosition(x + (16 * scale * i), y + (16 * scale * j));
			window->draw(tile);
		}
	grid(window, x_win, y_win);
	tile.setTextureRect(IntRect(picked_tile.x * 16, picked_tile.y * 16, 16, 16));
	tile.setPosition(15, 50); tile.setScale(4, 4);	tile.setTexture(*textures[picked_tile.tex_id]);
	window->draw(tile);
}

void MapBuilderState::update(float dt, RenderWindow* window, int* terminator, deque<State*>* states)
{
	this->dt = dt;

	if (fps_active)
		calc_fps(dt);

	dash_cam();

	if (!picker)
		tex_picker->run(picker);

	update_info_text();
}

void MapBuilderState::render(RenderWindow* window)
{
	int x_win = window->getSize().x, y_win = window->getSize().y; //<-- window boundaries
	x_mid = x_offset + (x_win / (16 * scale) / 2), y_mid = y_offset + (y_win / (16 * scale) / 2); // updates offset


	render_tiles(window, x_win, y_win, 0);
	render_tiles(window, x_win, y_win, 1);
	//----------render entities herre in game--------------
	render_tiles(window, x_win, y_win, 2);
	window->draw(info);
	if (fps_active)
		window->draw(fps_text);
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
			case Keyboard::Space:
				x = 0, y = 0; break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::E:
				if (picker)
					tex_picker = new CreativeMode(&textures, picked_tile);
				else
					delete this->tex_picker;
				picker = !picker; break;
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
