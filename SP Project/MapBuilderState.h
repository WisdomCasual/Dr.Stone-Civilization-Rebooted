#pragma once
#include"State.h"

using namespace std;

class MapBuilderState : public State
{
private:
	//variables:
	 int size_x, size_y;  //<-- map size
	 int x = 0, y = 0;    //<-- location of upper left corner of the map
	 int x_offset = 0, y_offset = 0; //<-- offset from upper left corner of the screen to upper left corner of the map
	 int x_mid = 0, y_mid = 0;       //<-- index of the tile in the center of the screen
	 int cam_speed = 150;
	 bool active_grid = 1; //<-- grid active/inactive
	 float scale = 10;
	 float dt = 0;
	 pair<char, char> Map[256][256];    //the level map

	 //private functions:
	 void grid(RenderWindow*, int, int);


public:
	//constructors/destructors:
	MapBuilderState(int a = 64,int b = 64);
	~MapBuilderState();
	
	//public functions:
	void update(float);
	void render(RenderWindow*);
	void pollevent(Event, RenderWindow*);
};

