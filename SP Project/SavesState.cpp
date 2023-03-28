#include "SavesState.h"

void SavesState::render_save(RenderWindow* window,int a,int b,int c,Font font)
{
	save[c].setTexture(*textures[0]);
	save[c].setScale(scale, scale);
	save[c].setPosition(a, b);
	
	// save_intersection->mouse
		if (save[c].getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			if (Mouse::isButtonPressed(Mouse::Left)) {
				save[c].setTexture(*textures[1]);
		 }

		 else {
				save[c].setTexture(*textures[2]);
		 }
		}
		window->draw(save[c]);
		string c1="Save3";
		if (c == 0) {
			c1 = "Save1";
		}
		else if (c == 1) {
			c1 = "Save2";
		}
		ifstream ifs("textures/saves/"+c1+".ini");
		if (ifs.is_open()) {
			char if_empty;
			ifs >> if_empty;
			if (if_empty == '0') {
				Text text;
				text.setFillColor(Color::Black);
				text.setFont(font);
				text.setString("Empty Save");
				text.setPosition(a+45,b+275);
				text.setCharacterSize(60);
				window->draw(text);
				//new_save
				if (save[c].getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
					if (Mouse::isButtonPressed(Mouse::Left)) {
						bob = 2;
						map_builder = 1;
					}
				}
		 }
			/*else {
				Text text;
				string name, character;
				char level;
				ifs>> name;
				ifs >> character;
				ifs >> level;


				
			}*/
		}
	
}

void SavesState::render_arrow(RenderWindow* window)
{
	arrow.setTexture(*textures[4]);
	arrow.setScale(scale, scale);
	arrow.setPosition(368, 136);
	// arrow_intersection->mouse
		if (arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			arrow.setTexture(*textures[5]);
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
	// text
	Text text;
	text.setFillColor(Color::Black);
	text.setFont(font);
	text.setString("Choose save file");
	text.setPosition(816,120);
	text.setCharacterSize(60);
	//render_savesBG
	savesBG.setTexture(*textures[3]);
	savesBG.setScale(scale, scale);
	//savesBG.setOrigin((921 ) / 2.0, (573) / 2);
	savesBG.setPosition(280,80);
	    window->draw(rec);
		window->draw(savesBG);
		window->draw(text);
}

SavesState::SavesState()
{  
	
	//loads "saves" textures
	initial_textures("saves");
}

SavesState::~SavesState()
{
}

void SavesState::update(float dt)
{
	//scale=max()

}

void SavesState::render(RenderWindow* window)
{
	Font font;
	font.loadFromFile("font.ttf");
	render_savesBG(window,font);
	render_save(window, 398, 218,0,font);
	render_save(window, 816, 218,1,font);
	render_save(window,1230, 218,2,font);
	render_arrow(window);
	/*Sprite save;
	save.setTexture(*textures["Saves design"]);
	save.setScale(scale, scale);
	save.setPosition(180, 0);
	window->draw(save);*/
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
			}
		}
	}
}
