#include "NewSaveState.h"



void NewSaveState::update_arrow()
{
	back_arrow.setPosition(x - 112  * scale, y - 104 * scale);
	if (back_arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		back_arrow.setTextureRect(IntRect(22, 0, 22, 21));
		back_arrow.setScale(scale * 1.2, scale * 1.2);
		if (Mouse::isButtonPressed(Mouse::Left) && back_arrow.getGlobalBounds().contains(clicked_on)) {
			arrow_pressed = 1;
			back_arrow.setTextureRect(IntRect(44, 0, 22, 21));
			back_arrow.setScale(scale * 1.08, scale * 1.08);
		}
		else {
			if (arrow_pressed) {
				arrow_pressed = 0;
				states->insert(SavesST);
				states->at(SavesID)->update();
				
				if (states->find(BackgroundID) == states->end())
					states->insert(BackgroundST);

				int exceptions[] = { SavesID , BackgroundID };
				game.erase_states(exceptions, 2);
				destruct = 1;
				return;
			}
		}

	}
	else {
		arrow_pressed = 0;
		back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
		back_arrow.setScale(scale * 1.2, scale * 1.2);
	}
}

void NewSaveState::update_buttons()
{
	if (!txt_box.empty() && selected) {
		buttontex.setColor(button_color);
		buttontex.setPosition(x + confirm.x * scale / 3.0, y + confirm.y * scale / 3.0);
		if (buttontex.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			if (Mouse::isButtonPressed(Mouse::Left) && buttontex.getGlobalBounds().contains(clicked_on))confirm.pressed = 1;
			else {
				if (confirm.pressed)
					confirm.execute = 1;
				confirm.pressed = 0;
			}
			confirm.hover = 1;
		}
		else {
			confirm.pressed = 0;
			confirm.hover = 0;
		}
	}
	else {
		buttontex.setColor(Color(button_color.r - 100, button_color.g - 100, button_color.b - 100));
	}
}

void NewSaveState::render_buttons()
{
	buttontex.setScale(scale / 1.0, scale / 1.0);
	button_text.setCharacterSize(23.69 * scale);
	buttontex.setTextureRect(IntRect(0, confirm.pressed * 49, 108, 49));
	buttontex.setPosition(x + confirm.x * scale / 3.0, y + confirm.y * scale / 3.0);
	button_text.setString(confirm.txt);
	FloatRect bounds = button_text.getLocalBounds();
	button_text.setOrigin(bounds.width / 2.0, bounds.top + bounds.height / 2.0);
	button_text.setPosition(x + confirm.x * scale / 3.0, (confirm.pressed) ? y + confirm.y * scale / 3.0 + 2 * scale / 1.0 : y + confirm.y * scale / 3.0 - 2 * scale / 1.0);
	if (confirm.hover)button_text.setFillColor(Color::White);
	else if (txt_box.empty() || !selected)
		button_text.setFillColor(Color(120, 120, 120));
	else
		button_text.setFillColor(Color(200, 200, 200));

	window->draw(buttontex);
	window->draw(button_text);
}

void NewSaveState::update_characters()
{
	//characters update
	for (int i = 0; i < 3; i++) {
		characters.setTextureRect({ i * 64,0,64,64 });
		characters.setPosition(x - 60 * scale + 60 * scale * i, y + 20 * scale);
		if (characters.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			if (Mouse::isButtonPressed(Mouse::Left) && characters.getGlobalBounds().contains(clicked_on))pressed = 1;
		}
		if (pressed)
		{
			pressed = 0;  selected = i + 1;
		}
		else
		{
			pressed = 0;
		}
	}
}

void NewSaveState::render_characters()
{
	//charaters render
	for (int i = 0; i < 3; i++) {
		characters.setTextureRect({ i * 64,0,64,64 });
		characters.setPosition(x - 60 * scale + 60 * scale * i, y + 20 * scale);
		if (i + 1 == selected)
		{
			characters.setColor({ og_color.r,og_color.g,og_color.b });
		}
		else
		{
			characters.setColor({ char_color.r,char_color.g,char_color.b });
		}
		window->draw(characters);
	}
}

