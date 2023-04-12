#pragma once
#include"State.h"
#include"MapBuilderState.h"
#include"Background.h"
#include"SavesState.h"
#include"MainMenuState.h"
#include"PauseState.h"

#include"Global.h"
using namespace globalvar;

using namespace std;
using namespace sf;

struct Game
{
private:
	//variables:
	RenderWindow* window;
	Clock dtclock;
	VideoMode* videomode;
	map<int, State*> states;
	map<string, Texture*> textures;

	//private functions:
	void initial_window();
	void initial_states();
	void initial_icon();
	void initial_fps();
	void calc_fps();

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