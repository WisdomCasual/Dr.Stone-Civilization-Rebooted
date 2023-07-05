#include "Game.h"
void Game::initial_window()
{
	//gets window properties from "config/window.ini";
	if (prev_res != windowbounds || prev_fullscreen != fullscreen) {
		ifstream ifs("config/window.ini");
		if (ifs.is_open()) {
			getline(ifs, title);
			ifs >> windowbounds.width >> windowbounds.height;
			ifs >> framelimit;
			ifs >> fullscreen;
			ifs >> vsync;
			ifs >> game_volume;
			ifs >> music_volume;
		}
		ifs.close();

		if (fullscreen)
			this->window = new RenderWindow(VideoMode::getDesktopMode(), title, Style::Fullscreen);
		else
			this->window = new RenderWindow(windowbounds, title, Style::Titlebar | Style::Close);

		prev_res = windowbounds;
		prev_fullscreen = fullscreen;
	}
	if (vsync) {
		this->window->setFramerateLimit(0);	
	}
	else
		this->window->setFramerateLimit(framelimit);
	this->window->setVerticalSyncEnabled(vsync);

	globalvar::window = this->window;
	initial_icon();
}

void Game::initial_states()
{
	//sets intial states 
	states.insert(BackgroundST);
	states.at(BackgroundID)->update();
	states.insert(MainMenuST);
	states.at(MainMenuID)->update();
}

void Game::initial_icon()
{
	Image icon;
	icon.loadFromFile("Resources/mainicon.png"); // File/Image/Pixel
	window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void Game::initial_fps()
{
	font.loadFromFile("Resources/font.ttf");
	fps_text.setFont(font);
	fps_text.setCharacterSize(40 * ((float)window->getSize().y / 1080.0));
}

void Game::calc_fps()
{
	//calculates framerate per second
	delay += dt; frame_sum += 1.0 / dt; frame_count++;
	if (delay > 1.0) { fps = frame_sum / frame_count; delay = 0, frame_sum = 0, frame_count = 0; }
	fps_text.setString("  FPS " + to_string(fps));
	fps_text.setCharacterSize(40 * ((float)window->getSize().y / 1080.0));
}

void Game::initial_sounds()
{
	shuffle(ingame_music, ingame_music + ingame_music_count, std::default_random_engine(generate_random(0, INT_MAX)));
	shuffle(menu_music, menu_music + menu_music_count, std::default_random_engine(generate_random(0, INT_MAX)));

	click_buff.loadFromFile("Audio/ui/click.ogg");
	click.setBuffer(click_buff);
}

void Game::erase_states(int exceptions[], int size = 0)
{
	map<int, State*> temp; bool skip;
	for (auto st = states.begin(); st != states.end(); st++) {
		skip = 0;
		for (int i = 0; i < size; i++)
			if (st->first == exceptions[i]) {
				temp.insert(*st);
				skip = 1;
			}
		if (!skip)
			delete st->second;
	}
	states = temp;
	//for (auto& state : states)
	//	state.second->update();
}

void Game::exit_prompt()
{
	if (states.find(MapBuilderID) == states.end() && states.find(GameID) == states.end() && states.find(WorldMapID) == states.end()) {
		string strings_array[] = { "Are you sure that you", "want to exit the", "game?" };
		states.insert({ ConfirmationID, new ConfirmationState(strings_array,3, exit_game) });
	}
	else {
		string strings_array[] = { "Are you sure that you", "want to exit the", "game?", "" , "Progress won't be saved" };
		states.insert({ ConfirmationID, new ConfirmationState(strings_array,5, exit_game) });
	}
	states.at(ConfirmationID)->update();
}

void Game::notification(string strings_arr[], int lines, float up_time)
{
	for (int i = 0; i < lines; i++) {
		notification_strings[i] = strings_arr[i];
	}
	notification_lines = lines + 3;
	notification_delay = up_time;
	if (states.find(NotificationID) == states.end()) {
		states.insert(NotificationST);
	}
	//states.at(NotificationID)->update();
}

Game::Game()
{
	globalvar::states = &this->states;

	text.setFont(font);

	//runs all intializers
	initial_window();
	initial_states();
	initial_fps();
	initial_sounds();
	
}

Game::~Game()
{
	//destructor for: 
	//window
	delete this->window;

	//states map
	for (auto &state: states) {
		delete state.second;
	}
	states.clear();
}

void Game::play_music(int track_num)
{
	if (music.getVolume() != music_volume)
		music.setVolume(music_volume);

	if (!music.getStatus()) {	
		if (track_num == -1) {
			if (states.find(GameID) != states.end() || states.find(WorldMapID) != states.end() || states.find(MapBuilderID) != states.end()) {
				if (ingame_music_idx > ingame_music_count - 1) {
					ingame_music_idx = 0;
					shuffle(ingame_music, ingame_music + ingame_music_count, std::default_random_engine(generate_random(0, INT_MAX)));
				}
				track_num = ingame_music[ingame_music_idx];
				ingame_music_idx++;
			}
			else {
				if (menu_music_idx > menu_music_count - 1) {
					menu_music_idx = 0;
					shuffle(menu_music, menu_music + menu_music_count, std::default_random_engine(generate_random(0, INT_MAX)));
				}
				track_num = menu_music[menu_music_idx];
				menu_music_idx++;
			}
		}
		music.openFromFile("Audio/Music/" + to_string(track_num) + ".ogg");
		music.play();
	}
}

void Game::play_sfx(int sfx_id)
{
	switch (sfx_id) {
	case 0:
		click.setVolume(game_volume);
		click.setPitch(0.6);
		click.play();
		break;
	case 1:
		click.setVolume(game_volume);
		click.setPitch(0.5);
		click.play();
		break;

	}
}

void Game::save()
{
	ofstream ofs("config/window.ini");
	if (ofs.is_open()) {
		ofs << title << '\n';
		ofs << windowbounds.width << ' ' << windowbounds.height << '\n';
		ofs << framelimit << '\n';
		ofs << fullscreen << '\n';
		ofs << vsync << '\n';
		ofs << game_volume << ' ';
		ofs << music_volume << '\n';
		ofs.close();
	}
}

void Game::update_window()
{
	save();

	if (prev_res != windowbounds || prev_fullscreen != fullscreen)
		delete window;
	initial_window();

	for (auto& state : states)
		state.second->update();
}

void Game::updatedt()
{
	//updates the DeltaTime;
	globalvar::dt = dtclock.restart().asSeconds();
}

void Game::pollevent()
{
	//calls pollevent update function of the top state in the map

	if (!states.empty())
		states.rbegin()->second->pollevent();

}

void Game::update()
{
	updatedt();
	pollevent();
	calc_fps();
	//calls update function of the top state in the map

	play_music();

	if (!states.empty())
		this->states.rbegin()->second->update();

	if (exit_game)
		window->close();
}

void Game::render()
{
	//renders all states in the deque (from buttom to top)
	this->window->clear();

	//draw objects
	for (auto &state : states)
		state.second->render();

	if (fps_active)
		window->draw(fps_text);

	window->display();
}

void Game::run()
{
	//game loop
	while (this->window->isOpen()) {
		update();
		render();
	}
}