#pragma once
#include"State.h"
#include"MapBuilderState.h"
#include"Background.h"
#include"SavesState.h"
#include"MainMenuState.h"
#include"PauseState.h"
#include "WorldMapState.h"
#include "NewSaveState.h"

#include"Global.h"
using namespace globalvar;

using namespace std;
using namespace sf;

struct Game
{
private:
	//private variables:
	RenderWindow* window;
	Clock dtclock;
	VideoMode* videomode;
	map<int, State*> states;
	map<string, Texture*> textures;
	VideoMode prev_res = { 0, 0 };
	bool prev_fullscreen = 0;

	//private functions:
	void initial_window();
	void initial_states();
	void initial_icon();
	void initial_fps();
	void calc_fps();

public:
	//public variables:
	string title = "none";
	VideoMode windowbounds = { 800, 600 };
	int framelimit = 120;
	bool vsync = 0;
	void erase_states(int[], int);

	//constructors/destructors:
	Game();
	~Game();

	//public functions:
	void update_window();
	void updatedt();
	void pollevent();
	void update();
	void render();
	void run();
};
inline Game game;