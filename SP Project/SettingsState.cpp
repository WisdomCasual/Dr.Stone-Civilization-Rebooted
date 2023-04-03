#include "SettingsState.h"
#define sliderconst 135.0

void SettingsState::update_arrow(RenderWindow* window, int* terminator, deque<State*>* states)
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
					ofs << resolutions[resolution].x << ' ' << resolutions[resolution].y <<'\n';
					ofs << framelimits[framelimit] <<'\n';
					ofs << save_vsync;
					ofs.close();
				}
				*terminator += states->size();
				states->push_front(new MainMenuState);
				states->push_front(new Background);
			}
		}

	}
	else {
		arrow_pressed = 0;
		back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
		back_arrow.setScale(scale * 0.3, scale * 0.3);
	}
}

void SettingsState::dev_button(RenderWindow* window, int* terminator, deque<State*>* states)
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
				*terminator += states->size();
				states->push_front(new MapBuilderState);
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
		slider_text.setString(to_string(framelimits[*sliders[target].linker]));
	else if(sliders[target].text_type == 1)
		slider_text.setString(to_string(*sliders[target].linker) + "%");
	else
		slider_text.setString(to_string(resolutions[*sliders[target].linker].x) + " x " + to_string(resolutions[*sliders[target].linker].y));
	slider_text.setPosition(x + (sliders[target].x + 9 + sliderconst + 15) * (scale / 3), y + (sliders[target].y - 4) * scale);
	window->draw(slider_text);
}

SettingsState::SettingsState()
{
	initial_fps();
	initial_textures("settings");
	tissue.setTexture(*textures.at(0));
	tissue.setTextureRect(buttons[3]);
	tissue.setOrigin(92 / 2, 92 / 2);
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

	for (int i = 0; i < 4; i++) {
		sliders[i].tipx = save_resolution.width / 2 + (sliders[i].x + 9 + *sliders[i].linker / (float)sliders[i].mx * sliderconst) * (scale / 3);
		sliders[i].midscale = (sliders[i].tipx - (save_resolution.width / 2 + (sliders[i].x + 9) * (scale / 3))) / (18 * (scale / 3));
	}
}

SettingsState::~SettingsState()
{

}

void SettingsState::update(float dt, RenderWindow* window, int* terminator, deque<State*>* states)
{
	win_x = window->getSize().x, win_y = window->getSize().y;
	x = win_x / 2, y = win_y / 2;
	if (win_x / 120.0 < win_y / 120.0) scale = win_x / 120.0;
	else scale = win_y / 120.0;
	mouse_pos = window->mapPixelToCoords(Mouse::getPosition(*window));
	/////// Do Not touch;

	tint.setSize({ win_x, win_y });

	tissue.setPosition(x, y);
	tissue.setScale(scale, scale);

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
	window->draw(devbutton);
	window->draw(devtext);
	window->draw(back_arrow);
	text.setFillColor(Color::Black);
	draw_text(window, "Settings", x, y - 35 * scale, 6.5 * scale);
	for(int i = 0; i < 4; i++)
		render_slider(window,i);


	if (fps_active)
		window->draw(fps_text);
}

void SettingsState::pollevent(Event event, RenderWindow* window, int* terminator, deque<State*>* states)
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