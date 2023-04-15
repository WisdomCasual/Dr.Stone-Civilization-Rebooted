#include "SettingsState.h"
#define sliderconst 135.0

void SettingsState::update_arrow()
{
	back_arrow.setPosition(x - 35 * scale, y - 35 * scale);
	if (back_arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		back_arrow.setTextureRect(IntRect(22, 0, 22, 21));
		back_arrow.setScale(scale * 0.3, scale * 0.3);
		if (Mouse::isButtonPressed(Mouse::Left) && back_arrow.getGlobalBounds().contains(clicked_on)) {
			arrow_pressed = 1;
			back_arrow.setTextureRect(IntRect(44, 0, 22, 21));
			back_arrow.setScale(scale * 0.27, scale * 0.27);
		}
		else {
			if (arrow_pressed) {
				arrow_pressed = 0;
				game.windowbounds = resolutions[resolution];
				game.framelimit = framelimits[framelimit];
				game.update_window();

				if (states->find(MapBuilderID) == states->end() && states->find(GameID) == states->end() && states->find(WorldMapID) == states->end()) {
					states->insert(MainMenuST);
				}
				else {
					states->insert(PauseST);
				}


				int exceptions[] = { MainMenuID, BackgroundID, PauseID, MapBuilderID, GameID, WorldMapID };
				game.erase_states(exceptions, 6);
				return;
			}
		}
	}
	else {
		arrow_pressed = 0;
		back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
		back_arrow.setScale(scale * 0.3, scale * 0.3);
	}
}

void SettingsState::dev_button()
{
	if (scale != previous_scale) {
		previous_scale = scale;
		devbutton.setScale(scale * 0.2, scale * 0.2);
		devbutton.setPosition(x + 35 * scale, y + 35 * scale);
		devtext.setCharacterSize(4.45 * scale);
		devtext.setOrigin(devtext.getLocalBounds().width / 2.0, devtext.getLocalBounds().top + devtext.getLocalBounds().height / 2.0);
	}
	devtext.setPosition(x + 35 * scale, y + 35 * scale - 2 * 0.2 * scale);
	devbutton.setTextureRect(IntRect(0, 0, 45, 49));
	if (devbutton.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		if (Mouse::isButtonPressed(Mouse::Left) && devbutton.getGlobalBounds().contains(clicked_on)) {
			button_pressed = 1;
			devtext.setPosition(x + 35 * scale, y + 35.4 * scale);
			devbutton.setTextureRect(IntRect(45, 0, 45, 49));
		}
		else {
			if (button_pressed) {
				button_pressed = 0;

				states->insert(WorldMapST_admin);
				states->at(WorldMapID)->update();
				states->at(WorldMapID)->render();

				int exceptions[] = { WorldMapID , BackgroundID };
				game.erase_states(exceptions, 2);
				return;
			}
			devbutton.setTextureRect(IntRect(0, 0, 45, 49));
			devtext.setPosition(x + 35 * scale, y + 34.6 * scale);
		}
		devtext.setFillColor(Color::White);
	}
	else {
		button_pressed = 0;
		devtext.setFillColor(Color(200, 200, 200));
	}
}

void SettingsState::update_slider(slider_info* sliders, int target)
{
	if (!sliders[target].disabled) {
		if (Mouse::isButtonPressed(Mouse::Left)) {

			tip.setScale(scale / 3, scale / 3);
			tip.setTextureRect(tipsleft[sliders[target].color]);
			tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
			tip.setPosition(x + sliders[target].x * (scale / 3), y + sliders[target].y * scale);
			if (tip.getGlobalBounds().contains(clicked_on))
				sliders[target].presssed = 1;


			tip.setScale(sliderconst / 18 * (scale / 3), (scale / 3));
			tip.setTextureRect(mids[3]);
			tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
			tip.setPosition(x + (sliders[target].x + 9) * (scale / 3), y + sliders[target].y * scale);
			if (tip.getGlobalBounds().contains(clicked_on))
				sliders[target].presssed = 1;


			tip.setScale(scale / 3, scale / 3);
			tip.setTextureRect(tipsright[3]);
			tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
			tip.setPosition(x + (sliders[target].x + 9 + sliderconst) * (scale / 3), y + sliders[target].y * scale);
			if (tip.getGlobalBounds().contains(clicked_on))
				sliders[target].presssed = 1;
		}
		else
			sliders[target].presssed = 0;
		if (sliders[target].presssed) {
			float initpos = x + (sliders[target].x + 9) * (scale / 3), mxlen = sliderconst * (scale / 3), stepsize = mxlen / sliders[target].mx;
			if (mouse_pos.x < initpos) sliders[target].tipx = initpos;
			else if (mouse_pos.x > initpos + mxlen) sliders[target].tipx = initpos + mxlen;
			else
			{
				sliders[target].tipx = initpos + (sliders[target].mx - round((initpos + mxlen - mouse_pos.x) / stepsize)) * stepsize;
			}

			*sliders[target].linker = round(((sliders[target].tipx - x) / (scale / 3) - sliders[target].x - 9) / sliderconst * sliders[target].mx);
			sliders[target].midscale = (sliders[target].tipx - initpos) / (18 * (scale / 3));
		}
	}
}

