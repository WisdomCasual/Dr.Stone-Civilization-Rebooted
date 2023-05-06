#pragma once
#include<bits\stdc++.h>

#include<SFML\System.hpp>
#include<SFML\Window.hpp>
#include<SFML\Graphics.hpp>
#include<SFML\Audio.hpp>


using namespace std;
using namespace sf;

inline Event text_event;
struct State
{
private:

public:
	//constructors/destructors:	
	State();
	virtual ~State();


	//public variables:
	struct tile_properties {
		short props = 0; // (AND &) destructable: 1, hitbox: 2, blocked: 4, back/front: 8, front core: 16, destruction core: 32, opaque: 64
	};
	struct sheet_properties {
		short x_size = 0, y_size = 0; // size of sprite sheet (in tile count)
		tile_properties** properties; // 2d array
	};
	short sheets_no = 0;
	vector <Texture*> textures, tile_sheets; //<-- current loaded Textures
	sheet_properties tile_props[15];

	Vector2f delta_movement();
	struct tex_tile {   //<-- stores texture tile info 
		int x, y, tex_id;
		bool select_done = 0, global_select_done = 0;;
		Vector2i selection_start = { 0,0 }, selection_end = { 0,0 },
			     previous_drawn_tile = { -1,-1 }, previous_erased_tile = { -1,-1 };;
		int start_x, start_y, wdth = 0, hght = 0;
		int global_layer = 0;
	};

	

	//public functions:
	void initial_textures(string);
	void initial_tile_sheets(string);

	virtual void pollevent() = 0;
	virtual void update() = 0;
	virtual void render() = 0;

};