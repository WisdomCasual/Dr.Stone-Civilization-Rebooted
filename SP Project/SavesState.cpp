#include "SavesState.h"

void SavesState::render_save(RenderWindow* window, int a, int b, int c, Font font)
{
	pos_y = savesBG.getPosition().y;
	dis = savesBG.getGlobalBounds().width / 3.3;
	pos_x = savesBG.getPosition().x - dis + (dis * c);
	save[c].setTexture(*textures[1]);
	save[c].setScale(scale, scale);
	save[c].setOrigin(save[c].getGlobalBounds().width / 2.0, save[c].getGlobalBounds().height / 2.0);
	save[c].setPosition(pos_x,pos_y);

	// save_intersection->mouse
	if (save[c].getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		if (Mouse::isButtonPressed(Mouse::Left)) {
			save[c].setTexture(*textures[2]);
		}

		else {
			save[c].setTexture(*textures[3]);
		}
	}
	window->draw(save[c]);
	if (save_empty[c] == 0) {
		Text text;
		text.setFillColor(Color::Black);
		text.setFont(font);
		text.setString("Empty Save");
		text.setPosition(a + 45, b + 275);
		text.setCharacterSize(60);
		window->draw(text);
	}
	else {
		string c1 = "Save3";
		if (c == 0) {
			c1 = "Save1";
		}
		else if (c == 1) {
			c1 = "Save2";
		}
		ifstream ifs("Saves/" + c1 + ".ini");
		if (ifs.is_open()) {
			Sprite character;
			string name ,level ;
			int chara;
			ifs >> name;
			ifs >> chara;
			ifs >> level;
			ifs.close();
			//draw_text
			dis = save[c].getGlobalBounds().height / 12.0;
			draw_text(window,name, pos_x ,pos_y-dis*4.5, 55 * scale);
			draw_text(window, "Re_Civilization Level:", pos_x, pos_y+dis*.45, 40 * scale);
			draw_text(window,"Progress", pos_x, pos_y+dis*3.5, 45*scale);
			draw_level(window, "Electirc", pos_x, pos_y+dis*1.3, 50 * scale, font);
			draw_level(window, "Generator", pos_x, pos_y+dis*2, 50 * scale, font);
			draw_text(window,level+"%" , pos_x, pos_y+dis*4.3, 42 * scale);
			//draw_delete
			Delete[c].setFillColor(Color(128, 0, 0));
			Delete[c].setFont(font);
			Delete[c].setString("Delete");
			Delete[c].setCharacterSize(45 * scale);
			Delete[c].setOrigin(Delete[c].getGlobalBounds().width / 2.0, Delete[c].getGlobalBounds().height / 2.0);
			if (Delete[c].getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))){
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					Delete[c].setFillColor(Color(255, 0, 0)); del_presse[c] = 1;
				}
				else {
					if (del_presse[c] == 1) {
						//remove("Saves/" + c1 + ".ini");
						del_presse[c] = 0;
						save_empty[c] = 0;
					}
				}
				
				}
			Delete[c].setPosition(pos_x, pos_y+dis*6.5);
			window->draw(Delete[c]);
			//draw_character
			/*character.setTexture(*textures[chara]);
			character.setScale(.3*scale,.3*scale);
			character.setOrigin(character.getGlobalBounds().width / 2.0, character.getGlobalBounds().height / 2.0);
			character.setPosition(pos_x,pos_y-dis*2.8);
			window->draw(character);*/
			//cout << character.getGlobalBounds().width <<"  "<<character.getLocalBounds().width<<"\n";


		}
	}
}

void SavesState::render_arrow(RenderWindow* window)
{
	dis = savesBG.getGlobalBounds().width / 3.5;
	pos_x = savesBG.getPosition().x -dis*1.5;
	pos_y = save[0].getPosition().y - (save[0].getGlobalBounds().height / 2.0 + arrow.getGlobalBounds().height);
	arrow.setTexture(*textures[5]);
	arrow.setScale(scale, scale);
	arrow.setOrigin(arrow.getGlobalBounds().width / 2.0, arrow.getGlobalBounds().height / 2.0);
	arrow.setPosition(pos_x,pos_y);
	// arrow_intersection->mouse
		if (arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			arrow.setTexture(*textures[6]);
			if (Mouse::isButtonPressed(Mouse::Left)) {
				bob=2;
				back_ground=1;
				main_menu=1;
			}
		}
		window->draw(arrow);
}

void SavesState::render_savesBG(RenderWindow* window,Font font)
{
	// render_rec
	RectangleShape rec(Vector2f(0, 0));
	rec.setSize(Vector2f(window->getSize().x, window->getSize().y));
	rec.setFillColor(Color(0, 0, 0, 153));
	window->draw(rec);
	//render_savesBG
	savesBG.setTexture(*textures[4]);
	savesBG.setScale(scale, scale);
	savesBG.setOrigin(savesBG.getGlobalBounds().width/ 2.0, savesBG.getGlobalBounds().height / 2.0);
	savesBG.setPosition(window->getSize().x/2.0, window->getSize().y / 2.0);
	window->draw(savesBG);
	//cout << savesBG.getPosition().x << " " << savesBG.getOrigin().x <<" "<< window->getSize().x << "\n";
	// text
	pos_x = savesBG.getPosition().x;
	pos_y = arrow.getPosition().y-arrow.getGlobalBounds().height/2.0;
	draw_text(window, "Choose save file", pos_x, pos_y, 60 * scale);
	  
}

