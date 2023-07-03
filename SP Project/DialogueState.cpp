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
	lim = box.getLocalBounds().height * 0.85 / 30;
}

DialogueState::~DialogueState()
{
}

void DialogueState::setDialogue(dialogue* dialogues, int dialogues_number)
{
	this->dialogues_number = dialogues_number;
	this->dialogues = dialogues;
	speaker_text.setString(dialogues[0].speaker);
	pic.setTexture(*textures[dialogues[0].pic]);
	set_expression(dialogues[0].expression);
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
	text_y_bound = output_text.getLocalBounds().top + output_text.getLocalBounds().height / 2.0;
	text_x_offset = position.x - (box_bounds.width / 6.0);
	box.setOrigin(box.getLocalBounds().left + box.getLocalBounds().width / 2.0, box.getLocalBounds().top + box.getLocalBounds().height / 2.0);
	box.setPosition(position);
	speaker_text.setOrigin(speaker_text.getLocalBounds().left, speaker_text.getLocalBounds().top + speaker_text.getLocalBounds().height);
	speaker_text.setPosition(Vector2f((float)position.x - box_bounds.width / 2.0, (float)position.y - box_bounds.height / 2.0));
	pic.setOrigin(pic.getLocalBounds().left + pic.getLocalBounds().width / 2, pic.getLocalBounds().top + pic.getLocalBounds().height / 2);
	pic.setPosition(Vector2f (position.x + box_bounds.width / 3.0, position.y));
}

void DialogueState::setScale(const float new_scale)
{
	scale = new_scale;
	output_text.setCharacterSize(40 * scale);
	speaker_text.setCharacterSize(40 * scale);
	pic.setScale(scale/2, scale/2);
	box.setScale(scale, scale);
}

void DialogueState::render_text()
{
	float init_line = (0.5 - lines / 2.0), dis = 30 * scale;
	for (int i = 0; i < lines; i++, init_line++) {
		output_text.setString(y_string);
		text_y_bound = output_text.getLocalBounds().top + output_text.getLocalBounds().height / 2;
		if (!add_idx && i == lines - 1)
			output_text.setString(output_strings[i].substr(0, output_strings[i].size()-1));
		else
			output_text.setString(output_strings[i]);
		output_text.setOrigin(output_text.getLocalBounds().left + output_text.getLocalBounds().width / 2.0, text_y_bound);
		output_text.setPosition(Vector2f(text_x_offset, position.y + init_line * dis));
		if (!add_idx && i == lines - 1)
			output_text.setString(output_strings[i]);
		window->draw(output_text);
	}
}

void DialogueState::word_in_new_line()
{
	for (auto i = output_strings[lines - 1].rbegin(); i != output_strings[lines - 1].rend(); i++) {
		if (!(*i >= '0' && *i <= '9' || *i >= 'a' && *i <= 'z' || *i >= 'A' && *i <= 'Z')) {
			const bool new_line = (lines >= lim);
			if (!new_line) {
				for (auto j = i; j != output_strings[lines - 1].rbegin(); j--) {
					output_strings[lines] += *j;
				}
				output_strings[lines] += *output_strings[lines - 1].rbegin();
			}
			for (auto j = output_strings[lines - 1].rbegin(); j != i; j++) {
				output_strings[lines - 1].pop_back();
				char_idx -= new_line;
			}
			output_strings[lines - 1].pop_back();
			char_idx -= new_line;
			break;
		}
	}
}

void DialogueState::write_text()
{
	if (text_x_bound >= 0.6 * box_bounds.width || dialogues[dialogue_idx].text[char_idx] == '\n') {
		if (text_x_bound >= 0.6 * box_bounds.width) {
			word_in_new_line();
		}
		else
			char_idx++;
		if (lines >= lim) {
			output_strings[lines - 1] += "...";
			reminder_idx = char_idx;
			typing = 0;
			return;
		}
		lines++;
	}
	if (dialogues[dialogue_idx].text[char_idx] == '/')
		commands();
	if (char_idx < dialogues[dialogue_idx].text.size()) {
		output_strings[lines - 1] += dialogues[dialogue_idx].text[char_idx], char_idx++;
		output_text.setString(output_strings[lines - 1]);
		text_x_bound = output_text.findCharacterPos(char_idx - reminder_idx).x - output_text.findCharacterPos(0).x;
	}
}

void DialogueState::set_expression(short id)
{
	pic.setTextureRect({ 320 * id, 0, 320, 300 });
}

void DialogueState::commands()
{
	char_idx += (char_idx + 1 < dialogues[dialogue_idx].text.size());
	switch (dialogues[dialogue_idx].text[char_idx]) {
		case 'E':
			string expression_str;
			short expression_num = 0;
			char_idx += (char_idx + 1 < dialogues[dialogue_idx].text.size());
			while (isdigit(dialogues[dialogue_idx].text[char_idx]) && (char_idx < dialogues[dialogue_idx].text.size())) {
				expression_str += dialogues[dialogue_idx].text[char_idx];
				char_idx++;
			}
			for (auto i = expression_str.rbegin(); i != expression_str.rend(); i++)
				expression_num = expression_num * 10 +  *i - '0';
			char_idx += expression_str.size()-1;
			set_expression(expression_num);
			return;
	}
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
		while (delay >= 0.08) {
			delay -= 0.08;
			if (char_idx < dialogues[dialogue_idx].text.size()) {
				write_text();
			}
			else
				typing = 0, reminder_idx = 0;
		}
		delay += dt;
	}
	else {
		if (add_idx) {
			output_strings[lines - 1] += ' ';
			add_idx = 0;
		}
		while (delay >= 0.75) {
			delay -= 0.75;
			cursor = !cursor;
			output_strings[lines - 1][output_strings[lines - 1].size() - 1] = (cursor) ? '|' : ' ';
		}
		delay += dt;
	}
	box_bounds = box.getGlobalBounds();
	setPosition(position);
}

void DialogueState::render()
{
	window->draw(box);
	window->draw(pic);
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
				states->insert(PauseST);
				states->at(PauseID)->update();
				return; break;
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
					pic.setTexture(*textures[dialogues[dialogue_idx].pic]);
					set_expression(dialogues[dialogue_idx].expression);
					char_idx = 0;
				}
				for (int i = 0; i < lines; i++) {
					output_strings[i].clear();
				}
				lines = 1, delay = 0, skip = 0, typing = 1, cursor = 0, add_idx = 1;
				output_text.setString(output_strings[lines - 1]);
				text_x_bound = output_text.findCharacterPos(char_idx - reminder_idx).x - output_text.findCharacterPos(0).x;
			}
			break;
		}
	}
}
