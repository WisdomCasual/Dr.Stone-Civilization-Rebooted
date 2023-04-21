#include "CreativeMode.h"

void CreativeMode::change_tex()
{	
	delete this->Tex;
	Tex = new Sprite;
	Tex->setTexture(*textures->at(curr_tex_set));
	a = Tex->getGlobalBounds().width, b = Tex->getGlobalBounds().height;
	videomode = { a,b };
	window_pos = sidewindow->getPosition();
	delete this->sidewindow;
	sidewindow = new RenderWindow(videomode, "Texture picker", Style::Titlebar | Style::Close);
	sidewindow->clear();
	sidewindow->setPosition(window_pos);
	sidewindow->setSize({ (unsigned int)(a * scale),(unsigned int)(b * scale) });

}

void CreativeMode::save_props()
{
	for (int sheet = 0; sheet < sheets_no; sheet++) {
		ofstream ofs("textures/game/tiles/properties/sheet "+ to_string(sheet) + ".prop", ofstream::out, ofstream::trunc);
		if (ofs.is_open()) {
			for (int i = 0; i < tile_props[sheet].x_size; i++) {
				for (int j = 0; j < tile_props[sheet].y_size; j++)
					ofs << tile_props[sheet].properties[i][j].props << ' ';
				ofs << '\n';
			}
		}
		ofs.close();
	}
}

void CreativeMode::grid_lines()
{
	//display grid lines
	RectangleShape rect;
	rect.setSize(Vector2f(1, b));
	rect.setFillColor(Color::Black);
	rect.setPosition({ 0, 0 });
	for (float i = 0; i < a; i += 16) {
		rect.setPosition(i, 0);
		sidewindow->draw(rect);
	}
	rect.setSize(Vector2f(a, 1));
	for (float i = 0; i < b; i += 16) {
		rect.setPosition(0, i);
		sidewindow->draw(rect);
	}
}

void CreativeMode::initial_rectangles()
{
	//hover rectangle
	hover_rect.setSize(Vector2f(16, 16));
	hover_rect.setFillColor(Color::Transparent);
	hover_rect.setOutlineThickness(1);
	hover_rect.setOutlineColor(Color::White);

	//selected rectangle
	selected_rect.setSize(Vector2f(16, 16));
	selected_rect.setFillColor(Color::Transparent);
	selected_rect.setOutlineThickness(1);
	selected_rect.setOutlineColor(Color::Green);
	selected_rect.setPosition(Vector2f(picked_tile->x * 16, picked_tile->y * 16));

	//selection rectangle
	select_rect.setSize(Vector2f(16, 16));
	select_rect.setFillColor(Color::Transparent);
	select_rect.setOutlineThickness(1);
	select_rect.setOutlineColor(Color::Green);

}

void CreativeMode::hover_tile()
{
	hover_rect.setPosition(Vector2f( current_tile.x * 16 , current_tile.y * 16));
	sidewindow->draw(hover_rect);
}

void CreativeMode::selected()
{
	if (active_highlight & 8) {
		short prev_priority = tile_props[curr_tex_set].properties[current_tile.x][current_tile.y].props;
		if (prev_priority & 8 && prev_priority & 16) {
			tile_props[curr_tex_set].properties[current_tile.x][current_tile.y].props ^= 24;
		}
		else if (prev_priority & 8)
			tile_props[curr_tex_set].properties[current_tile.x][current_tile.y].props ^= 16;
		else
			tile_props[curr_tex_set].properties[current_tile.x][current_tile.y].props ^= 8;
	}
	else if (active_highlight)
		tile_props[curr_tex_set].properties[current_tile.x][current_tile.y].props ^= active_highlight;
	else {
		selected_rect.setPosition(Vector2f(current_tile.x * 16, current_tile.y * 16));
		picked_tile->x = current_tile.x, picked_tile->y = current_tile.y;
		picked_tile->tex_id = curr_tex_set;
	}

}

void CreativeMode::selection()
{
	if (selecting) {
		picked_tile->start_x = min(picked_tile->selection_start.x, current_tile.x), 
		picked_tile->start_y = min(picked_tile->selection_start.y, current_tile.y);
		select_rect.setPosition(16 * picked_tile->start_x,16 * picked_tile->start_y);
		picked_tile->wdth = abs(picked_tile->selection_start.x - current_tile.x) + 1, picked_tile->hght = abs(picked_tile->selection_start.y - current_tile.y) + 1;
		select_rect.setSize(Vector2f(picked_tile->wdth * 16,picked_tile->hght * 16));
		sidewindow->draw(select_rect);
		
	}
	if (picked_tile->select_done) {
		sidewindow->draw(select_rect);
	}
}

void CreativeMode::highlight()
{
	highlight_rect.setFillColor(highlight_color);
	for (int i = 0; i < tile_props[curr_tex_set].x_size; i++)
		for (int j = 0; j < tile_props[curr_tex_set].y_size; j++) {
			short prop = tile_props[curr_tex_set].properties[i][j].props;
			if (active_highlight & 8) {
				
				if (prop & 16)
					highlight_color = Color(0, 175, 0, 80);
				else if (prop & 8)
					highlight_color = Color(175, 0, 0, 80);
				else
					highlight_color = Color(0, 0, 175, 80);

				highlight_rect.setFillColor(highlight_color);
				highlight_rect.setPosition(Vector2f(i * 16, j * 16));
				sidewindow->draw(highlight_rect);
			}
			else if (prop & active_highlight) {
				highlight_rect.setPosition(Vector2f(i * 16, j * 16));
				sidewindow->draw(highlight_rect);
			}
		}
}

