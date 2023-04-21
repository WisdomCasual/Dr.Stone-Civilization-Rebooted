#include "TestGroundState.h"

TestGroundState::TestGroundState()
{
	State::initial_textures("testground");
}

TestGroundState::~TestGroundState()
{

}

void TestGroundState::update()
{
}

void TestGroundState::render()
{
}

void TestGroundState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			window->close();
			return; break;
		}
	}
}
