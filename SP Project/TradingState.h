#pragma once
#include"State.h"
#include"Global.h"

using namespace globalvar;
using namespace std;
using namespace sf;

struct ingredient {
    short ID, count;
};

struct TradingMenu {
    String item_name;
    short ingredients_count;
    ingredient ingredients[3];
    ingredient result;
};

struct TradingInfo {
    ~TradingInfo() {
        if (trading_menu != nullptr)
            delete[] trading_menu;
    }
    string texture_name; short trades_count = 0;
    String dialogue;
    TradingMenu* trading_menu = nullptr;
};


class TradingState:
    public State
{
private:

    struct button {
        int y = 0, item_id = 0;
        int pressed = 0;
        bool hover = 0, disabled = 0;
    } *buttons = nullptr;

    in_order* inventory_order;
    unsigned short* inventory_count;

    Vector2f clicked_on = { -1, -1 };
    Texture pic_tex;
    Sprite tissue, panel, get_button, items, pic;
    Text button_text;
    RectangleShape tint;
    float x = 0, y = 0;
    float win_x = 0, win_y = 0, scale = 1, previous_scale = 1, transparency = 0, darkness = 0;
    bool exit = 0;
    int scroll_offset = 0;
    Vector2f mouse_pos = { 0,0 };
    Vector2u prev_win = { 0, 0 };
    TradingInfo& trading_info;

    string output_strings[10], next_word;
    int lines = 1;


    //private functions:
    void fade_in();
    bool fade_out();
    void update_buttons();
    void render_buttons();
    void render_trades();
    bool in_bounds(float);
    void set_dialogue_lines();

public:
    //constructors / destructors
    TradingState(in_order*, unsigned short*, TradingInfo&);

    ~TradingState();

    void update();

    void pollevent();

    void render();
};