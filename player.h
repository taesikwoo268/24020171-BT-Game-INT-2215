
#pragma once
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "CommonFunc.h"
class Threat;


#define gravity 0.8
#define MAX_FALL_SPEED 20

using namespace std;

enum condition { standing, running, charging, jumping };


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


    // for animation
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
    void ThreatAttack(Threat& threat);

    SDL_Rect GetCollider() { return collider; }


    void Update(SDL_Rect Tile[][60], int Mapping[][60]); //dong 357

    void RunLeft();
    void RunRight();
    void PrepareJump();
    void Jump();
    void JumpLeft();
    void JumpRight();

    void StopRunRight();
    void StopRunLeft();

    void SetClips();
    void Render();//dong 404
    void ObjectClose();//dong 469
    //va cham
    void CollideVertical(SDL_Rect& col, SDL_Rect Tile[][60], int Mapping[][60]);
    void CollideHorizontal(SDL_Rect& col, SDL_Rect Tile[][60], int Mapping[][60]);//179-205-231
    bool checkCollision2(SDL_Rect a, SDL_Rect b);



    int Getxvel() { return (int)xvel; }
    int Getyvel() { return (int)yvel; }


    static const int maxxspeed = 6;
    static const int maxyspeed = 8;

    static const int KING_WIDTH = 48;
    static const int KING_HEIGHT = 48;

    // camera
    SDL_Rect Camera = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    SDL_Rect getCamera() {return Camera;}


};

#endif
