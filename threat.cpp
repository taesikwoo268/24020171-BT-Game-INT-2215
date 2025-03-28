#include "threat.h"
#include "texture.h"
#include "player.h"

void Threat::setClips()
{
    ThreatAnimation[0].x = 0;
    ThreatAnimation[0].y = 0;
    ThreatAnimation[0].w = 128;
    ThreatAnimation[0].h = 131;

    ThreatAnimation[1].x = 168;
    ThreatAnimation[1].y = 0;
    ThreatAnimation[1].w = 128;
    ThreatAnimation[1].h = 131;
}
Threat::Threat(float x, float y)
{
    xpos = x;
    ypos = y;
    xvel = MOVE_SPEED;
    yvel = 0;
    on_ground = false;
    direction.left=false;
    direction.right=true;

    threat_Texture = texture::LoadTexture("image/Enemy.png");
    frame =0;
    setClips();

    threatRect.x = (int)xpos;
    threatRect.y = (int)ypos;
    threatRect.w = THREAT_WIDTH;
    threatRect.h = THREAT_HEIGHT;
}

bool Threat::CheckCollisionVertical(SDL_Rect Tile[][60], int Mapping[][60])
{

    ypos += yvel;
    threatRect.y = (int)ypos;

    for (int row = 0; row < 240; row++)
    {
        for (int column = 0; column < 60; column++)
        {
            if (Mapping[row][column] == 3) continue;

            if (SDL_HasIntersection(&threatRect, &Tile[row][column]))
            {
                if (yvel > 0)
                {
                    ypos = Tile[row][column].y - THREAT_HEIGHT;
                    threatRect.y = (int)ypos;
                    yvel = 0;
                    on_ground = true;
                    return true;
                }
                else if (yvel < 0)
                {
                    ypos = Tile[row][column].y + Tile[row][column].h;
                    threatRect.y = (int)ypos;
                    yvel = 0;
                    return true;
                }
            }
        }
    }

    on_ground = false;
    return false;
}

bool Threat::CheckCollisionHorizontal(SDL_Rect Tile[][60], int Mapping[][60])
{
    xpos += xvel;
    threatRect.x = (int)xpos;

    for (int row = 0; row < 240; row++)
    {
        for (int column = 0; column < 60; column++)
        {
            if (Mapping[row][column] == 3) continue;
            if  (Mapping[row][column] == 7&& SDL_HasIntersection(&Tile[row][column],&threatRect))
            {
                xvel = -xvel;
                direction.left = !direction.left;
                direction.right = !direction.right;
                xpos -= xvel * 2;
                threatRect.x = (int)xpos;
                return true;
            }
        }
    }

    return false;
}
void Threat::moveRight()
{
    xpos += MOVE_SPEED;
    threatRect.x = (int)xpos;
}
void Threat::moveLeft()
{
    xpos -= MOVE_SPEED;
    threatRect.x = (int)xpos;
}

void Threat::Update(SDL_Rect Tile[][60], int Mapping[][60], SDL_Rect& Camera)
{
    if (!on_ground)
    {
        yvel += gravity;
        if (yvel > MAX_FALL_SPEED) yvel = MAX_FALL_SPEED;
    }

    CheckCollisionVertical(Tile, Mapping);
    CheckCollisionHorizontal(Tile, Mapping);


    renderRect = threatRect;
    renderRect.y -= Camera.y;
}

void Threat::Render(SDL_Rect& Camera)
{
    if (SDL_HasIntersection(&threatRect,&Camera))
    {
        SDL_Rect* currentClip;
        frame++;
        if (frame>1) frame = 0;
        currentClip = &ThreatAnimation[frame];
        SDL_RenderCopy(Game::renderer,threat_Texture,currentClip,&renderRect);
    }
}
void Threat::closeThreat()
{
    SDL_DestroyTexture(threat_Texture);
    threat_Texture = NULL;
}
