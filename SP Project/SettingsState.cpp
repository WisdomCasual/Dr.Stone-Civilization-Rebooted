#include "SettingsState.h"

SettingsState::SettingsState()
{
	initial_textures("settings");
	tissue.setTexture(*textures.at(0));
	tissue.setTextureRect(buttons[3]);
}

SettingsState::~SettingsState()
{

}

void SettingsState::update(float dt)
{

}

void SettingsState::render(RenderWindow* window)
{
	window->draw(tissue);
}

void SettingsState::pollevent(Event event, RenderWindow* window)
{


}
