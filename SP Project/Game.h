#pragma once
#include"State.h"
#include"MapBuilderState.h"
#include"Background.h"
#include"SavesState.h"
#include"MainMenuState.h"
#include "PauseState.h"

using namespace std;
using namespace sf;

struct Game
{
private:
	//variables:
	RenderWindow* window;
	Event event;
	Clock dtclock;
	VideoMode* videomode;
	deque<State*> states;
	map<string, Texture*> textures;
	float dt = 0;
	int terminator;

	//private functions:
	void initial_window();
	void initial_states();
	void initial_icon();

public:
	//constructors/destructors:
	Game();
	~Game();

	//public functions:
	void update_window(VideoMode resolution, string title, int framelimit, bool vsync);
	void updatedt();
	void pollevent();
	void update();
	void render();
	void run();
};
inline Game game;