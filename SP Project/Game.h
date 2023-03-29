#pragma once
#include"State.h"
#include"MapBuilderState.h"
#include"Background.h"
#include"SavesState.h"
#include"MainMenuState.h"

using namespace std;
using namespace sf;

class Game
{
private:
	//variables:
	RenderWindow* window;
	Event event;
	Clock dtclock;
	VideoMode* videomode;
	deque<State*> states;
	map<string,Texture*> textures;
	float dt = 0;


	//private functions:
	void initial_window();
	void initial_states();

public:
	//constructors/destructors:
	Game();
	~Game();

	//public functions:
	void updatedt();
	void pollevent();
	void update();
	void render();
	void run();
};