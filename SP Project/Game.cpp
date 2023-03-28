#include "Game.h"
void Game::intial_window()
{
	//gets window properties from "config/window.ini";
	string title = "none";
	VideoMode windowbounds (800, 600);
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
	this->window = new RenderWindow(windowbounds, title);
	this->window->setFramerateLimit(framelimit);
	this->window->setVerticalSyncEnabled(vsync);
}
void Game::intial_states()
{
	//sets intial states (will probably push MainMenuState)
	states.push_back(new SavesState);  /////////////////////////////////////
}
Game::Game()
{
	//runs all intializers 
	this->intial_window();
	this->intial_states();
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
	updatedt();
	pollevent();
	//calls update function of the top state in the deque
	if (!states.empty())
		this->states.back()->update(dt);
}

void Game::render()
{
	//renders all states in the deque (from buttom to top)
	this->window->clear();

	//draw objects
	for (auto state : states)
		state->render(window);

	this->window->display();
}

void Game::run()
{
	//game loop
	while (this->window->isOpen()) {
		//cout << 1.0 / dt << endl; //<----- framerate on console
		cout << sf::Mouse::getPosition().x << "   " << sf::Mouse::getPosition().y << "\n";
		update();
		render();
	}
}
