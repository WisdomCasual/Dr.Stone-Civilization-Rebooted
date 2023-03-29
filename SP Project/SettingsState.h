#pragma once
#include "State.h"
using namespace std;
using namespace sf;

class SettingsState :
	public State
{
private:
	IntRect tipsright[4] = { {190,330,9,18} ,{190,348,9,18} ,{372,312,9,18}, { 190,294,9,18 } },
		tipsleft[4] = { {370,126,9,18} ,{370,90,9,18} ,{372,294,9,18},{ 372,330,9,18 } },
		mids[4] = { {338,449,18,18} ,{356,368,18,18} ,{356,431,18,18},{338,386,18,18} },
		arrow = { 303,486,22,21 },
		buttons[4] = { {290,139,93,94} ,{240,0,93,94} ,{290,49,93,94},{201,296,92,92} },
		checkMark = { 370,45,16,15 };
	//Colors loaded in arrangment : Yellow -> Red -> Blue
	Sprite tipleft, tipright, mid,tissue;
	RectangleShape tint;
	float x = 0, y = 0;
	float win_x = 0, win_y = 0,scale = 1;
public:
	//constructors / destructors
	SettingsState();

	~SettingsState();

	void update(float, RenderWindow*, int*, deque<State*>*);

	void pollevent(Event, RenderWindow*);

	void render(RenderWindow*);
};