#include "SettingsState.h"
#define sliderconst 135.0

void SettingsState::update_arrow(RenderWindow* window, int* terminator, map<int, State*>* states)
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
				ofstream ofs("config/window.ini", ofstream::out, ofstream::trunc);
				if (ofs.is_open()) {
					ofs << title <<'\n';
					ofs << resolutions[resolution].width << ' ' << resolutions[resolution].height <<'\n';
					ofs << framelimits[framelimit] <<'\n';
					ofs << save_vsync;
					ofs.close();
				}
				game.update_window(resolutions[resolution], title, framelimits[framelimit], save_vsync, &window);

				if (states->find(MapBuilderID) == states->end() && states->find(GameID) == states->end()) {
					states->insert(MainMenuST);
					states->at(MainMenuID)->update(dt, window, terminator, states);
				}
				else {
					states->insert(PauseST);
					states->at(PauseID)->update(dt, window, terminator, states);
				}
		
				
				for (auto& state : *states) {
					if (state.first != MainMenuID && state.first != BackgroundID && state.first != PauseID && state.first != MapBuilderID && state.first != GameID) {
						delete state.second;
						states->erase(state.first);
					}
				}

			}
		}

	}
	else {
		arrow_pressed = 0;
		back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
		back_arrow.setScale(scale * 0.3, scale * 0.3);
	}
}

