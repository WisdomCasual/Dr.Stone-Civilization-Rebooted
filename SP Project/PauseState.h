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
        Sprite tissue;
        RectangleShape tint;
        float x = 0, y = 0;
        float win_x = 0, win_y = 0, scale = 1, previous_scale = 1;
        bool button_pressed = 0;
        Vector2i mouse_pos = { 0,0 };

        Sprite buttontex, logo;
        Text button_text;
        struct button {
            string txt;
            int x = 0, y = 0;
            bool& execute;
            int pressed = 0;
            bool hover = 0;
        };
        vector<button> buttons;
        bool resume = 0, settings = 0, exit = 0;


        //private functions:
        void update_buttons();
        void render_buttons();

    public:
        //constructors / destructors
        PauseState();

        ~PauseState();

        void update();
        void pollevent();
        void render();
};
