#pragma once
#include"State.h"
#include"SavesState.h"
#include"Background.h"
#include"SettingsState.h"
using namespace sf;
using namespace std;
class MainMenuState :public State
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
	bool play = 0, settings = 0, exit = 0;

	//private functions
	void update_buttons(RenderWindow*);
	void render_buttons(RenderWindow*);
public:
	//constructors/destructors:
	MainMenuState();
	~MainMenuState();
	//public functions:
	void update(float, RenderWindow*, int*, deque<State*>*);
	void render(RenderWindow*);
	void pollevent(Event, RenderWindow*);
		
};

