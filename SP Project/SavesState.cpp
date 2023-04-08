#include "SavesState.h"

void SavesState::update_saves(RenderWindow* window)
{
	del.setOrigin(del.getLocalBounds().width / 2.0, del.getLocalBounds().top + del.getLocalBounds().height / 2.0);
	savesBG.setPosition(x, y);
	savesBG.setScale(scale * 0.33, scale * 0.33);
	save_bg.setScale(scale, scale);
	for (int i = 0; i < 3; i++) {
		save_bg.setTextureRect(IntRect(0, saves[i].pressed * 100, 100, 200));
		save_bg.setPosition(x + saves[i].x * scale, y + saves[i].y * scale);
		if (save_bg.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			if (Mouse::isButtonPressed(Mouse::Left) && save_bg.getGlobalBounds().contains(clicked_on))saves[i].pressed = 1;
			else {
				if (saves[i].pressed)
					if (saves[i].empty)
						saves[i].empty = 0;
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
			if (Mouse::isButtonPressed(Mouse::Left) && del.getGlobalBounds().contains(clicked_on))saves[i].del_pressed = 1;
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
			draw_text(window, "Empty Save", x + saves[i].x * scale, y + saves[i].y * scale, 18 * scale);
		}
		else {
			text.setFillColor(Color::Black);
			draw_text(window, saves[i].name, x + saves[i].x * scale, y - 70 * scale, 18 * scale);
			draw_text(window, "Re-Civilization Level:", x + saves[i].x * scale, y + 12 * scale, 12 * scale);
			draw_text(window, "Progress", x + saves[i].x * scale, y + 65 * scale, 14 * scale);
			draw_text(window, to_string(saves[i].progress) + "%", x + saves[i].x * scale, y + 78 * scale, 14 * scale);
			text.setFillColor(Color(170, 170, 170));
			//draw_text(window, "Electirc", pos_x, pos_y + dis * 1.3, 50 * scale);
			//draw_text(window, "Generator", pos_x, pos_y + dis * 2, 50 * scale);

		    ////////////place holder/////////
			charactertex.setTextureRect(IntRect( saves[i].character_id * (496 / 6.0), 0, 496 / 6.0, 373 / 2.0));
			charactertex.setPosition(x + saves[i].x * scale, y - 30 * scale);
			charactertex.setScale(scale * 0.35, scale * 0.35);
			window->draw(charactertex);
			/////////////////////

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

	initial_saves();
	////////////place holder/////////
	charactertex.setTexture(*textures[2]);
	charactertex.setTextureRect(IntRect(0, 0, 496 / 6.0, 373 / 2.0));
	charactertex.setOrigin(charactertex.getLocalBounds().width / 2.0, charactertex.getLocalBounds().height / 2.0);
	//////////////

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
	arrow.setTexture(*textures[3]);
	arrow.setTextureRect(IntRect(0, 0, 22, 21));
	arrow.setOrigin(22/2, 21/2);
}

SavesState::~SavesState()
{
}

void SavesState::update_arrow(RenderWindow* window, int* terminator, deque<State*>* states)
{
	arrow.setPosition(x - 180 * scale, y - 120 * scale);
	if (arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		arrow.setTextureRect(IntRect(22, 0, 22, 21));
		arrow.setScale(scale * 0.8, scale * 0.8);
		if (Mouse::isButtonPressed(Mouse::Left) && arrow.getGlobalBounds().contains(clicked_on)) {
			arrow_pressed = 1;
			arrow.setTextureRect(IntRect(44, 0, 22, 21));
			arrow.setScale(scale * 0.75, scale * 0.75);
		}
		else {
			if (arrow_pressed) {
				arrow_pressed = 0;
				*terminator += states->size();
				states->push_front(new MainMenuState);
				states->push_front(new Background);
			}
		}
		
	}
	else {
		arrow_pressed = 0;
		arrow.setTextureRect(IntRect(0, 0, 22, 21));
		arrow.setScale(scale * 0.8, scale * 0.8);
	}
}

void SavesState::initial_saves()
{
	for (int i = 0; i < 3; i++) {
		ifstream ifs("Saves/Save" + to_string(i + 1) + ".ini");
		string line;
		if (!(ifs >> line)) {
			saves[i].empty = 1; continue;
		}
		saves[i].empty = 0;
		ifs.seekg(ios::beg);
		if (ifs.is_open()) {
			string name;
			int progress, character_id;
			ifs >> name;
			saves[i].name = name;
			ifs >> character_id;
			saves[i].character_id = character_id;
			ifs >> progress;
			saves[i].progress = progress;      //<-- will be calculated by number of missons
		}
		ifs.close();
	}
}

void SavesState::update(float dt, RenderWindow* window, int* terminator, deque<State*>* states)
{
	float win_x = window->getSize().x, win_y = window->getSize().y;
	x = win_x / 2, y = win_y / 2;
	scale = min(win_x / 570.0, win_y / 350.0);
	
	tint.setSize({ win_x, win_y });
	update_saves(window);
	update_arrow(window, terminator, states);

	if (fps_active)
		calc_fps(dt);
}

void SavesState::render(RenderWindow* window)
{

	window->draw(tint);
	render_saves(window);
	window->draw(arrow);

	if (fps_active)
		window->draw(fps_text);
}

void SavesState::pollevent(Event event, RenderWindow* window, int* terminator, deque<State*>* states)
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
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case Mouse::Left:
				clicked_on = window->mapPixelToCoords(Mouse::getPosition(*window));
				break;
			}
		}
	}
}
