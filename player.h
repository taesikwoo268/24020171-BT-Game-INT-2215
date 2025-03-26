
#pragma once
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "CommonFunc.h"

#define gravity 0.8
#define MAX_FALL_SPEED 20

using namespace std;

/// condition of the player
enum condition { standing, running, charging, jumping };

/// check input key
typedef struct {
    int left;
    int right;
    int up;
    int jump;
}
Input;



class GameObject
{
private:

    double xpos;
    double ypos;

    double xvel;
    double yvel;

    int frame;

    Uint32 startTime;
    int jumpTime;


    SDL_Texture* objTextureRight = NULL;
    SDL_Texture* objTextureLeft = NULL;
    SDL_Texture* objTexturePJump = NULL;
    SDL_Rect srcRect, destRect, collider;

    // nhung am thanh hieu ung ngan
    Mix_Chunk* High = NULL;
    Mix_Chunk* Step = NULL;
    Mix_Chunk* Jumped = NULL;


    // for running animation
    SDL_Rect RunningRight[5];
    SDL_Rect RunningLeft[5];
    SDL_Rect ChargingJump;
    SDL_Rect JumpingLeft[2];
    SDL_Rect JumpingRight[2];

    // input and condition
    condition status;
    Input inputType;
    bool onGround;
    bool isWin;


public:
    GameObject(int x, int y);
    ~GameObject();

    friend void Game::handleEvents();
    friend void Game::update();

    SDL_Rect GetCollider() { return collider; }


    void Update(SDL_Rect Tile[][60], int Mapping[][60]);

    void RunLeft();
    void RunRight();
    void PrepareJump();
    void Jump();
    void JumpLeft();
    void JumpRight();

    void StopRunRight();
    void StopRunLeft();

    void SetClips();
    void Render();
    void ObjectClose();
    //va cham
    void CollideVertical(SDL_Rect& col, SDL_Rect Tile[][60], int Mapping[][60]);
    void CollideHorizontal(SDL_Rect& col, SDL_Rect Tile[][60], int Mapping[][60]);
    bool checkCollision2(SDL_Rect a, SDL_Rect b);


    int Getxvel() { return (int)xvel; }
    int Getyvel() { return (int)yvel; }


    static const int maxxspeed = 6;
    static const int maxyspeed = 8;

    static const int KING_WIDTH = 48;
    static const int KING_HEIGHT = 48;

    // camera
    SDL_Rect Camera = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };


};


#endif
