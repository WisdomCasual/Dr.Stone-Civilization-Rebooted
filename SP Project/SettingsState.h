#pragma once
#include "State.h"
#include"MainMenuState.h"
#include"MapBuilderState.h"
#include "Game.h"

#define resnum 10
#define framelimnum 6

#include"Global.h"
using namespace globalvar;

using namespace std;
using namespace sf;

struct SettingsState :
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
    Vector2f clicked_on = { -1, -1 };
    Sprite tissue, back_arrow, devbutton;
    Text devtext, slider_text;
    RectangleShape tint;
    float x = 0, y = 0;
    float win_x = 0, win_y = 0, scale = 1, previous_scale = 1;
    bool button_pressed = 0, arrow_pressed = 0, dev_button_active = 1, test = 1;
    Vector2f mouse_pos = { 0,0 };
    Sprite tip;
    VideoMode save_resolution = { 1920, 1080 };
    string title = "";
    int save_framelimit = 120, save_volume = 100, save_music = 100;
    bool save_vsync = 0;

    short int resolution = resnum, framelimit = framelimnum, game_volume = 100, music_volume = 100;
    VideoMode resolutions[resnum + 1] = { {800, 600}, {1280, 720}, {1280, 800}, {1366, 768}, {1920, 1080}, {1920, 1200}, {2560, 1440}, {2560, 1600}, {3840, 2160}, {3840, 2400} };
    short int framelimits[framelimnum + 1] = { 30, 60, 90, 120, 144, 165 }; //<-- last slot reserved for custom paramaters ^
    string resolutions_text[resnum + 1];
    string framelimits_text[framelimnum + 1];
    


    struct slider_info {
        short int x = 0, y = 0, mx = 1, color = 0;
        short int* linker = nullptr;
        string name = "";
        char text_type = 0;     //0 = plain text, 1 = percentage (a%), 2 = custom (link string array)
        string* txt = nullptr;
        short int tipx = 0;
        float midscale = 1;
        bool presssed = 0;

    }sliders[4] = { {-105, -15, resnum-1, 0, &resolution, "Resolution", 2, resolutions_text}, {-105, 0, framelimnum-1, 1, &framelimit, "Frame Limit", 2, framelimits_text}, {-105, 15, 100, 2, &game_volume, "Game Sounds", 1}, {-105, 30, 100, 2, &music_volume, "Music", 1}};

    //private functions:
    void update_arrow();
    void dev_button();
    void update_slider(slider_info*, int);
    void render_slider(int);
    void settings_intializer();
public:
    //constructors / destructors
    SettingsState();

    ~SettingsState();

    void scaling();

    void update();

    void pollevent();

    void render();
};