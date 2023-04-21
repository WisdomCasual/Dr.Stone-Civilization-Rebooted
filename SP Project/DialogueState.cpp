#include "DialogueState.h"

DialogueState::DialogueState(dialogue* dialogues, Vector2f pos, float scale, int dialogues_number)
{
	initial_textures("dialogue");
	output_text.setFont(font);
	output_text.setCharacterSize(40 * scale);
	output_text.setPosition(position);
	speaker_text.setFont(font);
	setDialogue(dialogues, dialogues_number);
	setPosition(pos);
	setScale(scale);
	box.setTexture(*textures[0]);
	lim = box.getLocalBounds().height / 15;
}

DialogueState::~DialogueState()
{
}

void DialogueState::setDialogue(dialogue* dialogues, int dialogues_number)
{
	this->dialogues_number = dialogues_number;
	this->dialogues = dialogues;
	speaker_text.setString(dialogues[0].speaker);
}

void DialogueState::setTexture(Texture& box_texture)
{
	box.setTexture(box_texture);
	box.setScale(scale * 2, scale * 2);
	box.setOrigin(box.getLocalBounds().left + box.getLocalBounds().width / 2.0, box.getLocalBounds().top + box.getLocalBounds().height / 2.0);
	box.setPosition(position);
}

void DialogueState::setFont(Font font)
{
	output_text.setFont(font);
	speaker_text.setFont(font);
}

void DialogueState::setPosition(const Vector2f new_position)
{
	position = new_position;
	output_text.setString(y_string);
	text_y_bound = output_text.getLocalBounds().top + output_text.getLocalBounds().height/2.0;
	box.setOrigin(box.getLocalBounds().left + box.getLocalBounds().width / 2.0, box.getLocalBounds().top + box.getLocalBounds().height / 2.0);
	box.setPosition(position);
	speaker_text.setOrigin(speaker_text.getLocalBounds().left, speaker_text.getLocalBounds().top + speaker_text.getLocalBounds().height);
	speaker_text.setPosition(Vector2f( (float)position.x - box_bounds.width / 2.0, (float)position.y - box_bounds.height / 2.0 ));
}

void DialogueState::setScale(const float new_scale)
{
	scale = new_scale;
	output_text.setCharacterSize(40 * scale);
	speaker_text.setCharacterSize(40 * scale);
	box.setScale(2 * scale, 2 * scale);
}

void DialogueState::render_text()
{
	float init_line = (0.5 - lines / 2.0), dis = 30*scale;
	for (int i = 0; i < lines; i++, init_line++) {
		output_text.setString(y_string);
		text_y_bound = output_text.getLocalBounds().top + output_text.getLocalBounds().height / 2;
		output_text.setString(output_strings[i]);
		output_text.setOrigin(output_text.getLocalBounds().left + output_text.getLocalBounds().width / 2.0, text_y_bound);
		output_text.setPosition(Vector2f(position.x, position.y + init_line * dis));
		window->draw(output_text);
	}
}

void DialogueState::write_text()
{
	if (text_x_bound >= 0.75 * box_bounds.width) {
		if (lines >= lim) {
			reminder_idx = char_idx;
			typing = 0;
			return;
		}
		lines++;
	}
	output_strings[lines - 1] += dialogues[dialogue_idx].text[char_idx], char_idx++;
	output_text.setString(output_strings[lines - 1]);
	text_x_bound = output_text.findCharacterPos(char_idx - reminder_idx).x - output_text.findCharacterPos(0).x;
}

void DialogueState::update()
{
	if (typing) {
		if (skip && char_idx > 0) {
			while (char_idx < dialogues[dialogue_idx].text.size() && typing) {
				write_text();
			}
			typing = 0, skip = 0;
			reminder_idx = (char_idx < dialogues[dialogue_idx].text.size()) ? char_idx : 0;
			return;
		}
		if (delay >= 0.14) {
			delay = 0;
			if (char_idx < dialogues[dialogue_idx].text.size()) {
				write_text();
			}
			else
				typing = 0, reminder_idx = 0;
		}
		else
			delay += dt;
	}
	box_bounds = box.getGlobalBounds();
	setPosition(position);
}

void DialogueState::render()
{
	window->draw(box);
	window->draw(speaker_text);
	render_text();
}

void DialogueState::pollevent()
{
	while (window->pollEvent(event)) {
		switch (event.type) {
		case Event::Closed:
			game.exit_prompt();
			return; break;
		case Event::KeyPressed:
			switch (event.key.code) {
			case Keyboard::Escape:
				states->insert(PauseST); return; break;
				break;
			case Keyboard::F3:
				fps_active = !fps_active; break;
			case Keyboard::F11:
				fullscreen = !fullscreen;
				game.update_window();
				break;
			}
		case Keyboard::Space:
			if (typing)
				skip = 1;
			else {
				if (!reminder_idx) {
					if (dialogue_idx >= dialogues_number - 1) {
						delete states->at(DialogueID);
						states->erase(DialogueID);
						return;
					}
					dialogue_idx++;
					speaker_text.setString(dialogues[dialogue_idx].speaker);
					char_idx = 0;
				}
				for (int i = 0; i < lines; i++) {
					output_strings[i].clear();
				}
				lines = 1, delay = 0, skip = 0, typing = 1;
				output_text.setString(output_strings[lines - 1]);
				text_x_bound = output_text.findCharacterPos(char_idx - reminder_idx).x - output_text.findCharacterPos(0).x;
			}
			break;
		}
	}
}
