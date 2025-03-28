#pragma once
#ifndef THREAT_H_
#define THREAT_H_

#include "CommonFunc.h"
class GameObject;
#include "map.h"
struct Dir
{
    bool left;
    bool right;
};
class Threat
{
private:
    float xpos, ypos;
    float xvel, yvel;
    SDL_Rect threatRect;
    SDL_Rect renderRect;
    bool on_ground;
    Dir direction;
    SDL_Texture* threat_Texture;
    SDL_Rect ThreatAnimation[2];
    int frame;

    const int THREAT_WIDTH = 24;
    const int THREAT_HEIGHT = 24;
    const float MOVE_SPEED = 1;


public:
    Threat(float x, float y);
    friend class GameObject;

    void Update(SDL_Rect Tile[][60], int Mapping[][60], SDL_Rect& Camera);
    void Render(SDL_Rect& Camera);
    void moveRight();
    void moveLeft();

    void setClips();
    bool CheckCollisionVertical(SDL_Rect Tile[][60], int Mapping[][60]);
    bool CheckCollisionHorizontal(SDL_Rect Tile[][60], int Mapping[][60]);

    SDL_Rect GetRect() const { return threatRect; }
    float GetXPos() const { return xpos; }
    float GetYPos() const { return ypos; }
    int GetWidth() const { return THREAT_WIDTH; }
    int GetHeight() const { return THREAT_HEIGHT; }
    void closeThreat();
};

#endif // THREAT_H_
