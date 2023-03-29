#include "MainMenuState.h"


void MainMenuState::update_buttons()
{
	for (auto& button : buttons) {
		buttontex.setTextureRect(IntRect(0,button.pressed * 49, 190, 49));
		buttontex.setPosition(x+button.x * scale, y+button.y * scale);
		if (buttontex.getGlobalBounds().contains(mouse_cords)) {
			if (Mouse::isButtonPressed(Mouse::Left))button.pressed = 1;
			else button.pressed = 0;
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
	mouse_cords = window->mapPixelToCoords(Mouse::getPosition(*window));
	buttontex.setScale(scale, scale);
	text.setCharacterSize(28.69*scale);
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
	text.setCharacterSize(28.69);
	buttons.push_back({ "Play",0,0});
	buttons.push_back({ "Settings",0,64});
	buttons.push_back({ "Exit" ,0,128});
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::update(float dt)
{


	update_buttons();
	if (fps_active)
		calc_fps(dt);

}

void MainMenuState::render(RenderWindow* window)
{
	float win_x = window->getSize().x, win_y = window->getSize().y;
	x = win_x / 2, y = win_y / 2;
	if (win_x / 620.0 < win_y / 370.0) scale = win_x / 620.0;
	else scale = win_y / 370.0;
	render_buttons(window);
	logo.setScale(scale * 0.13, scale * 0.13);
	logo.setPosition(x,y-110*scale);
	window->draw(logo);
	if (fps_active)
		window->draw(fps_text);
}

void MainMenuState::pollevent(Event event, RenderWindow* window)
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
		}
	}
}
