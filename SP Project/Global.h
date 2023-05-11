#pragma once
#include"State.h"

#define PI 3.141593f
// background: 1, MainMenuState: 2 , SavesState: 3 , NewSaveState: 4 , GameState: 5 , 
//MapBuilderState: 6, PauseState: 10, SettingsState: 11, ConfirmState: 12: 


#define quests_no 1

#define BackgroundID 1
#define BackgroundST { 1, new Background }

#define MainMenuID 2
#define MainMenuST { 2, new MainMenuState }

#define SavesID 3
#define SavesST { 3, new SavesState }

#define NewSaveID 4                       ///////
#define NewSaveST { 4, new NewSaveState } ///////

#define GameID 5
#define GameST { 5, new GameState }

#define MapBuilderID 6
#define MapBuilderST { 6, new MapBuilderState }

#define WorldMapID 7
#define WorldMapST { 7, new WorldMapState }
#define WorldMapST_admin { 7, new WorldMapState(1) }

#define NewMapID 8
#define NewMapST { 8, new NewMapState}

#define NotificationID 9
#define NotificationST { 9, new NotificationState }

#define DialogueID 10
#define DialogueST { 10, new DialogueState}

#define PauseID 11
#define PauseST { 11, new PauseState }

#define SettingsID 12
#define SettingsST { 12, new SettingsState }

#define PasswordID 13
#define PasswordST { 13, new PasswordState }

#define ConfirmationID 14
#define ConfirmationST { 14, new ConfirmationState }

#define TestGroundID 50
#define TestGroundST { 50, new TestGroundState }


namespace globalvar {

	inline float dt;
	inline RenderWindow* window;
	inline map<int, State*>* states;
	inline Event event;

	inline int fps = 0, frame_count = 0, frame_sum = 0;
	inline bool fps_active = 0, fullscreen = 0, vsync = 0, exit_game = 0;
	inline float delay = 0;

	inline Font font;
	inline Text fps_text, text;
	inline string notification_strings[3] = {""};
	inline int notification_lines = 1;
	inline float notification_delay = 0;

	inline const int dx[4] = { 1, -1, 0, 0 };
	inline const int dy[4] = { 0, 0, 1, -1};

	struct dialogue {
		string speaker, text;
		short expression = 0, pic = 1;
	};

	inline void draw_text(string text_string, int x_pos, int y_pos, float characterSize)   //<-- gets window pointer, string, X cord, Y cords, Character size to draw text
	{
		//gets window pointer, string, X cord, Y cords, Character size to draw text
		text.setString(text_string);
		text.setCharacterSize(characterSize);
		text.setOrigin(text.getLocalBounds().width / 2.0, text.getLocalBounds().top + text.getLocalBounds().height / 2.0);
		text.setPosition(x_pos, y_pos);
		window->draw(text);
	}
	
	inline void encrypt(string& string, int key) {
		srand(key);
		for (auto& i : string) {
			srand((((key % 256 + rand() % 256) + i) * (string.size() % 256))%256);
			i = ((i % 256 + rand() % 256) % 94) + 33;
		}
	}

	inline float dot(const Vector2f V1, const Vector2f V2) {
		return V1.x * V2.x + V1.y + V2.y;
	}

	inline float magnitude(const Vector2f V) {
		return sqrtf(V.x * V.x + V.y * V.y);
	}

	inline Vector2f toCartesian(const Vector2f V) {
		return Vector2f(V.x * cos(V.y), V.x * sin(V.y));
	} //magnitude, direction
}