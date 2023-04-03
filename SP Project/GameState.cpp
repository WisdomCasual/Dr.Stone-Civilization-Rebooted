#include "GameState.h"

GameState::GameState()
{

}

GameState::~GameState()
{

}


void GameState::update(float dt, RenderWindow* window, int* terminator, deque<State*>* states)
{

}

void GameState::render()
{

}

void GameState::pollevent(Event event, RenderWindow* window, int* terminator, deque<State*>* states)
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
