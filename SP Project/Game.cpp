#include "Game.h"
void Game::initial_window()
{
	//gets window properties from "config/window.ini";
	string title = "none";
	VideoMode windowbounds(800, 600);
	int framelimit = 120;
	bool vsync = 0;
	ifstream ifs("config/window.ini");
	if (ifs.is_open()) {
		getline(ifs, title);
		ifs >> windowbounds.width >> windowbounds.height;
		ifs >> framelimit;
		ifs >> vsync;
	}
	ifs.close();
	this->window = new RenderWindow(windowbounds, title, Style::Titlebar | Style::Close);
	this->window->setFramerateLimit(framelimit);
	this->window->setVerticalSyncEnabled(vsync);
	globalvar::window = this->window;
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
}

Game::Game()
{
	globalvar::states = &this->states;

	text.setFont(font);

	//runs all intializers 
	initial_window();
	initial_states();
	initial_icon();
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

void Game::update_window(VideoMode resolution, string title, int framelimit, bool vsync)
{
	if ( Vector2u { resolution.width, resolution.height } != this->window->getSize()) {
		delete this->window;
		this->window = new RenderWindow(resolution, title, Style::Titlebar | Style::Close);	
		globalvar::window = this->window;
		fps_text.setCharacterSize(40 * ((float)window->getSize().y / 1080.0));
	}
	this->window->setFramerateLimit(framelimit);
	this->window->setVerticalSyncEnabled(vsync);

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