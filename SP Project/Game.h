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
#include"InventoryState.h"
#include "MiniMapState.h"

#include"Global.h"
using namespace globalvar;

using namespace std;
using namespace sf;

struct Game
{
private:
	//private variables:
	RenderWindow* window;
	VideoMode* videomode;
	map<int, State*> states;
	map<string, Texture*> textures;
	bool prev_fullscreen = 0;
	short menu_music_idx = 0, ingame_music_idx = 0;
	const short menu_music_count = 3, ingame_music_count = 5;
	short menu_music[3] = { 0, 1, 2 }, ingame_music[5] = { 3, 4, 5, 6, 7};

	//private functions:
	void initial_window();
	void initial_states();
	void initial_icon();
	void initial_fps();
	void calc_fps();
	void initial_sounds();

public:
	//public variables:
	Music music;
	SoundBuffer click_buff;
	Sound click;
	string title = "none";
	VideoMode windowbounds = { 800, 600 }, prev_res = { 0, 0 };
	int framelimit = 120;
	void erase_states(int[], int);
	void exit_prompt();
	void notification(string[], int, float up_time = 3);

	//constructors/destructors:
	Game();
	~Game();

	//public functions:
	void play_music(int track_num = -1);
	void play_sfx(int);
	void save();
	void update_window();
	void updatedt();
	void pollevent();
	void update();
	void render();
	void run();
};
inline Game game;