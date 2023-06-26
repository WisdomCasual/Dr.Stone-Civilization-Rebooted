#pragma once
#include "State.h"
#include"Game.h"

#include"Global.h"
using namespace globalvar;

class InventoryState : public State
{
private:
    Sprite tissue, panel, item;
    RectangleShape tint;
    float x = 0, y = 0;
    float win_x = 0, win_y = 0, scale = 1, previous_scale = 1;
    in_order* inventory_order;
    unsigned short* inventory_count;
    Vector2u prev_win = { 0, 0 };
    float panel_pos = 0, velocity = 0, darkness = 0;
    bool close = false;
    nod* iteration_start = NULL;
    int scroll_offset = 0, item_offset = 0;

    //private functions:
    void render_items();

public:
    //constructors / destructors
    InventoryState(in_order*, unsigned short*);
    ~InventoryState();

    void update();
    void pollevent();
    void render();
};