void SettingsState::dev_button(RenderWindow* window, int* terminator, map<int, State*>* states)
{
	if (test) {
		if (states->find(MapBuilderID) != states->end() || states->find(GameID) != states->end())
			dev_button_active = 0;
		test = 0;
	}

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

				states->insert(MapBuilderST);
				states->at(MapBuilderID)->update(dt, window, terminator, states);

				for (auto &state : *states) {
					if (state.first != MapBuilderID) {
						delete state.second;
						states->erase(state.first);
					}
				}
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

void SettingsState::update_slider(RenderWindow* window, slider_info* sliders, int target)
{
	if (Mouse::isButtonPressed(Mouse::Left)) {

		tip.setScale(scale / 3, scale / 3);
		tip.setTextureRect(tipsleft[sliders[target].color]);
		tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
		tip.setPosition(x + sliders[target].x * (scale / 3), y + sliders[target].y * scale);
		if(tip.getGlobalBounds().contains(clicked_on))
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
		if(mouse_pos.x < initpos) sliders[target].tipx = initpos;
		else if (mouse_pos.x > initpos + mxlen) sliders[target].tipx = initpos + mxlen;
		else
		{
			sliders[target].tipx = initpos + (sliders[target].mx - round((initpos + mxlen - mouse_pos.x) / stepsize)) * stepsize;
		}
		*sliders[target].linker = ((sliders[target].tipx - x) / (scale / 3) - sliders[target].x - 9) / sliderconst * sliders[target].mx;
		sliders[target].midscale = (sliders[target].tipx - initpos) / (18 * (scale/3));
	}
}

void SettingsState::render_slider(RenderWindow* window,int target)
{
	//background

	tip.setScale(sliderconst/18 * (scale / 3), (scale / 3));
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
	tip.setScale(scale / 3, scale / 3);
	tip.setTextureRect(tipsleft[sliders[target].color]);
	tip.setOrigin(tip.getLocalBounds().left , tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);   
	tip.setPosition(x + sliders[target].x * (scale/3), y + sliders[target].y * scale);
	window->draw(tip);

	tip.setScale(sliders[target].midscale * (scale/3), (scale/3));
	tip.setTextureRect(mids[sliders[target].color]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(x + (sliders[target].x + 9) * (scale/3), y + sliders[target].y * scale);
	window->draw(tip);

	tip.setScale(scale / 3, scale / 3);
	tip.setTextureRect(tipsright[sliders[target].color]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(sliders[target].tipx, y + sliders[target].y * scale);

	window->draw(tip);

	slider_text.setCharacterSize(6.5 * scale);
	slider_text.setString(sliders[target].name);
	slider_text.setPosition(x + sliders[target].x * (scale / 3), y + (sliders[target].y - 12) * scale);
	window->draw(slider_text);

	slider_text.setCharacterSize(5 * scale);
	if(sliders[target].text_type == 0)
		slider_text.setString(to_string(*sliders[target].linker));
	else if(sliders[target].text_type == 1)
		slider_text.setString(to_string(*sliders[target].linker) + "%");
	else
		slider_text.setString(sliders[target].txt[*sliders[target].linker]);
	slider_text.setPosition(x + (sliders[target].x + 9 + sliderconst + 15) * (scale / 3), y + (sliders[target].y - 4) * scale);
	window->draw(slider_text);
}

void SettingsState::settings_intializer()
{
	//read current settings
	ifstream ifs("config/window.ini");
	if (ifs.is_open()) {
		getline(ifs, title);
		ifs >> save_resolution.width >> save_resolution.height;
		ifs >> save_framelimit;
		ifs >> save_vsync;
	}
	ifs.close();
	if (save_resolution.width / 120.0 < save_resolution.height / 120.0) scale = save_resolution.width / 120.0;
	else scale = save_resolution.height / 120.0;
	x = save_resolution.width / 2;
	y = save_resolution.height / 2;

	//initialize resolution data
	for (int i = 0; i < resnum; i++) {
		if (save_resolution == resolutions[i]) {
			resolution = i;
			break;
		}
	}
	if (resolution == resnum)
		resolutions[resnum] = save_resolution, resolutions_text[resnum] = to_string(save_resolution.width) + " x " + to_string(save_resolution.height);

	for (int i = 0; i < resnum; i++)
		resolutions_text[i] = to_string(resolutions[i].width) + " x " + to_string(resolutions[i].height);



	//initialize framelimit data
	for (int i = 0; i < framelimnum; i++) {
		if (save_framelimit == framelimits[i]) {
			framelimit = i;
			break;
		}
	}

	if (framelimit == framelimnum)
		framelimits[framelimnum] = save_framelimit, framelimits_text[framelimnum] = to_string(save_framelimit);
	
	for (int i = 0; i < framelimnum; i++)
		framelimits_text[i] = to_string(framelimits[i]);


	//intiliazie slider positions
	float percentage = 1;
	for (int i = 0; i < 4; i++) {
		percentage = (*sliders[i].linker < sliders[i].mx) ? *sliders[i].linker / (float)sliders[i].mx : 1.0;
		sliders[i].tipx = x + (sliders[i].x + 9 +percentage * sliderconst) * (scale / 3);
		sliders[i].midscale = (sliders[i].tipx - (x + (sliders[i].x + 9) * (scale / 3))) / (18 * (scale / 3));
	}
}

SettingsState::SettingsState()
{
	initial_fps();
	initial_textures("settings");
	tissue.setTexture(*textures[3]);
	tissue.setOrigin(700 / 2, 700 / 2);
	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));

	back_arrow.setTexture(*textures[1]);
	back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
	back_arrow.setOrigin(22 / 2, 21 / 2);


	devbutton.setTexture(*textures[2]);
	devbutton.setTextureRect(IntRect(0, 0, 45, 49));
	devbutton.setOrigin(45 / 2, 49 / 2);


	devtext.setFont(font);
	devtext.setString("Dev");
	devtext.setCharacterSize(40);
	devtext.setFillColor(Color(200, 200, 200));
	
	slider_text.setFont(font);
	slider_text.setCharacterSize(50);
	slider_text.setFillColor(Color::Black);

	tip.setTexture(*textures[0]);

	settings_intializer();
}

SettingsState::~SettingsState()
{

}

void SettingsState::update(float dt, RenderWindow* window, int* terminator, map<int, State*>* states)
{
	this->dt = dt;
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
		dev_button(window, terminator, states);

	update_arrow(window, terminator, states);
	if (fps_active)
		calc_fps(dt);

	for (int i = 0; i < 4; i++)
		update_slider(window, sliders, i);
}

void SettingsState::render(RenderWindow* window)
{
	window->draw(tint);
	window->draw(tissue);
	if (dev_button_active) {
		window->draw(devbutton);
		window->draw(devtext);
	}
	window->draw(back_arrow);
	text.setFillColor(Color::Black);
	draw_text(window, "Settings", x, y - 35 * scale, 6.5 * scale);
	for(int i = 0; i < 4; i++)
		render_slider(window,i);


	if (fps_active)
		window->draw(fps_text);
}

void SettingsState::pollevent(Event event, RenderWindow* window, int* terminator, map<int, State*>* states)
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			window->close(); break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				if (states->find(MapBuilderID) == states->end() && states->find(GameID) == states->end()) {
					states->insert(MainMenuST);
					states->at(MainMenuID)->update(dt, window, terminator, states);
				}
				else {
					states->insert(PauseST);
					states->at(PauseID)->update(dt, window, terminator, states);
				} 
				for (auto& state : *states) {
					if (state.first != MainMenuID && state.first != BackgroundID && state.first != PauseID && state.first != MapBuilderID && state.first != GameID) {
						delete state.second;
						states->erase(state.first);
					}
				}
				break;
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