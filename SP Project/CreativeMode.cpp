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
//display grid lines
void CreativeMode::grid_lines()
{
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
	selected_rect.setPosition(Vector2f(picked_tile->x, picked_tile->y));

}
void CreativeMode::hover_tile()
{
	hover_rect.setPosition(Vector2f( current_tile.x * 16 , current_tile.y * 16));
	sidewindow->draw(hover_rect);
}
void CreativeMode::selected()
{
	selected_rect.setPosition(Vector2f(current_tile.x * 16, current_tile.y * 16));
	picked_tile->x = current_tile.x, picked_tile->y = current_tile.y;
	picked_tile->tex_id = curr_tex_set;
}
CreativeMode::CreativeMode(vector<Texture*>* textures, State::tex_tile& picked_tile)
{
	sidewindow = new RenderWindow(videomode, "Texture Picker", Style::Titlebar | Style::Close);
	sidewindow->setPosition({ 0,0 });
	this->textures = textures;
	this->picked_tile = &picked_tile;
	change_tex();
	initial_rectangles();
}

CreativeMode::~CreativeMode()
{
	delete this->sidewindow;
	delete this->Tex;
}

void CreativeMode::update()
{
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
	sidewindow->draw(selected_rect);
	hover_tile();
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
			}
		}
		if (event.type == Event::MouseButtonPressed) {
		switch (event.key.code)
		case Mouse::Left:
			selected(); break;
		}
	}
}

void CreativeMode::run(bool& picker)
{
	update();
	pollevent(picker);
	render();
}