void NewSaveState::add_save()
{
	ofstream ofs("Saves/Save" + to_string(save_no + 1) + ".ini", ofstream::out, ofstream::trunc);

	if (ofs.is_open()) {
		ofs << test_str << '\n';
		ofs << (int)selected << '\n';
		ofs << 1 << '\n';
	}
	ofs.close();
}

NewSaveState::NewSaveState(int save_no)
{
	this->save_no = save_no;

	State::initial_textures("newsave");

	tissue.setTexture(*textures[0]);
	tissue.setOrigin(700 / 2, 700 / 2);

	panel.setTexture(*textures[5]);
	panel.setOrigin(600 / 2, 600 / 2);

	back_arrow.setTexture(*textures[1]);
	back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
	back_arrow.setOrigin(22 / 2, 21 / 2);

	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));

	win_x = window->getSize().x, win_y = window->getSize().y;
	scale = min(win_x / 1920.0, win_y / 1080.0);
	if (win_x > 1280) scale *= 0.75;
	txt_box.initializeTextBox(test_str, *textures[2], "Enter name", Vector2f(win_x / 2.0, (win_y / 2) + 5 * scale), scale * 1.2);

	buttontex.setTexture(*textures[4]);
	buttontex.setTextureRect(IntRect(0, 0, 108, 49));
	buttontex.setOrigin(108 / 2, 49 / 2);
	button_color = buttontex.getColor();

	button_text.setFont(font);
	button_text.setCharacterSize(50);
	button_text.setFillColor(Color(200, 200, 200));

	//characters
	characters.setTexture(*textures[3]);
	og_color = characters.getColor();
	char_color = characters.getColor();
	char_color.r -= 120;
	char_color.g -= 120;
	char_color.b -= 120;
	for (int i = 0; i < 3; i++) {
		characters.setTextureRect({ i * 64,0,64,64 });
		characters.setOrigin(32, 32);
		characters.setColor({ char_color.r,char_color.g,char_color.b });
	}
}

NewSaveState::~NewSaveState()
{

}

void NewSaveState::update()
{
	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 450.0 < win_y / 450.0) scale = win_x / 450.0;
		else scale = win_y / 450.0;

		if (win_x > 1280) scale *= 0.75;
		////////////////////

		tissue.setScale(scale * 0.4, scale * 0.4); 
		tissue.setPosition(x, y);
		panel.setScale(scale * 0.52, scale * 0.52);
		panel.setPosition(x, y);
		tint.setSize({ win_x, win_y });
		txt_box.setScale(scale * 0.6);
		txt_box.setPosition({ x, y - 52 * scale });
		characters.setScale(1.2 * scale, 1.2 * scale);
	}
	tissue.setPosition(x, y);
	update_buttons();
	if (confirmed) {
		//button functionality
		add_save();
		states->insert({ GameID, new GameState((int)selected) });
		states->at(GameID)->update();
		confirmed = 0;
		return;
	}
	update_arrow();
	if (destruct)
		return;
	txt_box.update();
	
	update_characters();
}

void NewSaveState::render()
{
	window->draw(tint);
	window->draw(panel);
	window->draw(tissue);
	txt_box.drawTextBox(window);
	window->draw(back_arrow);
	render_buttons();
	text.setFillColor(Color::Black);
	draw_text("Choose name and", x, y - 112 * scale, 26 * scale);
	draw_text("character", x, y - 112 * scale+ (20 * scale), 26 * scale);
	render_characters();
}

void NewSaveState::pollevent()
{
	while (window->pollEvent(event)) {
		txt_box.text_poll(event);
		switch (event.type) {
		case Event::Closed:
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				states->insert(SavesST);
				states->at(SavesID)->update();

				if (states->find(BackgroundID) == states->end())
					states->insert(BackgroundST);
				{
					int exceptions[] = { SavesID , BackgroundID };
					game.erase_states(exceptions, 2);
					return; 
				}
				break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
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
