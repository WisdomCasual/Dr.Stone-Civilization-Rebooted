#pragma once
#include "State.h"
#include"Game.h"

#include"Global.h"
using namespace globalvar;

struct InventoryState : public State
{
private:

    struct button {
        int y = 0, item_id = 0;
        int pressed = 0;
        bool hover = 0;
    }*buttons = nullptr;

    Sprite tissue, panel, item, consumebutton, heart;
    RectangleShape tint;
    float x = 0, y = 0;
    float win_x = 0, win_y = 0, scale = 1, previous_scale = 1;
    Vector2f clicked_on = { -1, -1 };
    in_order* inventory_order;
    unsigned short* inventory_count;
    Vector2u prev_win = { 0, 0 };
    float panel_pos = 0, velocity = 0, darkness = 0, *combat_tag;
    bool close = false;
    nod* iteration_start = NULL;
    int scroll_offset = 0, item_offset = 0;
    short* player_health, player_max_health;

    //private functions:
    void render_items();
    void update_buttons();
    void render_buttons();

public:
    //constructors / destructors
    InventoryState(in_order*, unsigned short*, short*, short, float*);
    ~InventoryState();

    void update();
    void pollevent();
    void render();
};

