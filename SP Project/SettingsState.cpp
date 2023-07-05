#include "SettingsState.h"
// length of slider (mostly used to give the size for the hollow slider)
#define sliderconst 135.0


void SettingsState::fade_in()
{
	if (transparency < 255) {
		if (transparency + 1500 * dt > 255)
			transparency = 255;
		else
			transparency += 1500 * dt;

		back_arrow.setColor(Color(255, 255, 255, transparency));
		tip.setColor(Color(255, 255, 255, transparency));
		slider_text.setFillColor(Color(0, 0, 0, transparency));
		checkbox.setColor(Color(255, 255, 255, transparency));
		checkmark.setColor(Color(255, 255, 255, transparency));
		if (dev_button_active)
			devbutton.setColor(Color(255, 255, 255, transparency));


		if (bg_fade) {
			panel.setColor(Color(255, 255, 255, transparency));
			tissue.setColor(Color(255, 255, 255, transparency));
			if (darkness < 154) {
				if (darkness + 154 * dt * 6 > 154)
					darkness = 154;
				else
					darkness += 154 * dt * 6;
				tint.setFillColor(Color(0, 0, 0, darkness));
			}
		}
	}
}

bool SettingsState::fade_out()
{
	if (transparency > 0) {
		if (transparency - 1500 * dt < 0)
			transparency = 0;
		else
			transparency -= 1500 * dt;

		back_arrow.setColor(Color(255, 255, 255, transparency));
		tip.setColor(Color(255, 255, 255, transparency));
		slider_text.setFillColor(Color(0, 0, 0, transparency));
		checkbox.setColor(Color(255, 255, 255, transparency));
		checkmark.setColor(Color(255, 255, 255, transparency));
		if (dev_button_active)
			devbutton.setColor(Color(255, 255, 255, transparency));

		if (bg_fade) {
			panel.setColor(Color(255, 255, 255, transparency));
			tissue.setColor(Color(255, 255, 255, transparency));
			if (darkness > 0) {
				if (darkness - 154 * dt * 6 < 0)
					darkness = 0;
				else
					darkness -= 154 * dt * 6;
				tint.setFillColor(Color(0, 0, 0, darkness));
			}
		}
		return false;
	}
	else
		return true;
}

void SettingsState::update_arrow()
{
	// here we save the new settings in the config file
	// also set the position for the arrow and detect for clicks and add a little animation for the click
	back_arrow.setPosition(x - 35 * scale, y - 35 * scale);
	if (back_arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		back_arrow.setTextureRect(IntRect(22, 0, 22, 21));
		back_arrow.setScale(scale * 0.3, scale * 0.3);
		if (Mouse::isButtonPressed(Mouse::Left) && back_arrow.getGlobalBounds().contains(clicked_on)) {
			if(!arrow_pressed)
				game.play_sfx(1);
			arrow_pressed = 1;
			back_arrow.setTextureRect(IntRect(44, 0, 22, 21));
			back_arrow.setScale(scale * 0.27, scale * 0.27);
		}
		else {
			// go back to previous state and apply the changes 
			if (arrow_pressed) {
				arrow_pressed = 0;
				// set the new resolution and framelimit
				game.windowbounds = resolutions[resolution];
				game.framelimit = framelimit_values[framelimit];
				//deletes the current window and draws a new window 

				if(framelimit != prev_framelimit || resolution != prev_resolution)
					game.update_window();
				else
					game.save();

				exit = true;
			}
		}
	}
	else {
		arrow_pressed = 0;
		back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
		back_arrow.setScale(scale * 0.3, scale * 0.3);
	}
}

// a button to go to map builder state but if pressed pushes the password state for authentication
void SettingsState::dev_button()
{
	// Positioning the button and it's text
	if (scale != previous_scale) {
		previous_scale = scale;
		devbutton.setScale(scale * 0.2, scale * 0.2);
		devbutton.setPosition(x + 35 * scale, y + 35 * scale);
		devtext.setCharacterSize(4.45 * scale);
		devtext.setOrigin(devtext.getLocalBounds().width / 2.0, devtext.getLocalBounds().top + devtext.getLocalBounds().height / 2.0);
	}
	devtext.setPosition(x + 35 * scale, y + 35 * scale - 2 * 0.2 * scale);
	devbutton.setTextureRect(IntRect(0, 0, 45, 49));
	// detect for clicks
	if (devbutton.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		if (Mouse::isButtonPressed(Mouse::Left) && devbutton.getGlobalBounds().contains(clicked_on)) {
			if(!button_pressed)
				game.play_sfx(0);
			button_pressed = 1;
			devtext.setPosition(x + 35 * scale, y + 35.4 * scale);
			devbutton.setTextureRect(IntRect(45, 0, 45, 49));
		}
		else {
			if (button_pressed) {
				// pushing the password state 
				button_pressed = 0;
				devtext.setFillColor(Color(200, 200, 200, transparency));

				states->insert(PasswordST);
				states->at(PasswordID)->update();
				int exceptions[] = { SettingsID , BackgroundID, PasswordID };
				game.erase_states(exceptions, 3);

				return;
			}
			devbutton.setTextureRect(IntRect(0, 0, 45, 49));
			devtext.setPosition(x + 35 * scale, y + 34.6 * scale);
		}
		devtext.setFillColor(Color(255, 255, 255, transparency));
	}
	else {
		button_pressed = 0;
		devtext.setFillColor(Color(200, 200, 200, transparency));
	}
}

