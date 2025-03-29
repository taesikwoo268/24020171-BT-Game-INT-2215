#pragma once
#ifndef COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
using namespace std;

const int FPS = 60;
const int frameDelay = 1000 / FPS;
static const int SCREEN_WIDTH = 960;
static const int SCREEN_HEIGHT = 720;
const int LEVEL_WIDTH = 960;
const int LEVEL_HEIGHT = 3840;
const int TILE_SIZE = 16;
enum gameStates
{
    MainMenu,
    Guide,
};
class Game
{
private:

    bool isRunning;
    bool win;
    SDL_Window* window = NULL;

public:

    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    bool isStart;
    void handleEvents();
    void update();
    void render();
    void clean();
    gameStates state;
    int createMenu(TTF_Font* font);
    int createGuide(TTF_Font* font);
    void Ending();
    void record();
    bool checkSelected(const int& x, const int& y, const SDL_Rect& rect);
    bool running();
    bool winning();
    void setWin(bool b);

    static SDL_Renderer* renderer;

};

#endif // COMMON_FUNCTION_H_

