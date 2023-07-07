#include "SavesState.h"
#include <filesystem>

void SavesState::fade_in()
{
	if (transparency < 255) {
		if (transparency + 1500 * dt > 255)
			transparency = 255;
		else
			transparency += 1500 * dt;

		savesBG.setColor(Color(255, 255, 255, transparency));
		save_bg.setColor(Color(255, 255, 255, transparency));
		arrow.setColor(Color(255, 255, 255, transparency));
		charactertex.setColor(Color(255, 255, 255, transparency));


		if (darkness < 154) {
			if (darkness + 154 * dt * 6 > 154)
				darkness = 154;
			else
				darkness += 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, darkness));
		}
	}
}

bool SavesState::fade_out()
{
	if (transparency > 0) {
		if (transparency - 1500 * dt < 0)
			transparency = 0;
		else
			transparency -= 1500 * dt;

		savesBG.setColor(Color(255, 255, 255, transparency));
		save_bg.setColor(Color(255, 255, 255, transparency));
		arrow.setColor(Color(255, 255, 255, transparency));
		charactertex.setColor(Color(255, 255, 255, transparency));

		if (darkness > 0) {
			if (darkness - 154 * dt * 6 < 0)
				darkness = 0;
			else
				darkness -= 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, darkness));
		}
		return false;
	}
	else
		return true;
}

bool SavesState::black_out()
{
	if (blackining < 255) {
		blacking_out = true;
		if (blackining + 500 * dt > 255)
			blackining = 255;
		else
			blackining += 500 * dt;
		blackscreen.setFillColor(Color(0, 0, 0, blackining));
		return false;
	}
	return true;
}

void SavesState::update_saves()
{
	del.setOrigin(del.getLocalBounds().width / 2.0, del.getLocalBounds().top + del.getLocalBounds().height / 2.0);
	savesBG.setPosition(x, y);
	savesBG.setScale(scale * 0.33, scale * 0.33);
	save_bg.setScale(scale, scale);
	for (int i = 0; i < 3; i++) {
		save_bg.setTextureRect(IntRect(0, saves[i].pressed * 100, 100, 200));
		save_bg.setPosition(x + saves[i].x * scale, y + saves[i].y * scale);
		if (save_bg.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			clickable_cursor = true;
			saves[i].hover = 1;
			if (Mouse::isButtonPressed(Mouse::Left) && save_bg.getGlobalBounds().contains(clicked_on)) {
				if(!saves[i].pressed)
					game.play_sfx(0);
				saves[i].pressed = 1;
			}
			else {
				if (saves[i].pressed)
					if (saves[i].empty) {
						states->insert({ NewSaveID, new NewSaveState(i)});
						states->at(NewSaveID)->update();
						saves[i].hover = 0;
					}
					else {
						selected_save = i + 1;
					}
				saves[i].pressed = 0;
			}
			
		}
		else {
			saves[i].pressed = 0;
			saves[i].hover = 0;
		}
		if(!saves[i].empty){
			del.setPosition(x + saves[i].x * scale, y + (saves[i].y + 115) * scale);
			if (del.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
				clickable_cursor = true;
				if (Mouse::isButtonPressed(Mouse::Left) && del.getGlobalBounds().contains(clicked_on)) {
					if (!saves[i].del_pressed)
						game.play_sfx(0);
					saves[i].del_pressed = 1;
				}
				else {
					saves[i].del_hover = 1;

					if (saves[i].del_pressed) {
						//delete save //////////////////
						del_save_no = i + 1;
						string strings_array[] = { "Are you sure that you", "want to delete", '"' + saves[i].name + '"' };
						states->insert({ ConfirmationID, new ConfirmationState(strings_array, 3, del_save) });
						states->at(ConfirmationID)->update();
						saves[i].del_hover = 0;
					}
					saves[i].del_pressed = 0;
				}

			}
			else {
				saves[i].del_pressed = 0;
				saves[i].del_hover = 0;
			}
		}
	}
}

void SavesState::render_saves()
{
	window->draw(savesBG);
	text.setFillColor(Color(0, 0, 0, transparency));
	draw_text("Choose Save File", x, y - 120 * scale, 20 * scale);

	del.setCharacterSize(16.69 * scale);
	for (int i = 0; i < 3; i++) {
		if (saves[i].pressed)
			save_bg.setTextureRect(IntRect(200, 0, 100, 200));
		else if (saves[i].hover)
			save_bg.setTextureRect(IntRect(100, 0, 100, 200));
		else
			save_bg.setTextureRect(IntRect(0, 0, 100, 200));

		save_bg.setPosition(x + saves[i].x * scale, y + saves[i].y * scale);

		if (saves[i].del_hover)
			del.setFillColor(Color(255, 0, 0, transparency));
		else 
			del.setFillColor(Color(164, 0, 0, transparency));

		if (saves[i].del_pressed)
			del.setScale(0.95,0.95);
		else 
			del.setScale(1, 1);


		window->draw(save_bg);
		
		if (saves[i].empty) {
			text.setFillColor(Color(0, 0, 0, transparency));
			draw_text("Empty Save", x + saves[i].x * scale, y + saves[i].y * scale, 18 * scale);
		}
		else {
			text.setFillColor(Color(0, 0, 0, transparency));
			draw_text(saves[i].name, x + saves[i].x * scale, y - 70 * scale, 18 * scale);
			draw_text("Re-Civilization Level:", x + saves[i].x * scale, y + 12 * scale, 12 * scale);
			draw_text("Progress", x + saves[i].x * scale, y + 65 * scale, 14 * scale);
			draw_text(to_string(saves[i].progress) + "%", x + saves[i].x * scale, y + 78 * scale, 14 * scale);
			text.setFillColor(Color(170, 170, 170, transparency));

			draw_text(quest_names[saves[i].progress].first_line, x + saves[i].x * scale, y + 30 * scale, 14 * scale);
			draw_text(quest_names[saves[i].progress].second_line, x + saves[i].x * scale, y + 45 * scale, 14 * scale);

		    ////////////place holder/////////
			charactertex.setTextureRect(IntRect( (saves[i].character_id-1) * 64, 0, 64, 64));
			charactertex.setPosition(x + saves[i].x * scale, y - 30 * scale);
			charactertex.setScale(scale * 0.95, scale * 0.95);
			window->draw(charactertex);
			/////////////////////

			del.setPosition(x + saves[i].x * scale, y + saves[i].y * scale + 115 * scale);
			window->draw(del);
		}
	}
}

