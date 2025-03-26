#include <iostream>
#include <string>
#include <stdio.h>
#include "CommonFunc.h"
#include "texture.h"

Game* game = nullptr;

int main(int argc, char* args[]){

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();

    game->init("JumpKing!!!!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);


    while (game->running() == true)
    {
        if (game->winning() == false)
        {
            frameStart = SDL_GetTicks();

            game->handleEvents();
            game->update();
            game->render();

            frameTime = SDL_GetTicks() - frameStart;

            if (frameDelay > frameTime)
            {
                SDL_Delay(frameDelay - frameTime);
            }
        }
        else
        {
            game->Ending();
            SDL_Delay(7000);
            game->setWin(false);
        }
    }
    game->clean();


    return 0;
}
