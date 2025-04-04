
#pragma once
#ifndef MAP_H_
#define MAP_H_

#include "CommonFunc.h"
#include "player.h"
#include "threat.h"

class Map
{
private:
    SDL_Rect src, dest;

    SDL_Texture* dirt = NULL;
    SDL_Texture* grass = NULL;
    SDL_Texture* red_grass = NULL;
    SDL_Texture* stone = NULL;

    int mapping[240][60];
    SDL_Rect tile[240][60];

public:

    Map();
    ~Map();


    void LoadMap(const char*path);
    void DrawMap(SDL_Rect Camera);
    void CloseMap();

    bool checkCollision(SDL_Rect a, SDL_Rect b);

    friend void GameObject::Update(SDL_Rect Tile[][60], int Mapping[][60]);
    friend void Game::update();

};
#endif