SavesState::SavesState()
{
	//loads "saves" textures
	initial_textures("saves");

	initial_saves();
	////////////place holder/////////
	charactertex.setTexture(*textures[2]);
	charactertex.setTextureRect(IntRect(0, 0, 64, 64));
	charactertex.setOrigin(charactertex.getLocalBounds().width / 2.0, charactertex.getLocalBounds().height / 2.0);
	//////////////

	tint.setSize({ 1920, 1080 });

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

void SavesState::update_arrow()
{
	arrow.setPosition(x - 180 * scale, y - 120 * scale);
	arrow.setScale(scale * 0.8, scale * 0.8);
	if (arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		clickable_cursor = true;
		arrow.setTextureRect(IntRect(22, 0, 22, 21));
		if (Mouse::isButtonPressed(Mouse::Left) && arrow.getGlobalBounds().contains(clicked_on)) {
			if(!arrow_pressed)
				game.play_sfx(1);
			arrow_pressed = 1;
			arrow.setTextureRect(IntRect(44, 0, 22, 21));
			arrow.setScale(scale * 0.75, scale * 0.75);
		}
		else {
			if (arrow_pressed) {
				arrow_pressed = 0;
				back = true;
			}
		}
		
	}
	else {
		arrow_pressed = 0;
		arrow.setTextureRect(IntRect(0, 0, 22, 21));
	}
}

void SavesState::initial_saves()
{
	for (int i = 0; i < 3; i++) {
		ifstream ifs("Saves/Save " + to_string(i + 1) + "/Save.dat");
		string line;
		if (!(ifs >> line)) {
			saves[i].empty = 1; continue;
		}
		saves[i].empty = 0;
		ifs.seekg(ios::beg);
		if (ifs.is_open()) {
			string name;
			int progress, character_id;
			getline(ifs, name);
			saves[i].name = name;
			ifs >> character_id;
			saves[i].character_id = character_id;
			ifs >> progress;
			saves[i].progress = progress / quests_no * 100;      //<-- calculated by number of missons
		}
		ifs.close();
	}
}

void SavesState::update()
{
	float win_x = window->getSize().x, win_y = window->getSize().y;
	x = win_x / 2, y = win_y / 2;
	scale = min(win_x / 570.0, win_y / 350.0);
	if (win_x > 1280) scale *= 0.9;

	tint.setSize({ win_x, win_y });
	blackscreen.setSize({ win_x, win_y });

	if (del_save) {
		filesystem::directory_entry dnd("Saves/Save " + to_string(del_save_no) + "\\DO NOT DELETE THIS.DND");
		for (auto& path : filesystem::directory_iterator("Saves/Save " + to_string(del_save_no))) {
			if(path != dnd)
				filesystem::remove_all(path);
		}
		saves[del_save_no-1].empty = 1;
		del_save = 0;
	}

	
	if (selected_save) {
		if (black_out()) {
			game.music.stop();
			states->insert({ GameID, new GameState(selected_save - 1) });
			states->at(GameID)->update();
			selected_save = 0;
			int exceptions[] = { GameID };
			game.erase_states(exceptions, 1);
			return;
		}
	}
	if (back) {
		if (fade_out()) {
			back = false;

			states->insert(MainMenuST);
			states->at(MainMenuID)->update();

			if (states->find(BackgroundID) == states->end()) {
				states->insert(BackgroundST);
				states->at(BackgroundID)->update();
			}

			int exceptions[] = { MainMenuID , BackgroundID };
			game.erase_states(exceptions, 2);
			return;
		}
	}
	else
		fade_in();

	update_saves();
	update_arrow();

}

void SavesState::render()
{
	
	window->draw(tint);
	render_saves();
	window->draw(arrow);
	if (blacking_out)
		window->draw(blackscreen);
}

void SavesState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				back = true;
				break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			}
			break;
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case Mouse::Left:
				clicked_on = window->mapPixelToCoords(Mouse::getPosition(*window));
				break;
			}
		}
	}
}
