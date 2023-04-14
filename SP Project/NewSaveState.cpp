#include "NewSaveState.h"

NewSaveState::NewSaveState()
{
	State::initial_textures("newsave");
	txt_box.initializeTextBox(test_str, *textures[2], "Write Here", {500.0, 500.0});
}

NewSaveState::~NewSaveState()
{
}

void NewSaveState::update()
{
}

void NewSaveState::render()
{
	txt_box.drawTextBox(window);
}

void NewSaveState::pollevent()
{
	while (window->pollEvent(event)) {
		txt_box.text_poll(event);
		switch (event.type) {
			case Event::Closed:
				window->close(); break;
		}
	}
}
