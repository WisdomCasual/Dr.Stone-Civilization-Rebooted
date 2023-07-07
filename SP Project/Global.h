#pragma once
#include"State.h"
#include <random>

#define PI 3.141593f
// background: 1, MainMenuState: 2 , SavesState: 3 , NewSaveState: 4 , GameState: 5 , 
//MapBuilderState: 6, PauseState: 10, SettingsState: 11, ConfirmState: 12: 


#define quests_no 2

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

#define InventoryID 7
#define InventoryST { 7, new InventoryState }

#define MiniMapID 8
#define MiniMapST { 8, new MiniMapState }

#define WorldMapID 9
#define WorldMapST { 9, new WorldMapState }
#define WorldMapST_admin { 9, new WorldMapState("", 1) }

#define NewMapID 10
#define NewMapST { 10, new NewMapState}

#define NotificationID 11
#define NotificationST { 11, new NotificationState }

#define DialogueID 12
#define DialogueST { 12, new DialogueState}

#define PauseID 13
#define PauseST { 13, new PauseState }

#define SettingsID 14
#define SettingsST { 14, new SettingsState }

#define PasswordID 15
#define PasswordST { 15, new PasswordState }

#define ConfirmationID 16
#define ConfirmationST { 16, new ConfirmationState }

#define TestGroundID 50
#define TestGroundST { 50, new TestGroundState }


namespace globalvar {

	inline float dt;
    inline Clock dtclock;
	inline RenderWindow* window;
	inline map<int, State*>* states;
	inline Event event;

	inline int fps = 0, frame_count = 0, frame_sum = 0;
	inline bool fps_active = 0, fullscreen = 0, vsync = 0, exit_game = 0, active_cursor = 1, clickable_cursor = 0;
    inline float delay = 0;
    inline double game_time = 0;
    inline short quest_idx = -1, game_volume = 100, music_volume = 100;
    inline string character_name;

	inline Font font;
	inline Text fps_text, text;
	inline string notification_strings[3] = {""};
	inline int notification_lines = 1;
	inline float notification_delay = 0;

    inline random_device real_randomness;
    inline mt19937 mt(real_randomness());

    inline string item_names[50] = {"Wood", "Stone"};

	inline const int dx[4] = { 1, -1, 0, 0 };
	inline const int dy[4] = { 0, 0, 1, -1};

    inline float blackining = 0;

	struct dialogue {
		string speaker, text;
		short expression = 0, pic = 1; //pic is 1-based, expression is 0-based
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

    inline int generate_random(float mn = 0, float mx = FLT_MAX-1) {
        uniform_real_distribution<float> dist(mn, mx+1);
        int rando = dist(mt);
        return (rando > mx) ? mx : rando;
    }

    inline float generate_random_f(float mn = 0, float mx = FLT_MAX - 1) {
        uniform_real_distribution<float> dist(mn, mx);
        float rando = dist(mt);
        return rando;
    }

    struct nod {
        int itm;
        struct nod* link = NULL, * back_link = NULL;
    };

    struct in_order {
        int size = 0;
        nod* first, *last;
        /* insert function */
        in_order() {
            first = NULL;
            last = NULL;
        }

        ~in_order() {
            clear();
        }

        void add(int inputed) {

            nod* tmp = new nod;

            tmp->itm = inputed;

            tmp->link = NULL;

            if (first == NULL)
                first = last = tmp;
            else
                tmp->back_link = last, last->link = tmp, last = tmp;

            size++;

        }

        /*Clear function*/
        void clear() {
            while (!empty())
                erase(first->itm);

        }

        /* empty function */
        bool empty() {
            if (first == NULL)
                return 1;
            else return 0;
        }

        /* erase */
        bool erase(int element) {
            bool found = 0;

            nod* node = first;

            if (element == first->itm) {
                if (first->link != NULL)
                    first->link->back_link = NULL;
                else
                    last = NULL;
                first = first->link;
                delete node;
                size--;
                return 1;
            }
            else if (element == last->itm) {
                node = last;
                last = last->back_link;
                last->link = NULL;
                size--;
                delete node;
                return 1;
            }

            for (; node != NULL; node = node->link) {
                if (node->itm == element) {
                    found = 1;
                    if (node->link != NULL)
                        node->link->back_link = node->back_link;
                    if (node->back_link != NULL)
                        node->back_link->link = node->link;
                    size--;
                    delete node;
                    break;
                }
            }

            if (size == 1) {
                first->link = NULL, first->back_link = NULL, last = NULL;
            }
            return found;
        }
    };

}