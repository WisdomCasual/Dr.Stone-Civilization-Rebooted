#include "GameState.h"


void GameState::load_map(string map_name)
{
	ifstream ifs("Maps/" + map_name + ".mp");
	string line;
	
	if (!(ifs >> line))
		return;

	short mpsize = 0, count[2]{ 0,0 };
	Vector3i tle;
	char layr;
	Vector3i temp_layers[2][8]{};
	short layer_prop = 0;

	deload_map();

	
	ifs.seekg(ios::beg);

	if (ifs.is_open()) {

		ifs >> size_x >> size_y;

		static_map = new render_tile* [size_x];

		for (int i = 0; i < size_x; i++) {
			
			static_map[i] = new render_tile[size_y];

			for (int j = 0; j < size_y; j++) {
				ifs >> mpsize;
				while (mpsize--) {

					ifs >> layr >> tle.x >> tle.y >> tle.z;

					layer_prop = tile_props[tle.z].properties[tle.x][tle.y].props;

					
					if (layer_prop & 16) { // inbetween
						for (int p = 0; p < 2; p++) {
							temp_layers[p][count[p]] = tle;
							count[p]++;
						}
						//add to dynamic tiles
					}
					else if (layer_prop & 8) { // front
						temp_layers[1][count[1]] = tle;
						count[1]++;
					}
					else { // back
						temp_layers[0][count[0]] = tle;
						count[0]++;
					}

					static_map[i][j].tile_props |= layer_prop;
				}

				for (int p = 0; p < 2; p++) { // front/back
					static_map[i][j].size[p] = count[p];
					static_map[i][j].layer[p] = new Vector3i[count[p]];

					for (int l = 0; l < count[p]; l++) // layers
						static_map[i][j].layer[p][l] = temp_layers[p][l];
					count[p] = 0;
				}
			}
		}
	}
	ifs.close();
}

void GameState::deload_map()
{

	// static map destructor;

	for (int i = 0; i < size_x; i++) {
		for (int j = 0; j < size_y; j++) {
			delete[] static_map[i][j].layer[0];
			delete[] static_map[i][j].layer[1];

		}
		delete[] static_map[i];
	}
	if (size_x)
		delete[] static_map;
	size_x = 0, size_y = 0;
}

void GameState::initial_game()
{
	load_map("Sheraton");
}

void GameState::render_map(int priority)
{
	tile.setScale(scale, scale);
	for (int i = (x_offset > 0) ? x_offset : 0; i < (win_x + ((x_offset + 1) * 16 * scale)) / (16 * scale) && i < size_x; i++)
		for (int j = (y_offset > 0) ? y_offset : 0; j < (win_y + ((y_offset + 1) * 16 * scale)) / (16 * scale) && j < size_y; j++) {
			for (auto map_tile = static_map[i][j].layer[priority]; map_tile != static_map[i][j].layer[priority] + static_map[i][j].size[priority]; map_tile++) {
				tile.setTexture(*tile_sheets[map_tile->z]);
				tile.setTextureRect(IntRect(map_tile->x * 16, map_tile->y * 16, 16, 16));
				tile.setPosition(map_x + (16 * scale * i), map_y + (16 * scale * j));
				window->draw(tile);
			}
		}
}

GameState::GameState()
{
	initial_tile_sheets("game/tiles");
	load_maps(); //loads all maps ( pins[name]  { world map location x, world map location y, size x, size, y })
	initial_game();
	
}

GameState::~GameState()
{

}

void GameState::update()
{
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 480.0 < win_y / 270.0) scale = win_x / 480.0;
		else scale = win_y / 270.0;
		/////////////////////


	}

	////////////
	map_x -= dt * scale * delta_movement().x * 500;
	map_y -= dt * scale * delta_movement().y * 500;

	x_offset = -map_x / (scale * 16);
	y_offset = -map_y / (scale * 16);
	//////////


}


void GameState::render()
{
	render_map(0);

	render_map(1);
}

void GameState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				//add exit functionality
				break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			}
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case Mouse::Left:
				clicked_on = window->mapPixelToCoords(Mouse::getPosition(*window));
				break;
			}
		}
	}
}