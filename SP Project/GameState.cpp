#include "GameState.h"

GameState::GameState()
{

}

GameState::~GameState()
{

}


void GameState::update()
{

}

void GameState::scaling()
{
}

void GameState::render()
{

}

void GameState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			window->close(); break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				window->close(); break;
			}
		}
	}
}