void SettingsState::render_slider(int target)
{
	//background

	tip.setScale(sliderconst / 18 * (scale / 3), (scale / 3));
	tip.setTextureRect(mids[3]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(x + (sliders[target].x + 9) * (scale / 3), y + sliders[target].y * scale);
	window->draw(tip);

	tip.setScale(scale / 3, scale / 3);
	tip.setTextureRect(tipsright[3]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(x + (sliders[target].x + 9 + sliderconst) * (scale / 3), y + sliders[target].y * scale);
	window->draw(tip);

	//foreground
	color = tip.getColor();
	if (sliders[target].disabled)
		tip.setColor(Color(color.r-100, color.g-100, color.b-100));

	tip.setScale(scale / 3, scale / 3);
	tip.setTextureRect(tipsleft[sliders[target].color]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(x + sliders[target].x * (scale / 3), y + sliders[target].y * scale);
	window->draw(tip);

	tip.setScale(sliders[target].midscale * (scale / 3), (scale / 3));
	tip.setTextureRect(mids[sliders[target].color]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(x + (sliders[target].x + 9) * (scale / 3), y + sliders[target].y * scale);
	window->draw(tip);

	tip.setScale(scale / 3, scale / 3);
	tip.setTextureRect(tipsright[sliders[target].color]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(sliders[target].tipx, y + sliders[target].y * scale);

	window->draw(tip);

	slider_text.setCharacterSize(5.5 * scale);
	slider_text.setString(sliders[target].name);
	slider_text.setPosition(x + sliders[target].x * (scale / 3), y + (sliders[target].y - 11) * scale);
	window->draw(slider_text);

	slider_text.setCharacterSize(5 * scale);
	if (sliders[target].text_type == 0)
		slider_text.setString(to_string(*sliders[target].linker));
	else if (sliders[target].text_type == 1)
		slider_text.setString(to_string(*sliders[target].linker) + "%");
	else
		slider_text.setString(sliders[target].txt[*sliders[target].linker]);
	slider_text.setPosition(x + (sliders[target].x + 9 + sliderconst + 15) * (scale / 3), y + (sliders[target].y - 4) * scale);
	window->draw(slider_text);

	tip.setColor(color);
}

void SettingsState::settings_intializer()
{
	//read current settings
	if (window->getSize().x / 120.0 < window->getSize().y / 120.0) scale = window->getSize().x / 120.0;
	else scale = window->getSize().y / 120.0;
	x = window->getSize().x / 2;
	y = window->getSize().y / 2;
	
	//initialize resolution data
	for (int i = 0; i < resnum; i++) {
		if (game.windowbounds == resolutions[i]) {
			resolution = i;
			break;
		}
	}
	if (resolution == resnum)
		resolutions[resnum] = game.windowbounds, resolutions_text[resnum] = to_string(game.windowbounds.width) + " x " + to_string(game.windowbounds.height);

	for (int i = 0; i < resnum; i++)
		resolutions_text[i] = to_string(resolutions[i].width) + " x " + to_string(resolutions[i].height);



	//initialize framelimit data
	for (int i = 0; i < framelimnum; i++) {
		if (game.framelimit == framelimits[i]) {
			framelimit = i;
			break;
		}
	}

	if (framelimit == framelimnum)
		framelimits[framelimnum] = game.framelimit, framelimits_text[framelimnum] = to_string(game.framelimit);

	for (int i = 0; i < framelimnum; i++)
		framelimits_text[i] = to_string(framelimits[i]);


	//intiliazie slider positions
	float percentage = 1;
	for (int i = 0; i < 4; i++) {
		percentage = (*sliders[i].linker < sliders[i].mx) ? *sliders[i].linker / (float)sliders[i].mx : 1.0;
		sliders[i].tipx = x + (sliders[i].x + 9 + percentage * sliderconst) * (scale / 3);
		sliders[i].midscale = (sliders[i].tipx - (x + (sliders[i].x + 9) * (scale / 3))) / (18 * (scale / 3));
	}
}

void SettingsState::update_checkbox(int i)
{
	checkbox.setScale(scale / 8.0, scale / 8.0);
	checkmark.setScale(scale/ 3.5, scale / 3.5);

	checkbox.setPosition(x + checkboxes[i].x * (scale / 3) + (45 / 2) * (scale / 8.0), y + checkboxes[i].y * scale);
	if (checkbox.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		if (Mouse::isButtonPressed(Mouse::Left) && checkbox.getGlobalBounds().contains(clicked_on)) checkboxes[i].pressed = 1;
		else {
			if (checkboxes[i].pressed) {
				if (checkboxes[i].checked)
					checkboxes[i].execute = 0;
				else
					checkboxes[i].execute = 1;
				checkboxes[i].checked = !checkboxes[i].checked;
				checkboxes[i].pressed = 0;
				game.update_window();
				settings_intializer();
			}
		}
		checkboxes[i].hover = 1;
	}
	else {
		checkboxes[i].pressed = 0;
		checkboxes[i].hover = 0;
	}
}

void SettingsState::render_checkbox(int i)
{
	checkbox.setPosition(x + checkboxes[i].x * (scale / 3) + (45/2) * (scale/8.0), y + checkboxes[i].y * scale);
	checkmark.setPosition(x + checkboxes[i].x * (scale / 3) + (45 / 2) * (scale / 8.0), y + checkboxes[i].y * scale - 2 * (scale / 8.0));

	if (checkboxes[i].pressed){
		checkbox.setTextureRect(IntRect(45, 0, 45, 49));
		checkmark.setPosition(x + checkboxes[i].x * (scale / 3) + (45 / 2) * (scale / 8.0), y + checkboxes[i].y * scale + 2 * (scale / 8.0));
	}
	else if (checkboxes[i].hover){
		checkbox.setTextureRect(IntRect(0, 0, 45, 49));
	}
	else {
		checkbox.setTextureRect(IntRect(0, 0, 45, 49));
	}
	window->draw(checkbox);

	if(checkboxes[i].checked)
		window->draw(checkmark);

	draw_text(checkboxes[i].name, x + (checkboxes[i].x + 40) * (scale / 3) + (45 / 2) * (scale / 8.0), y + checkboxes[i].y * scale, 5.5 * scale);
}

SettingsState::SettingsState()
{
	initial_textures("settings");
	tissue.setTexture(*textures[3]);
	tissue.setOrigin(700 / 2, 700 / 2);
	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));

	back_arrow.setTexture(*textures[1]);
	back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
	back_arrow.setOrigin(22 / 2, 21 / 2);

	if (states->find(MapBuilderID) != states->end() || states->find(GameID) != states->end() || states->find(WorldMapID) != states->end())
		dev_button_active = 0;
	else {
		devbutton.setTexture(*textures[2]);
		devbutton.setTextureRect(IntRect(0, 0, 45, 49));
		devbutton.setOrigin(45 / 2, 49 / 2);
		devtext.setFont(font);
		devtext.setString("Dev");
		devtext.setCharacterSize(40);
		devtext.setFillColor(Color(200, 200, 200));
	}

	slider_text.setFont(font);
	slider_text.setCharacterSize(50);
	slider_text.setFillColor(Color::Black);

	tip.setTexture(*textures[0]);

	checkboxes[0].checked = fullscreen;
	checkboxes[1].checked = vsync;

	checkbox.setTexture(*textures[2]);
	checkbox.setTextureRect(IntRect(0, 0, 45, 49));
	checkbox.setOrigin(45 / 2, 49 / 2);

	checkmark.setTexture(*textures[0]);
	checkmark.setTextureRect(checkMark);
	checkmark.setOrigin(16 / 2, 15 / 2);


	settings_intializer();
}

SettingsState::~SettingsState()
{

}

void SettingsState::update()
{

	win_x = window->getSize().x, win_y = window->getSize().y;
	x = win_x / 2, y = win_y / 2;
	if (win_x / 120.0 < win_y / 120.0) scale = win_x / 120.0;
	else scale = win_y / 120.0;
	mouse_pos = window->mapPixelToCoords(Mouse::getPosition(*window));
	/////// Do Not touch;

	tint.setSize({ win_x, win_y });

	tissue.setPosition(x, y);
	tissue.setScale(scale * 0.125, scale * 0.125);

	if (dev_button_active)
		dev_button();

	update_arrow();

	if (fullscreen)
		sliders[0].disabled = 1;
	else
		sliders[0].disabled = 0;

	if (vsync)
		sliders[1].disabled = 1;
	else
		sliders[1].disabled = 0;


	for (int i = 0; i < 4; i++)
		update_slider(sliders, i);

	for (int i = 0; i < 2; i++)
		update_checkbox(i);
}

void SettingsState::render()
{

	window->draw(tint);
	window->draw(tissue);
	if (dev_button_active) {
		window->draw(devbutton);
		window->draw(devtext);
	}
	window->draw(back_arrow);
	text.setFillColor(Color::Black);
	draw_text("Settings", x, y - 35 * scale, 6.5 * scale);
	for (int i = 0; i < 4; i++)
		render_slider(i);
	for (int i = 0; i < 2; i++)
		render_checkbox(i);
}

void SettingsState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			window->close(); break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				if (states->find(MapBuilderID) == states->end() && states->find(GameID) == states->end() && states->find(WorldMapID) == states->end()) {
					states->insert(MainMenuST);
				}
				else {
					states->insert(PauseST);
				}
				{
				int exceptions[] = { MainMenuID, BackgroundID, PauseID, MapBuilderID, GameID, WorldMapID };
				game.erase_states(exceptions, 6);
				return;
				}
				break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F11:
				fullscreen = !fullscreen; 
				checkboxes[0].checked = fullscreen;
				game.update_window();
				settings_intializer();
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