void SettingsState::update_slider(slider_info* sliders, int target)
{
	// what does the .disabled function do?
	// we shouldn't be able to apply changes to specific sliders in specific modes
	// for example: if fullscreen mode is enabled then resolution slider should be disabled, same with the framelimit.
	if (!sliders[target].disabled) {
		if (Mouse::isButtonPressed(Mouse::Left)) {

			/*
			Here we detect for clicks on the slider and apply changes accordingly,
			We have a sprite named tip which we overwrite according to the part we are dealing with in the slider.
			for example the left tip should have a fixed position all the time and just position it according to the current
			resolution.
			For the middle part we should scale it according to the mouse position on the x axis.
			The right tip should be moving also with the mouse position on the x axis
			Also we detect for clicks on the whole slider so slider should be movable from any part of it and if you press
			of a random part of the slider the right tip will go there and also the mid will be scaled
			*/

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
			// make sure that the slider doesn't pass it's boundries
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
	// the left tip wasn't rendered because it'll never appear in any case (Hollow slider/background)
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
	
	// first we check if the slider was disabled if so, we change it's color to a dimmer color.

	color = tip.getColor();
	if (sliders[target].disabled)
		tip.setColor(Color(color.r-100, color.g-100, color.b-100, transparency));
	
	// here we just draw and position every part of the slider

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

	// add the slider functionality name on top of the slider 

	slider_text.setCharacterSize(5.5 * scale);
	slider_text.setString(sliders[target].name);
	slider_text.setPosition(x + sliders[target].x * (scale / 3), y + (sliders[target].y - 11) * scale);
	window->draw(slider_text);

	// add some text to the right side of the slider to show it's current value (game sound, framelimit, etc...)

	slider_text.setCharacterSize(5 * scale);
	if (sliders[target].text_type == 0)
		slider_text.setString(to_string(*sliders[target].linker));
	else if (sliders[target].text_type == 1)
		slider_text.setString(to_string(*sliders[target].linker) + "%");
	else
		slider_text.setString(sliders[target].txt[*sliders[target].linker]);
	slider_text.setPosition(x + (sliders[target].x + 9 + sliderconst + 15) * (scale / 3), y + (sliders[target].y - 4) * scale);
	window->draw(slider_text);

	// ?? 
	tip.setColor(color);
}

void SettingsState::settings_intializer()
{
	//read current settings
	if (window->getSize().x / 120.0 < window->getSize().y / 120.0) scale = window->getSize().x / 120.0;
	else scale = window->getSize().y / 120.0;
	x = window->getSize().x;
	y = window->getSize().y / 2;
	if (x > 1280) scale *= 0.75;
	x /= 2;

	//initialize resolution data
	for (int i = 0; i < resnum; i++) {
		if (game.windowbounds == resolutions[i]) {
			resolution = prev_resolution = i;
			break;
		}
	}
	
	// if a custom resolution is added straight in the config file it's added in the end of the array and game
	// will function normally

	if (resolution == resnum)
		resolutions[resnum] = game.windowbounds, resolutions_text[resnum] = to_string(game.windowbounds.width) + " x " + to_string(game.windowbounds.height);

	for (int i = 0; i < resnum; i++)
		resolutions_text[i] = to_string(resolutions[i].width) + " x " + to_string(resolutions[i].height);



	//initialize framelimit data
	for (int i = 0; i < framelimnum; i++) {
		if (game.framelimit == framelimit_values[i]) {
			framelimit = prev_framelimit = i;
			break;
		}
	}

	// if a custom framelimit is added straight in the config file it's added in the end of the array and game
	// will function normally

	if (framelimit == framelimnum)
		framelimits[framelimnum] = game.framelimit, framelimits_text[framelimnum] = to_string(game.framelimit);

	for (int i = 0; i < framelimnum; i++)
		framelimits_text[i] = framelimits[i];


	//intiliazie slider positions
	float percentage = 1;
	for (int i = 0; i < 4; i++) {
		percentage = (*sliders[i].linker < sliders[i].mx) ? *sliders[i].linker / (float)sliders[i].mx : 1.0;
		sliders[i].tipx = x + (sliders[i].x + 9 + percentage * sliderconst) * (scale / 3);
		sliders[i].midscale = (sliders[i].tipx - (x + (sliders[i].x + 9) * (scale / 3))) / (18 * (scale / 3));
	}
}

// checkboxes positioning, scaling and functionality

void SettingsState::update_checkbox(int i)
{
	checkbox.setScale(scale / 8.0, scale / 8.0);
	checkmark.setScale(scale/ 3.5, scale / 3.5);

	checkbox.setPosition(x + checkboxes[i].x * (scale / 3) + (45 / 2) * (scale / 8.0), y + checkboxes[i].y * scale);
	if (checkbox.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		if (Mouse::isButtonPressed(Mouse::Left) && checkbox.getGlobalBounds().contains(clicked_on)) {
			if (!checkboxes[i].pressed) {
				if(checkboxes[i].checked)
					game.play_sfx(1);
				else
					game.play_sfx(0);
			}
			checkboxes[i].pressed = 1;
		}
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

SettingsState::SettingsState(bool bg_fade)
{
	this->bg_fade = bg_fade;

	// initialize the texture file
	initial_textures("settings");

	panel.setTexture(*textures[4]);
	panel.setOrigin(600 / 2, 600 / 2);

	tissue.setTexture(*textures[3]);
	tissue.setOrigin(700 / 2, 700 / 2);

	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));

	back_arrow.setTexture(*textures[1]);
	back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
	back_arrow.setOrigin(22 / 2, 21 / 2);

	// we add a check so that we can't access the map builder (see the dev button) in certain occasions.
	// we can't access it while being in the map builder itself neither in the game and the world map

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
	slider_text.setFillColor(Color(0, 0, 0));

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
	window->setMouseCursorVisible(true);

	//get real time mouse position and store it in a variable

	mouse_pos = window->mapPixelToCoords(Mouse::getPosition(*window));

	if (prev_win != window->getSize()) {
		// we always store the last window reslution in a variable and compare it with the current one
		// if there is a difference we get the new resolution and update our scale accordingly
		// also the variables for the window size the screen mid point
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 120.0 < win_y / 120.0) scale = win_x / 120.0;
		else scale = win_y / 120.0;
		if (win_x > 1280) scale *= 0.75;

		/////// Do Not touch;

		tint.setSize({ win_x, win_y });

		panel.setPosition(x, y);
		panel.setScale(scale * 0.17, scale * 0.17);

		tissue.setPosition(x, y);
		tissue.setScale(scale * 0.13, scale * 0.13);

	}

	// some simple conditions to make sure that everything is disabled in the right time

	if (fullscreen)
		sliders[0].disabled = 1;
	else
		sliders[0].disabled = 0;

	if (vsync)
		sliders[1].disabled = 1;
	else
		sliders[1].disabled = 0;

	// a for loop to update all sliders and checkboxes and also the dev button

	for (int i = 0; i < 4; i++)
		update_slider(sliders, i);

	for (int i = 0; i < 2; i++)
		update_checkbox(i);

	if (dev_button_active)
		dev_button();

	if (exit) {
		if (fade_out()) {
			exit = false;
			if (states->find(MapBuilderID) == states->end() && states->find(GameID) == states->end() && states->find(WorldMapID) == states->end()) {
				// if escape is pressed while not being in the map builder or game state or world map the main menu state is pushed
				// any other place the pause state is pushed
				states->insert(MainMenuST);
				states->at(MainMenuID)->update();
			}
			else {
				states->insert({ PauseID, new PauseState(false) });
				states->at(PauseID)->update();
			}
			int exceptions[] = { MainMenuID, BackgroundID, PauseID, MapBuilderID, GameID, WorldMapID, NotificationID };
			game.erase_states(exceptions, 7);
			return;
		}
	}
	else
		fade_in();


	update_arrow();
}

void SettingsState::render()
{

	// drawing the whole settings 

	window->draw(tint);
	window->draw(panel);
	window->draw(tissue);
	if (dev_button_active) {
		window->draw(devbutton);
		window->draw(devtext);
	}

	window->draw(back_arrow);
	text.setFillColor(Color(0, 0, 0, transparency));
	draw_text("Settings", x, y - 35 * scale, 6.5 * scale);

	// for loops to render the sliders and checkboxex

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
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				exit = true;
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
			break;
		case Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
			case Mouse::Left:
				// clicked on is a vector2f that stores the position of the mouse when the left mouse button is clicked
				clicked_on = window->mapPixelToCoords(Mouse::getPosition(*window));
				break;
			}
		}
	}
}