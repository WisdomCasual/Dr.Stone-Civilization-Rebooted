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
	states.insert(MainMenuST);

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
	for (auto& state : states)
		state.second->update();
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

void Game::notification(string strings_arr[], int lines)
{
	for (int i = 0; i < lines; i++) {
		notification_strings[i] = strings_arr[i];
	}
	notification_lines = lines+3;
	notification_delay = 0;
	if (states.find(NotificationID) == states.end())
		states.insert(NotificationST);
	states.at(NotificationID)->update();
}

Game::Game()
{
	globalvar::states = &this->states;

	text.setFont(font);

	//runs all intializers 
	initial_window();
	initial_states();
	initial_fps();
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

void Game::update_window()
{
	ofstream ofs("config/window.ini");
	if (ofs.is_open()) {
		ofs << title << '\n';
		ofs << windowbounds.width << ' ' << windowbounds.height << '\n';
		ofs << framelimit << '\n';
		ofs << fullscreen << '\n';
		ofs << vsync;
		ofs.close();
	}
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