#include "NewMapState.h"
#define sliderconst 100.0


void NewMapState::fade_in()
{
	if (transparency < 255) {
		if (transparency + 1500 * dt > 255)
			transparency = 255;
		else
			transparency += 1500 * dt;

		tissue.setColor(Color(255, 255, 255, transparency));
		bg.setColor(Color(255, 255, 255, transparency));
		back_arrow.setColor(Color(255, 255, 255, transparency));
		tip.setColor(Color(255, 255, 255, transparency));
		txt_box.setColor(Color(255, 255, 255, transparency));
		slider_text.setFillColor(Color(0, 0, 0, transparency));

		if (darkness < 154) {
			if (darkness + 154 * dt * 6 > 154)
				darkness = 154;
			else
				darkness += 154 * dt * 6;
			tint.setFillColor(Color(0, 0, 0, darkness));
		}
	}
}

bool NewMapState::fade_out()
{
	if (transparency > 0) {
		if (transparency - 1500 * dt < 0)
			transparency = 0;
		else
			transparency -= 1500 * dt;

		tissue.setColor(Color(255, 255, 255, transparency));
		bg.setColor(Color(255, 255, 255, transparency));
		back_arrow.setColor(Color(255, 255, 255, transparency));
		tip.setColor(Color(255, 255, 255, transparency));
		txt_box.setColor(Color(255, 255, 255, transparency));
		slider_text.setFillColor(Color(0, 0, 0, transparency));

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

void NewMapState::update_arrow()
{
	back_arrow.setPosition(x - 35 * scale, y - 35 * scale);
	back_arrow.setScale(scale * 0.36, scale * 0.36);
	if (back_arrow.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
		clickable_cursor = true;
		back_arrow.setTextureRect(IntRect(22, 0, 22, 21));
		if (Mouse::isButtonPressed(Mouse::Left) && back_arrow.getGlobalBounds().contains(clicked_on)) {
			if(!arrow_pressed)
				game.play_sfx(1);
			arrow_pressed = 1;
			back_arrow.setTextureRect(IntRect(44, 0, 22, 21));
			back_arrow.setScale(scale * 0.33, scale * 0.33);
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
		back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
	}
}

void NewMapState::update_slider(slider_info* sliders, int target)
{

	sliders[target].presssed = 0;

	tip.setScale(scale / 2.3, scale / 2.3);
	tip.setTextureRect(tipsleft[sliders[target].color]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(x + sliders[target].x * (scale / 2.3), y + sliders[target].y * scale);
	if (tip.getGlobalBounds().contains(clicked_on) && Mouse::isButtonPressed(Mouse::Left))
		sliders[target].presssed = 1;

	if (tip.getGlobalBounds().contains(mouse_pos))
		clickable_cursor = true;


	tip.setScale(sliderconst / 18 * (scale / 2.3), (scale / 2.3));
	tip.setTextureRect(mids[3]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(x + (sliders[target].x + 9) * (scale / 2.3), y + sliders[target].y * scale);
	if (tip.getGlobalBounds().contains(clicked_on) && Mouse::isButtonPressed(Mouse::Left))
		sliders[target].presssed = 1;

	if (tip.getGlobalBounds().contains(mouse_pos))
		clickable_cursor = true;

	tip.setScale(scale / 2.3, scale / 2.3);
	tip.setTextureRect(tipsright[3]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(x + (sliders[target].x + 9 + sliderconst) * (scale / 2.3), y + sliders[target].y * scale);
	if (tip.getGlobalBounds().contains(clicked_on) && Mouse::isButtonPressed(Mouse::Left))
		sliders[target].presssed = 1;

	if (tip.getGlobalBounds().contains(mouse_pos))
		clickable_cursor = true;


	if (sliders[target].presssed) {
		clickable_cursor = true;

		float initpos = x + (sliders[target].x + 9) * (scale / 2.3), mxlen = sliderconst * (scale / 2.3), stepsize = mxlen / sliders[target].mx;
		if (mouse_pos.x < initpos) sliders[target].tipx = initpos;
		else if (mouse_pos.x > initpos + mxlen) sliders[target].tipx = initpos + mxlen;
		else
		{
			sliders[target].tipx = initpos + (sliders[target].mx - round((initpos + mxlen - mouse_pos.x) / stepsize)) * stepsize;
		}
		*sliders[target].linker = round(((sliders[target].tipx - x) / (scale / 2.3) - sliders[target].x - 9) / sliderconst * sliders[target].mx) + 1;
		sliders[target].midscale = (sliders[target].tipx - initpos) / (18 * (scale / 2.3));
	}
}

void NewMapState::render_slider(int target)
{
	//background

	tip.setScale(sliderconst / 18 * (scale / 2.3), (scale / 2.3));
	tip.setTextureRect(mids[3]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(x + (sliders[target].x + 9) * (scale / 2.3), y + sliders[target].y * scale);
	window->draw(tip);

	tip.setScale(scale / 2.3, scale / 2.3);
	tip.setTextureRect(tipsright[3]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(x + (sliders[target].x + 9 + sliderconst) * (scale / 2.3), y + sliders[target].y * scale);
	window->draw(tip);

	//foreground

	tip.setScale(scale / 2.3, scale / 2.3);
	tip.setTextureRect(tipsleft[sliders[target].color]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(x + sliders[target].x * (scale / 2.3), y + sliders[target].y * scale);
	window->draw(tip);

	tip.setScale(sliders[target].midscale * (scale / 2.3), (scale / 2.3));
	tip.setTextureRect(mids[sliders[target].color]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(x + (sliders[target].x + 9) * (scale / 2.3), y + sliders[target].y * scale);
	window->draw(tip);

	tip.setScale(scale / 2.3, scale / 2.3);
	tip.setTextureRect(tipsright[sliders[target].color]);
	tip.setOrigin(tip.getLocalBounds().left, tip.getLocalBounds().top + tip.getLocalBounds().height / 2.0);
	tip.setPosition(sliders[target].tipx, y + sliders[target].y * scale);

	window->draw(tip);

	slider_text.setCharacterSize(8 * scale);
	slider_text.setString(sliders[target].name);
	slider_text.setPosition(x + (sliders[target].x - 15) * (scale / 2.3), y + (sliders[target].y - 5.5) * scale);
	window->draw(slider_text);

	slider_text.setCharacterSize(8 * scale);
	if (sliders[target].text_type == 0)
		slider_text.setString(to_string(*sliders[target].linker));
	else if (sliders[target].text_type == 1)
		slider_text.setString(to_string(*sliders[target].linker) + "%");
	else
		slider_text.setString(sliders[target].txt[*sliders[target].linker]);
	slider_text.setPosition(x + (sliders[target].x + 9 + sliderconst + 15) * (scale / 2.3), y + (sliders[target].y - 5.5) * scale);
	window->draw(slider_text);

}

void NewMapState::update_buttons()
{
	if(!txt_box.empty()) {
		buttontex.setColor(Color(255, 255, 255, transparency));
		buttontex.setPosition(x + confirm.x * scale / 3.0, y + confirm.y * scale / 3.0);
		if (buttontex.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			clickable_cursor = true;
			if (Mouse::isButtonPressed(Mouse::Left) && buttontex.getGlobalBounds().contains(clicked_on)) {
				if(!confirm.pressed)
					game.play_sfx(0);
				confirm.pressed = 1;
			}
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
		buttontex.setColor(Color(155, 155, 155, transparency));
	}
}

void NewMapState::render_buttons()
{
	buttontex.setScale(scale / 3.0, scale / 3.0);
	button_text.setCharacterSize(8.69 * scale);
		buttontex.setTextureRect(IntRect(0, confirm.pressed * 49, 108, 49));
		buttontex.setPosition(x + confirm.x * scale / 3.0, y + confirm.y * scale / 3.0);
		button_text.setString(confirm.txt);
		FloatRect bounds = button_text.getLocalBounds();
		button_text.setOrigin(bounds.width / 2.0, bounds.top + bounds.height / 2.0);
		button_text.setPosition(x + confirm.x * scale / 3.0, (confirm.pressed) ? y + confirm.y * scale / 3.0 + 2 * scale / 3.0 : y + confirm.y * scale / 3.0 - 2 * scale / 3.0);
		if (confirm.hover)button_text.setFillColor(Color(255, 255, 255, transparency));
		else if (txt_box.empty())
			button_text.setFillColor(Color(120, 120, 120, transparency));
		else
			button_text.setFillColor(Color(200, 200, 200, transparency));
		
		window->draw(buttontex);
		window->draw(button_text);
}

void NewMapState::initialize_sliders()
{
	float percentage = 1;
	for (int i = 0; i < 2; i++) {
		percentage = (*sliders[i].linker < sliders[i].mx) ? *sliders[i].linker / (float)sliders[i].mx : 1.0;
		sliders[i].tipx = x + (sliders[i].x + 9 + percentage * sliderconst) * (scale / 2.3);
		sliders[i].midscale = (sliders[i].tipx - (x + (sliders[i].x + 9) * (scale / 2.3))) / (18 * (scale / 2.3));
	}
}

NewMapState::NewMapState()
{
	win_x = window->getSize().x, win_y = window->getSize().y;
	x = win_x / 2, y = win_y / 2;
	if (win_x / 150.0 < win_y / 150.0) scale = win_x / 150.0;
	else scale = win_y / 150.0;
	if (VideoMode::getDesktopMode().width < win_x * 1.7 || VideoMode::getDesktopMode().height < win_y * 1.7) scale *= 0.75;

	initial_textures("newmap");

	tissue.setTexture(*textures[5]);
	tissue.setOrigin(700 / 2, 700 / 2);

	bg.setTexture(*textures[0]);
	bg.setOrigin(600 / 2, 600 / 2);

	tint.setSize({ 1920, 1080 });
	tint.setFillColor(Color(0, 0, 0, 154));

	back_arrow.setTexture(*textures[1]);
	back_arrow.setTextureRect(IntRect(0, 0, 22, 21));
	back_arrow.setOrigin(22 / 2, 21 / 2);

	slider_text.setFont(font);
	slider_text.setCharacterSize(50);
	slider_text.setFillColor(Color::Black);

	buttontex.setTexture(*textures[3]);
	buttontex.setTextureRect(IntRect(0, 0, 108, 49));
	buttontex.setOrigin(108 / 2, 49 / 2);

	button_text.setFont(font);
	button_text.setCharacterSize(50);
	button_text.setFillColor(Color(200, 200, 200));

	tip.setTexture(*textures[4]);

	txt_box.initializeTextBox(map_name, *textures[2], "Enter Map name");

	initialize_sliders();

}

NewMapState::~NewMapState()
{

}

void NewMapState::update()
{
	mouse_pos = window->mapPixelToCoords(Mouse::getPosition(*window));

	if (prev_win != window->getSize()) {
		prev_win = window->getSize();
		win_x = window->getSize().x, win_y = window->getSize().y;
		x = win_x / 2, y = win_y / 2;
		if (win_x / 150.0 < win_y / 150.0) scale = win_x / 150.0;
		else scale = win_y / 150.0;

		if (VideoMode::getDesktopMode().width < win_x * 1.7 || VideoMode::getDesktopMode().height < win_y * 1.7) scale *= 0.75;

		///////////////////////////////////

		initialize_sliders();

		tint.setSize({ win_x, win_y });

		tissue.setScale(scale * 0.13, scale * 0.13);
		tissue.setPosition(x, y);

		bg.setPosition(x, y);
		bg.setScale(scale * 0.17, scale * 0.17);

		txt_box.setPosition({ x, y - 18 * scale});

		txt_box.setScale(scale * 0.2);
	}

	update_arrow();

	update_buttons();

	txt_box.update();

	for (int i = 0; i < 2; i++)
		update_slider(sliders, i);

	if (back) {
		pins.erase("");
		if (fade_out()) {
			delete states->at(NewMapID);
			states->erase(NewMapID);
			return;
		}
	}
	else if (confirmed) {
		if (fade_out()) {
			pins.insert({ map_name, {pins.at("").x,pins.at("").y ,x_size, y_size} });
			pins.erase("");
			delete states->at(NewMapID);
			states->erase(NewMapID);
			return;
		}
	}
	else
		fade_in();
}

void NewMapState::render()
{
	window->draw(tint);
	window->draw(bg);
	window->draw(tissue);

	window->draw(back_arrow);

	render_buttons();

	for (int i = 0; i < 2; i++)
		render_slider(i);

	txt_box.drawTextBox(window);
	text.setFillColor(Color(0, 0, 0, transparency));
	draw_text("Create a new map", x, y - 35 * scale, 7.5 * scale);
}

void NewMapState::pollevent()
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
				pins.erase("");states->erase(NewMapID); return; break;
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
