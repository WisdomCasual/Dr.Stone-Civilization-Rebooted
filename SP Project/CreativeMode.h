#pragma once
#include"State.h"

#include"Global.h"
using namespace globalvar;

using namespace std;
using namespace sf;

struct CreativeMode
{
private:
	//variables:
	RenderWindow* sidewindow;
	VideoMode videomode {800, 600};
	Vector2i window_pos{ 0,0 };
	Vector2i mouse_pos = { 0, 0 }, current_tile = { 0,0 };
	vector<Texture*>* textures;
	Sprite* Tex;
	Event event;
	State::tex_tile* picked_tile;
	RectangleShape hover_rect, selected_rect, select_rect, highlight_rect;
	Text saved_text;
	Texture notification_tex;
	Sprite notification_BG;
	unsigned int a = 0, b = 0;
	int curr_tex_set = 0, saved_delay = 0;
	float scale = 1.15;
	bool selecting = 0;
	State::sheet_properties* tile_props;
	short sheets_no = 0; 
	short& active_highlight;
	bool &hitbox, &destroyable, &view_layers, &blocked, &opaque;
	Color& highlight_color;

	//private functions:
	void change_tex();
	void save_props();
	void grid_lines();
	void initial_rectangles();
	void hover_tile();
	void selected();
	void selection();
	void highlight();

public:
	//constructors/destructors
	CreativeMode(vector<Texture*>*, State::tex_tile&, State::sheet_properties[], short, short&, bool&, bool&, bool&, bool&, bool&, Color&);
	~CreativeMode();

	//public functions:
	void update();
	void render();
	void pollevent(bool&);
	void run(bool&);
};