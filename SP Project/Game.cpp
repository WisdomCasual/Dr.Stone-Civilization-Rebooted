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
}
void Game::initial_states()
{
	//sets intial states 
	states.push_back(new Background);
	states.push_back(new MainMenuState);
}

void Game::initial_icon()
{
	Image icon;
	icon.loadFromFile("Resources/mainicon.png"); // File/Image/Pixel
	window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

Game::Game()
{
	//runs all intializers 
	initial_window();
	initial_states();
	initial_icon();
}

Game::~Game()
{
	//destructor for: 
	//window
	delete this->window;

	//states deque
	while (!states.empty()) {
		delete this->states.back();
		this->states.pop_back();
	}
}

void Game::updatedt()
{
	//updates the DeltaTime;
	dt = dtclock.restart().asSeconds();
}

void Game::pollevent()
{
	//calls pollevent update function of the top state in the deque
	if (!states.empty())
		states.back()->pollevent(event, window);
}

void Game::update()
{
	while (terminator != 0) {
		delete states.back();
		states.pop_back();
		terminator--;
	}
	updatedt();
	pollevent();

	//calls update function of the top state in the deque
	if (!states.empty())
		this->states.back()->update(dt, window, &terminator, &states);
}

void Game::render()
{
	//renders all states in the deque (from buttom to top)

	this->window->clear();

	//draw objects
	for (auto state : states)
		state->render(window);
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
