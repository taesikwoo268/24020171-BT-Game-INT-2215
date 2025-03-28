
#include "map.h"
#include "texture.h"
#include "CommonFunc.h"
#include "player.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


Map::Map()
{
    dirt = texture::LoadTexture("image/dirt.png");
    if (dirt == NULL) cout <<SDL_GetError();
    grass = texture::LoadTexture("image/grass.png");
    stone = texture::LoadTexture("image/stone.png");
    red_grass = texture::LoadTexture("image/red_grass.png");


    src.x = 0;
    src.y = 0;
    src.h = 16;
    src.w = 16;

    dest.x = 0;
    dest.y = 0;
    dest.h = 16;
    dest.w = 16;

    LoadMap("map_1.txt");
}


void Map::LoadMap(const char* path)
{
    ifstream file;
    file.open(path);
    if (!file.is_open())
    {
        cout << "File error!";
    }
    else
    {
        for (int i=0;i<240;i++){
            for (int j=0;j<60;j++) {
                file >> mapping[i][j];
                tile[i][j].x=j*TILE_SIZE;
                tile[i][j].y=i*TILE_SIZE;
                tile[i][j].w=TILE_SIZE;
                tile[i][j].h=TILE_SIZE;
            }
        }
    }

}



bool Map::checkCollision(SDL_Rect a, SDL_Rect b) {
    return SDL_HasIntersection(&a,&b);
}

//ve map
void Map::DrawMap(SDL_Rect Camera) {
    int type = 0;
    for (int row = 0; row < 240; row++) {
        for (int column = 0; column < 60; column++){
            type = mapping[row][column];

            dest.x = column * 16;
            dest.y = row * 16;
            if (checkCollision(Camera, tile[row][column]))
            {
                dest.y -= Camera.y;
                switch (type)
                {
                case 0:
                    {
                        texture::Draw(dirt, src, dest);
                        break;
                    }
                case 1:
                    {
                        texture::Draw(grass, src, dest);
                        break;
                    }
                case 2:
                    {
                        texture::Draw(stone, src, dest);
                    break;
                    }
                case 8:
                    {
                        texture::Draw(red_grass, src, dest);
                        break;
                    }
                default:
                    break;
                }
            }
        }
    }

}

void Map::CloseMap()
{
    SDL_DestroyTexture(dirt);
    SDL_DestroyTexture(stone);
    SDL_DestroyTexture(grass);
    SDL_DestroyTexture(red_grass);
    dirt = NULL;
    stone = NULL;
    grass = NULL;
    red_grass = NULL;
}