void SavesState::update_save(RenderWindow*window, int c)
{
	if (save_empty[c] == 0) {
			if ((save[c].getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window))))&&Mouse::isButtonPressed(Mouse::Left)) {
					bob = 2;
					//map_builder = 1;
					save_empty[c] = 1;
			}
		}
	else {
		if ((save[c].getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) && Mouse::isButtonPressed(Mouse::Left)) {
			bob = 2;
			//map_builder = 1;
		}
	}
}

void SavesState::update_arrow(RenderWindow* window)
{
	if (arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		if (Mouse::isButtonPressed(Mouse::Left)) {
			bob = 2;
			//back_ground = 1;
			//main_menu = 1;
		}
	}
}

void SavesState::draw_text(RenderWindow* window,string tex, int a, int b, int c)
{
	text.setString(tex);
	text.setCharacterSize(c);
	text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().top + text.getLocalBounds().height / 2.0);
	text.setPosition(a, b);
	window->draw(text);
}

void SavesState::draw_level(RenderWindow* window, string tex, int a, int b , int c, Font font)
{
	Text text;
	text.setFillColor(Color(170,170,170));
	text.setFont(font);
	text.setString(tex);
	text.setCharacterSize(c);
	text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().height / 2.0);
	text.setPosition(a, b);
	window->draw(text);
}

void SavesState::update_saves(RenderWindow* window)
{
	del.setOrigin(del.getLocalBounds().width / 2.0, del.getLocalBounds().top + del.getLocalBounds().height / 2.0);
	savesBG.setPosition(x, y);
	savesBG.setScale(scale * 3.1, scale * 3.1);
	save_bg.setScale(scale, scale);
	for (int i = 0; i < 3; i++) {
		save_bg.setTextureRect(IntRect(0, saves[i].pressed * 100, 100, 200));
		save_bg.setPosition(x + saves[i].x * scale, y + saves[i].y * scale);
		if (save_bg.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			if (Mouse::isButtonPressed(Mouse::Left))saves[i].pressed = 1;
			else {
				if (saves[i].pressed)
					if (saves[i].empty)
						saves[i].empty = 0;
						saves[i].excute = 1;
				saves[i].pressed = 0;
			}
			saves[i].hover = 1;
		}
		else {
			saves[i].pressed = 0;
			saves[i].hover = 0;
		}
		del.setPosition(x + saves[i].x * scale, y + (saves[i].y + 115) * scale);
		if (del.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			if (Mouse::isButtonPressed(Mouse::Left))saves[i].del_pressed = 1;
			else {
				if (saves[i].del_pressed) {
					saves[i].empty = 1;
					//delete save //////////////////
				}
				saves[i].del_pressed = 0;
			}
			saves[i].del_hover = 1;
		}
		else {
			saves[i].del_pressed = 0;
			saves[i].del_hover = 0;
		}
	}
}
void SavesState::render_saves(RenderWindow* window)
{
	window->draw(savesBG);
	text.setFillColor(Color::Black);
	draw_text(window, "Choose Save File", x, y - 120 * scale, 20 * scale);

	del.setCharacterSize(16.69 * scale);
	for (int i = 0; i < 3; i++) {
		if (saves[i].pressed)
			save_bg.setTextureRect(IntRect(200, 0, 100, 200));
		else if (saves[i].hover)
			save_bg.setTextureRect(IntRect(100, 0, 100, 200));
		else
			save_bg.setTextureRect(IntRect(0, 0, 100, 200));

		save_bg.setPosition(x + saves[i].x * scale, y + saves[i].y * scale);
		if (saves[i].del_pressed)
			del.setScale(0.95,0.95);
		else
			del.setScale(1,1);

		if (saves[i].del_hover)
			del.setFillColor(Color::Red);
		else 
			del.setFillColor(Color(164, 0, 0));

		window->draw(save_bg);
		
		if (saves[i].empty) {
			text.setFillColor(Color::Black);
			draw_text(window, "Empty Save", x + saves[i].x * scale, y + saves[i].y * scale, 20 * scale);
		}
		else {
			del.setPosition(x + saves[i].x * scale, y + saves[i].y * scale + 115 * scale);
			window->draw(del);
		}
	}
}

SavesState::SavesState()
{
	initial_fps();
	//loads "saves" textures
	initial_textures("saves");

	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));
	savesBG.setTexture(*textures[1]);
	savesBG.setOrigin(savesBG.getGlobalBounds().width / 2, savesBG.getGlobalBounds().height / 2);
	save_bg.setTexture(*textures[0]);
	save_bg.setTextureRect(IntRect(0,0,100,200));
	save_bg.setOrigin(50, 100);
	text.setFont(font);
	del.setFont(font);
	del.setString("Delete");
}

SavesState::~SavesState()
{
}

void SavesState::update(float dt, RenderWindow* window, int* terminator, deque<State*>* states)
{
	float win_x = window->getSize().x, win_y = window->getSize().y;
	x = win_x / 2, y = win_y / 2;
	scale = min(win_x / 500.0, win_y / 300.0);
	
	tint.setSize({ win_x, win_y });
	update_saves(window);

	update_arrow(window);
	update_save(window, 0);
	update_save(window, 1);
	update_save(window, 2);

	if (fps_active)
		calc_fps(dt);
}

void SavesState::render(RenderWindow* window)
{

	window->draw(tint);
	render_saves(window);



	if (fps_active)
		window->draw(fps_text);
}

void SavesState::pollevent(Event event, RenderWindow* window)
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			window->close(); break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				window->close(); break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			}
		}
	}
}
