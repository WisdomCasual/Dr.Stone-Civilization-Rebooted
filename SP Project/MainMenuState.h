#pragma once
#include"State.h"
#include"SavesState.h"
#include"Background.h"
#include"SettingsState.h"

#include"Global.h"
using namespace globalvar;

using namespace sf;
using namespace std;
struct MainMenuState :public State
{
private:
	//variables
	float scale = 0;	
	int x=0, y=0;
	Sprite buttontex,logo;
	Text text;
	struct button{
		string txt;
		int x = 0, y = 0;
		bool& execute;
		int pressed = 0;
		bool hover = 0;
	};
	vector<button> buttons;
	Vector2f clicked_on = { -1, -1 };
	bool play = 0, settings = 0, exit = 0;

	//private functions
	void update_buttons();
	void render_buttons();
public:
	//constructors/destructors:
	MainMenuState();
	~MainMenuState();
	//public functions:
	void scaling();
	void update();
	void render();
	void pollevent();
		
};

