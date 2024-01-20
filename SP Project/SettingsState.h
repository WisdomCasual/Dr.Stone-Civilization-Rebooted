#pragma once
#include "State.h"
#include"MainMenuState.h"
#include"MapBuilderState.h"
#include "WorldMapState.h"
#include "Game.h"
// Limitations for sliders 
/*The game has nine standered resolutions and a custom resolution slot which can be accessed through the config file*/
#define resnum 10
#define framelimnum 7

#include"Global.h"
using namespace globalvar;

using namespace std;
using namespace sf;

struct SettingsState :
    public State
{
private:
    /*Textures for everything was included in here and stored in an int rect arrays with their (coordinates,size)*/
    const IntRect tipsright[5] = { {190,330,9,18} ,{190,348,9,18} ,{372,312,9,18}, { 190,294,9,18 }, {190,312,9,18} },
        tipsleft[5] = { {370,126,9,18} ,{370,90,9,18} ,{372,294,9,18},{ 372,330,9,18 }, {370,108,9,18} },
        mids[5] = { {338,449,18,18} ,{356,368,18,18} ,{356,431,18,18},{338,386,18,18}, {338,368,18,18} },
        arrow = { 303,486,22,21 },
        buttons[4] = { {290,139,93,94} ,{240,0,93,94} ,{290,49,93,94},{201,296,92,92} },
        checkMark = {369,184,16,15};
    //Colors loaded in arrangment : Yellow -> Red -> Blue -> Green -> Hollow
    Vector2f clicked_on = { -1, -1 };
    Sprite tissue, back_arrow, devbutton, checkbox, checkmark, panel;
    Text devtext, slider_text;
    RectangleShape tint;
    float x = 0, y = 0;
    float win_x = 0, win_y = 0, scale = 1, previous_scale = 1, transparency = 0, darkness = 0;
    bool button_pressed = 0, arrow_pressed = 0, dev_button_active = 1, exit = 0, bg_fade = 1, vsync_box = vsync, fullscreen_box = fullscreen;
    Vector2f mouse_pos = { 0,0 };
    Vector2u prev_win = { 0, 0 };
    Sprite tip;
    int save_framelimit = 120, save_volume = 100, save_music = 100;

    short resolution = resnum, framelimit = framelimnum, prev_resolution = resnum, prev_framelimit = framelimnum;
    VideoMode resolutions[resnum + 1] = { {800, 600}, {1280, 720}, {1280, 800}, {1366, 768}, {1920, 1080}, {1920, 1200}, {2560, 1440}, {2560, 1600}, {3840, 2160}, {3840, 2400} };
    string framelimits[framelimnum + 1] = { "30", "60", "90", "120", "144", "165", "Unlocked"}; //<-- last slot reserved for custom paramaters ^
    short framelimit_values[framelimnum + 1] = { 30, 60, 90, 120, 144, 165, 5000 };
    string resolutions_text[resnum + 1];
    string framelimits_text[framelimnum + 1];

    struct slider_info {
        short int x = 0, y = 0, mx = 1, color = 0;

        // linker is used to link functionalities of specific sliders to their actual job.

        short int* linker = nullptr;
        string name = "";
        char text_type = 0;     //0 = plain text, 1 = percentage (a%), 2 = custom (link string array)
        string* txt = nullptr;
        short int tipx = 0;
        float midscale = 1;
        bool presssed = 0, disabled = 0, released = 0;

        //initiate positions of sliders and assign them to their objective variables

    }sliders[4] = { {-105, -10, resnum-1, 0, &resolution, "Resolution", 2, resolutions_text}, {-105, 5, framelimnum-1, 1, &framelimit, "Frame Limit", 2, framelimits_text}, {-105, 20, 100, 2, &game_volume, "Game Sounds", 1}, {-105, 35, 100, 2, &music_volume, "Music", 1}};
    
    // added for vsync and fullscreen mode

    struct CheckBox {
        short int x = 0, y = 0;
        string name = "";
        bool* execute;
        bool checked = 0, hover = 0, pressed = 0, released = 0;
    } checkboxes[2];



    //private functions:
    void fade_in();
    bool fade_out();
    void update_arrow();
    void dev_button();
    void update_slider(slider_info*, int);
    void render_slider(int);
    void settings_intializer();
    void update_checkbox(int);
    void render_checkbox(int);

public:
    //constructors / destructors
    SettingsState(bool bg_fade = true);

    ~SettingsState();

    void update();

    void pollevent();

    void render();
};