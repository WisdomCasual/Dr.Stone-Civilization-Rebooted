#pragma once
#include"State.h"

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

#define WorldMapID 7
#define WorldMapST { 7, new WorldMapState }
#define WorldMapST_admin { 7, new WorldMapState(1) }

#define NewMapID 8
#define NewMapST { 8, new NewMapState}

#define PauseID 10
#define PauseST { 10, new PauseState }

#define SettingsID 11
#define SettingsST { 11, new SettingsState }

#define ConfirmID 12                        /////
#define ConfirmST { 12, new ConfirmState }  /////

namespace globalvar {

	inline float dt;
	inline RenderWindow* window;
	inline map<int, State*>* states;
	inline Event event;

	inline int fps = 0, frame_count = 0, frame_sum = 0;
	inline bool fps_active = 0, fullscreen = 0, vsync = 0;
	inline float delay = 0;

	inline Font font;
	inline Text fps_text, text;

	inline void draw_text(string tex, int a, int b, float c)   //<-- gets window pointer, string, X cord, Y cords, Character size to draw text
	{
		//gets window pointer, string, X cord, Y cords, Character size to draw text
		text.setString(tex);
		text.setCharacterSize(c);
		text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().top + text.getLocalBounds().height / 2.0);
		text.setPosition(a, b);
		window->draw(text);
	}

}