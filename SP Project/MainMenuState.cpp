#include "MainMenuState.h"


void MainMenuState::update_buttons(RenderWindow* window)
{
	for (auto& button : buttons) {
		buttontex.setTextureRect(IntRect(0,button.pressed * 49, 190, 49));
		buttontex.setPosition(x+button.x * scale, y+button.y * scale);
		if (buttontex.getGlobalBounds().contains(window->mapPixelToCoords(Mouse::getPosition(*window)))) {
			if (Mouse::isButtonPressed(Mouse::Left) && buttontex.getGlobalBounds().contains(clicked_on))button.pressed = 1;
			else {
				if (button.pressed)
					button.execute = 1;
				button.pressed = 0; 
			}
			button.hover = 1;
		}
		else {
			button.pressed = 0;
			button.hover = 0;
		}
	}
}

void MainMenuState::render_buttons(RenderWindow* window)
{
	buttontex.setScale(scale, scale);
	text.setCharacterSize(25.69*scale);
	for (auto& button : buttons) {
		buttontex.setTextureRect(IntRect(0, button.pressed * 49, 190, 49));
		buttontex.setPosition(x+button.x*scale, y+button.y * scale);
		text.setString(button.txt);
		FloatRect bounds=text.getLocalBounds();
		text.setOrigin(bounds.width / 2.0, bounds.top+bounds.height / 2.0);
		text.setPosition(x + button.x * scale, (button.pressed) ? y + button.y * scale + 2 * scale : y + button.y * scale - 2 * scale);
		if (button.hover)text.setFillColor(Color::White);
		else text.setFillColor(Color::Color(226,211,195));
		window->draw(buttontex);
		window->draw(text);
	}
}

MainMenuState::MainMenuState()
{
	initial_textures("mainmenu");
	initial_fps();
	buttontex.setTexture(*textures[0]);
	buttontex.setTextureRect(IntRect(0, 0, 190, 49));
	buttontex.setOrigin(190/2,49/2);
	logo.setTexture(*textures[1]);
	logo.setOrigin(2280/2,772/2);
	text.setFont(font);
	buttons.push_back({ "Play",0,0, play});
	buttons.push_back({ "Settings",0,64, settings});
	buttons.push_back({ "Exit" ,0,128, exit});
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::update(float dt, RenderWindow* window, int* terminator, deque<State*>* states)
{
	float win_x = window->getSize().x, win_y = window->getSize().y;
	x = win_x / 2, y = win_y / 2;
	if (win_x / 390.0 < win_y / 390.0) scale = win_x / 390.0;
	else scale = win_y / 390.0;

	logo.setScale(scale * 0.13, scale * 0.13);
	logo.setPosition(x, y - 110 * scale);

	if (play) {
		play = 0;
		*terminator += states->size();
		states->push_front(new SavesState);
		states->push_front(new Background);
	}
	if (settings) { 
		settings = 0;
		*terminator += states->size();
		states->push_front(new SettingsState);
		states->push_front(new Background); 
	}
	if (exit) { exit = 0; window->close(); }

	update_buttons(window);
	if (fps_active)
		calc_fps(dt);
}

void MainMenuState::render(RenderWindow* window)
{
	render_buttons(window);
	window->draw(logo);
	if (fps_active)
		window->draw(fps_text);
}

void MainMenuState::pollevent(Event event, RenderWindow* window, int* terminator, deque<State*>* states)
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
