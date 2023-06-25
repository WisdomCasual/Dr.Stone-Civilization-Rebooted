#pragma once
#include "State.h"
#include "SettingsState.h"

#include"Global.h"
using namespace globalvar;

using namespace std;
using namespace sf;


class InventoryState : public State
{
private:
    Sprite tissue, panel, item;
    RectangleShape tint;
    float x = 0, y = 0;
    float win_x = 0, win_y = 0, scale = 1, previous_scale = 1;
    Vector2u prev_win = { 0, 0 };
    float panel_pos = 0, velocity = 0, darkness = 0;
    bool close = false;


public:
    //constructors / destructors
    InventoryState();
    ~InventoryState();

    void update();
    void pollevent();
    void render();
};

