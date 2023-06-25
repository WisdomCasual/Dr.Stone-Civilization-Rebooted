#pragma once
#include "State.h"
#include "SettingsState.h"

#include"Global.h"
using namespace globalvar;

using namespace std;
using namespace sf;

struct PauseState : public State
{
private:
    bool presssed = 0;
    Vector2f clicked_on = { -1, -1 };
    Sprite tissue, panel;
    RectangleShape tint;
    float x = 0, y = 0;
    float win_x = 0, win_y = 0, scale = 1, previous_scale = 1, transparency = 0, darkness = 0;
    bool button_pressed = 0, bg_fade_in = 1;
    Vector2i mouse_pos = { 0,0 };
    Vector2u prev_win = { 0, 0 };

    Sprite buttontex, logo;
    Text button_text;
    struct button {
        string txt;
        int x = 0, y = 0;
        bool* execute = nullptr;
        int pressed = 0;
        bool hover = 0;
    } buttons[3] = {{ "Resume",0,-48 }, { "Settings",0,16 }, { "Save and Exit" ,0,80 }};

        bool resume = 0, settings = 0, exit = 0;


        //private functions:
        void update_buttons();
        void render_buttons();
        void fade_in();
        bool fade_out(bool bg_fade_out = true);

    public:
        //constructors / destructors
        PauseState(bool bg_fade_in = true);

        ~PauseState();

        void update();
        void pollevent();
        void render();
};

