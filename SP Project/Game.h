#pragma once
#include"State.h"
#include"MapBuilderState.h"
#include"Background.h"
#include"SavesState.h"
#include"MainMenuState.h"
#include"PauseState.h"
#include"WorldMapState.h"
#include"NewSaveState.h"
#include"NewMapState.h"
#include"ConfirmationState.h"
#include"PasswordState.h"
#include"NotificationState.h"
#include"TestGroundState.h"
#include"DialogueState.h"
#include"GameState.h"
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
	VideoMode windowbounds = { 800, 600 }, prev_res = { 0, 0 };
	int framelimit = 120;
	void erase_states(int[], int);
	void exit_prompt();
	void notification(string[], int);

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