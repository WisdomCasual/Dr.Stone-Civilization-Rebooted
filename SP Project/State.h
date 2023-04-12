#pragma once
#include<bits\stdc++.h>

#include<SFML\System.hpp>
#include<SFML\Window.hpp>
#include<SFML\Graphics.hpp>
#include<SFML\Audio.hpp>

// background: 1, MainMenuState: 2 , SavesState: 3 , NewSaveState: 4 , GameState: 5 , 
//MapBuilderState: 6, PauseState: 10, SettingsState: 11, ConfirmState: 12: 

#define BackgroundID 1
#define BackgroundST { 1, new Background }

#define MainMenuID 2
#define MainMenuST { 2, new MainMenuState }

#define SavesID 3
#define SavesST { 3, new SavesState }

#define NewSaveID 4                       ///////
#define NewSaveST { 4, new NewSaveState } ///////

#define GameID 5
#define GameST { 5, new GameState }

#define MapBuilderID 6
#define MapBuilderST { 6, new MapBuilderState }

#define PauseID 10
#define PauseST { 10, new PauseState }

#define SettingsID 11
#define SettingsST { 11, new SettingsState }

#define ConfirmID 12                        /////
#define ConfirmST { 12, new ConfirmState }  /////


using namespace std;
using namespace sf;

inline int fps = 0, frame_count = 0, frame_sum = 0;
inline bool fps_active = 0;
inline float delay = 0;

struct State
{
private:

public:
	//constructors/destructors:	
	State();
	virtual ~State();

	//public variables:
	vector <Texture*> textures; //<-- current loaded Textures
	Vector2f delta_movement();
	struct tex_tile {   //<-- stores texture tile info 
		int x, y, tex_id;
		bool select_done = 0, global_select_done = 0;;
		Vector2i selection_start = { 0,0 }, selection_end = { 0,0 },
			     previous_drawn_tile = { -1,-1 }, previous_erased_tile = { -1,-1 };;
		int start_x, start_y, wdth = 0, hght = 0;
		int global_layer = 0, global_priority = 0;
	};
	Font font;
	Text fps_text, text;

	//public functions:
	void initial_textures(string);
	void initial_fps();
	void calc_fps(float);
	void draw_text(RenderWindow*, string, int, int, int);  //<-- gets window pointer, string, X cord, Y cords, Character size to draw text


	virtual void pollevent(Event, RenderWindow*, int*, map<int, State*>*) = 0;
	virtual void update(float, RenderWindow*, int*, map<int, State*>*) = 0;
	virtual void render(RenderWindow*) = 0;

};