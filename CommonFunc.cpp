#include "CommonFunc.h"
#include "texture.h"
#include "textObj.h"
#include "player.h"
#include "map.h"


SDL_Texture* background = NULL;
SDL_Texture* foreground = NULL;
SDL_Texture* victory = NULL;
SDL_Texture* imgStart = NULL;
SDL_Texture* babe = NULL;



Mix_Music* Music = NULL;
Mix_Music* BgMusic = NULL;
TTF_Font* font = NULL;


SDL_Rect babeSrcRect = { 0,0,48,48 };
SDL_Rect babeDestRect = { 500,112,48,48 };//luu vi tri theo camera
SDL_Rect babeRect = { 500,112,48,48 };//luu vi tri co dinh

Uint32  timeVal, startTime;


SDL_Rect BgSrc = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }, BgDest = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
GameObject* player;
textObj timeGame;
Map* mapper;


SDL_Renderer* Game::renderer = nullptr;

Game::Game()
{}
Game::~Game()
{}


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flag = 0;
    if (fullscreen)
    {
        flag = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        cout << "Initialized..." << endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flag);
        if (window)
        {
            cout << "Window created!" << endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            cout << "Renderer created!" << endl;
        }
        if (TTF_Init() == 0)
        {
            font = TTF_OpenFont("font/font2.ttf", 24);
        }

    }
    else
    {
        isRunning = false;
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Music = Mix_LoadMUS("sound/win.wav");
    BgMusic = Mix_LoadMUS("sound/bgMusic.wav");
    Mix_VolumeMusic(MIX_MAX_VOLUME/7);
    if (Music==NULL||BgMusic==NULL) cout << SDL_GetError();


    win = false;


    //load back and fore ground
    background = texture::LoadTexture("image/background.png");
    if (background==NULL) cout <<1<< SDL_GetError()<<endl;
    foreground = texture::LoadTexture("image/foreground.png");
    babe = texture::LoadTexture("image/babe.png");
    victory = texture::LoadTexture("image/victory.png");
    imgStart = texture::LoadTexture("image/main_menu.png");


    player = new GameObject(64, LEVEL_HEIGHT - 100);
    mapper = new Map();
    startTime = 0;

    bool isBegin = Start();
    if (isBegin)
    {
        isRunning = true;
        startTime = SDL_GetTicks()/1000;
    }
    else
    {
        isRunning = false;
    }

}


void Game::handleEvents()
{

    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
    {
        isRunning = false;
    }
    else if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            {
        case SDLK_RIGHT:
        {
            player->inputType.right = 1;
            player->inputType.left = 0;
            break;
        }
        case SDLK_LEFT:
        {
            player->inputType.left = 1;
            player->inputType.right = 0;
            break;
        }
        case SDLK_UP:
        {
            player->inputType.up = 1;
            break;
        }

        case SDLK_SPACE:
        {
            player->PrepareJump();
            break;
        }
            }
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_RIGHT:
        {
            player->inputType.right = 2;
            break;
        }
        case SDLK_LEFT:
        {
            player->inputType.left = 2;
            break;
        }
        case SDLK_UP:
        {
            player->inputType.up = 2;
            break;
        }
        case SDLK_SPACE:
            if (player->onGround == true)
            {
                switch (player->inputType.jump)
                {
                case 0:
                        player->Jump();
                        break;

                case 1:
                        player->JumpRight();
                        break;

                case 2:
                        player->JumpLeft();
                        break;
                }
            }

        }
    }
}
void Game::update()
{


    player->Update(mapper->tile, mapper->mapping);
    babeDestRect.y = babeRect.y - player->Camera.y;


    if (player->isWin == true)
    {
        win = true;
        if (Mix_PlayingMusic() == 0)
        {
            Mix_PlayMusic(Music, -1);
        }
    }
    if (player->isWin == false)
    {
        if (!Mix_PlayingMusic())
        {
            Mix_PlayMusic(BgMusic, -1);
        }
    }
}

bool Game::Start()
{
    texture::Draw(imgStart,player->Camera,BgDest);
    SDL_Event e;
    while (true)
    {
        texture::Draw(imgStart,player->Camera,BgDest);
        while (SDL_PollEvent(&e))
        {
            switch(e.key.keysym.sym)
            {
            case SDLK_y:
                return true;
            case SDLK_n:
                return false;
            }

        }
        SDL_RenderPresent(renderer);
    }
    return true;
}
void Game::Ending()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, victory, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void Game::render()
{
    SDL_RenderClear(renderer);
    mapper->DrawMap(player->Camera);
    texture::Draw(background, player->Camera, BgDest);

    player->Render();
    texture::Draw(foreground, player->Camera, BgDest);
    texture::Draw(babe, babeSrcRect, babeDestRect);



    //Time counting and render on the screen
    string strTime = "TIME: ";
    timeVal = SDL_GetTicks() / 1000 - startTime;
    string timeRes = to_string(timeVal);
    strTime += timeRes;
    timeGame.setText(strTime);
    timeGame.setTextColor(red);
    timeGame.loadFromRenderedText(font, renderer);
    timeGame.renderText(renderer, 25, 15);
    SDL_RenderPresent(renderer);

}



void Game::clean()
{
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyTexture(background);
    background = NULL;
    SDL_DestroyTexture(foreground);
    foreground = NULL;
    SDL_DestroyTexture(babe);
    babe = NULL;
    SDL_DestroyTexture(victory);
    victory = NULL;


    player->ObjectClose();
    player = NULL;
    mapper->CloseMap();
    mapper = NULL;
    TTF_CloseFont(font);
    font = NULL;

    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
    cout << "Game cleaned" << endl;
}

void Game::setWin(bool b)
{
    isRunning= b;
}

bool Game::running()
{
    return isRunning;
}
bool Game::winning()
{
    return win;
}