CreativeMode::CreativeMode(vector<Texture*>* textures, State::tex_tile& picked_tile, State::sheet_properties tile_props[], short sheets_no, short& active_highlight, bool& hitbox, bool& destroyable, bool& view_layers, bool& blocked, Color& highlight_color)
	: hitbox(hitbox), destroyable(destroyable), view_layers(view_layers), blocked(blocked), active_highlight(active_highlight), highlight_color(highlight_color)
{
	sidewindow = new RenderWindow(videomode, "Texture Picker", Style::Titlebar | Style::Close);
	sidewindow->setFramerateLimit(60);
	sidewindow->setPosition({ 0,0 });
	this->textures = textures;
	this->picked_tile = &picked_tile;
	this->tile_props = tile_props;
	this->sheets_no = sheets_no;
	curr_tex_set = picked_tile.tex_id;
	Tex = new Sprite;
	change_tex();
	initial_rectangles();

	saved_text.setFont(font);
	saved_text.setFillColor(Color::Black);
	saved_text.setString("Properties Saved\n  Successfully");
	saved_text.setPosition(27, 15);

	notification_tex.loadFromFile("textures/notification/notification_bg.png");
	notification_BG.setTexture(notification_tex);
	notification_BG.setScale(1.35, 1.35);
	notification_BG.setPosition(10,10);

	highlight_rect.setSize({ 16, 16 });
}

CreativeMode::~CreativeMode()
{
	delete this->sidewindow;
	delete this->Tex;
}

void CreativeMode::update()
{
	if ((Mouse::isButtonPressed(Mouse::Right) || Keyboard::isKeyPressed(Keyboard::LShift)) && sidewindow->hasFocus()) {
		picked_tile->previous_drawn_tile = { -1,-1 }, picked_tile->previous_erased_tile = { -1,-1 };
		if (!selecting) {
			picked_tile->selection_start = current_tile;
			picked_tile->tex_id = curr_tex_set;
		}
		selecting = 1;
		picked_tile->select_done = 0;
		picked_tile->global_select_done = 0;
	}
	else {
		if (selecting) {
			picked_tile->select_done = 1;
			picked_tile->selection_end = current_tile;
		}
		selecting = 0;
	}


	mouse_pos = Mouse::getPosition(*sidewindow);
	if(mouse_pos.x>0 && mouse_pos.x<sidewindow->getSize().x && mouse_pos.y > 0 && mouse_pos.y < sidewindow->getSize().y){
		current_tile = { int(mouse_pos.x / scale / 16), int(mouse_pos.y / scale / 16) };
	}
}

void CreativeMode::render()
{
	sidewindow->clear();
	sidewindow->draw(*Tex);
	grid_lines();
	if (!selecting && !picked_tile->select_done)
		sidewindow->draw(selected_rect);
	hover_tile();
	selection();
	if (saved_delay) {
		sidewindow->draw(notification_BG);
		sidewindow->draw(saved_text);
		saved_delay--;
	}
	if (active_highlight)
		highlight();
	sidewindow->display();
}

void CreativeMode::pollevent(bool& picker)
{				
	while (sidewindow->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			sidewindow->close(); picker = 1; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
			case Keyboard::E:
				sidewindow->close(); picker = 1; break;
			case Keyboard::O:
				window_pos = sidewindow->getPosition();
				window_pos.y -= 48;
				sidewindow->setPosition(window_pos); break;
			case Keyboard::L:
				window_pos = sidewindow->getPosition();
				window_pos.y += 48;
				sidewindow->setPosition(window_pos); break;
			case Keyboard::Equal:
				scale += 0.05;
				sidewindow->setSize({ (unsigned int)(a * scale),(unsigned int)(b * scale) });
				break;
			case Keyboard::Hyphen:
				scale -= 0.05;
				sidewindow->setSize({ (unsigned int)(a * scale),(unsigned int)(b * scale) });
				break;
			case Keyboard::Comma:

				if (curr_tex_set == 0)
					curr_tex_set = textures->size();
				--curr_tex_set;
				change_tex();
				break;
			case Keyboard::Period:
				++curr_tex_set;
				if (curr_tex_set == textures->size())
					curr_tex_set = 0;
				change_tex();
				break;
			case Keyboard::B:
				hitbox = 0, destroyable = 0, view_layers = 0;
				highlight_color = Color(0, 0, 175, 80);
				blocked = !blocked;
				active_highlight = (blocked) ? 4 : 0; break;
			case Keyboard::H:
				blocked = 0, destroyable = 0, view_layers = 0;
				highlight_color = Color(175, 0, 0, 80);
				hitbox = !hitbox;
				active_highlight = (hitbox) ? 2 : 0; break;
			case Keyboard::F:
				blocked = 0, destroyable = 0, hitbox = 0;
				view_layers = !view_layers;
				active_highlight = (view_layers) ? 8 : 0; break;
			case Keyboard::Q:
				blocked = 0, view_layers = 0, hitbox = 0;
				highlight_color = Color(0, 175, 0, 80);
				destroyable = !destroyable;
				active_highlight = (destroyable) ? 1 : 0; break;
			case Keyboard::F6:
				save_props();
				saved_delay = 300;
				break;
			}
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case Mouse::Left:
				if (sidewindow->hasFocus())
					picked_tile->previous_drawn_tile = { -1,-1 }, picked_tile->previous_erased_tile = { -1,-1 };
					picked_tile->select_done = 0; picked_tile->global_select_done = 0;
					selected(); 
					break;
			}
		}
	}
}

void CreativeMode::run(bool& picker)
{
	update();
	pollevent(picker);
	render();
}
