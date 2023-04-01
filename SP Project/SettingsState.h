#pragma once
#include "State.h"
#include"MainMenuState.h"
#include"MapBuilderState.h"

using namespace std;
using namespace sf;

class SettingsState :
    public State
{
private:
    IntRect tipsright[4] = { {190,330,9,18} ,{190,348,9,18} ,{372,312,9,18}, { 190,294,9,18 } },
        tipsleft[4] = { {370,126,9,18} ,{370,90,9,18} ,{372,294,9,18},{ 372,330,9,18 } },
        mids[4] = { {338,449,18,18} ,{356,368,18,18} ,{356,431,18,18},{338,386,18,18} },
        arrow = { 303,486,22,21 },
        buttons[4] = { {290,139,93,94} ,{240,0,93,94} ,{290,49,93,94},{201,296,92,92} },
        checkMark = { 370,45,16,15 };
    //Colors loaded in arrangment : Yellow -> Red -> Blue
    bool presssed = 0;
    Sprite tissue, back_arrow, devbutton;
    Text devtext;
    RectangleShape tint;
    float x = 0, y = 0;
    float win_x = 0, win_y = 0, scale = 1, previous_scale = 1;
    bool button_pressed = 0, arrow_pressed = 0;
    Vector2i mouse_pos = { 0,0 };

    Sprite tip;
    struct slider_info {
        int x = 0, y = 0, length = 0, color = 0, tipleftx = 0, tiprightx = 0, midx = 0;

    }info[3] = { {  0, 0, 20, 1 } };

    //private functions:
    void update_arrow(RenderWindow*, int*, deque<State*>*);
    void dev_button(RenderWindow*, int*, deque<State*>*);
    void update_slider(RenderWindow*, int, int, int,int, string);
    void render_slider(RenderWindow*,int);
public:
    //constructors / destructors
    SettingsState();

    ~SettingsState();

    void update(float, RenderWindow*, int*, deque<State*>*);

    void pollevent(Event, RenderWindow*);

    void render(RenderWindow*);
};