#pragma once
#include"State.h"
#include"MapBuilderState.h"
#include"Background.h"

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
	void intial_window();
	void intial_states();

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