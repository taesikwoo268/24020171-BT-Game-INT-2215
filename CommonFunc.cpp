#include "CommonFunc.h"
#include "texture.h"
#include "textObj.h"
#include "player.h"
#include "map.h"
#include "threat.h"
#include <fstream>
#include <vector>


SDL_Texture* background = NULL;
SDL_Texture* foreground = NULL;
SDL_Texture* victory = NULL;
SDL_Texture* imgStart = NULL;
SDL_Texture* imgGuide = NULL;
SDL_Texture* babe = NULL;



Mix_Music* Music = NULL;
Mix_Music* BgMusic = NULL;
Mix_Chunk* Beep = NULL;
TTF_Font* font = NULL;
TTF_Font* fontMenu = NULL;


SDL_Rect babeSrcRect = { 0,0,48,48 };
SDL_Rect babeDestRect = { 500,112,48,48 };//luu vi tri theo camera
SDL_Rect babeRect = { 500,112,48,48 };//luu vi tri co dinh

Uint32  score,hscore,timeVal, startTime;


SDL_Rect BgSrc = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }, BgDest = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

GameObject* player;
textObj timeGame;
textObj HighestScore;
Map* mapper;
vector<Threat*> threats;


SDL_Renderer* Game::renderer = nullptr;

Game::Game()
{}
Game::~Game()
{}

int Game::createMenu(TTF_Font* font){
    imgStart = texture::LoadTexture("image/main_menu.png");
    if (imgStart == NULL) return 1;
    texture::Draw(imgStart, player->Camera, BgDest);

    // 0 = play, 1 = guide,2 = exit;
    const int numMenu = 3;
    SDL_Rect menuRect[numMenu];

    menuRect[0].x = 550;
    menuRect[0].y = 360;
    menuRect[0].w = 96;
    menuRect[0].h = 32;

    menuRect[1].x = 550;
    menuRect[1].y = 420;
    menuRect[1].w = 96;
    menuRect[1].h = 32;

    menuRect[2].x = 550;
    menuRect[2].y = 480;
    menuRect[2].w = 96;
    menuRect[2].h = 32;

    textObj textMenu[numMenu];

    textMenu[0].setText("PLAY");
    textMenu[0].setTextColor(white);
    textMenu[1].setText("GUIDE");
    textMenu[1].setTextColor(white);
    textMenu[2].setText("EXIT");
    textMenu[2].setTextColor(white);


    bool chosen[numMenu] = {false, false};

    SDL_Event mouseEvent;
    while(true){
        texture::Draw(imgStart, player->Camera, BgDest);
        for (int i=0; i < numMenu; i++){
            textMenu[i].loadFromRenderedText(font, renderer);
            textMenu[i].renderText(renderer, menuRect[i].x, menuRect[i].y);

        }
        while(SDL_PollEvent(&mouseEvent)){
            switch(mouseEvent.type){
                case SDL_QUIT:
                    return 2;
                case SDL_MOUSEMOTION:
                    for (int i=0; i<numMenu; i++){
                        if(checkSelected(mouseEvent.motion.x, mouseEvent.motion.y, menuRect[i])){
                            if (chosen[i] == false){
                                chosen[i] = true;
                                textMenu[i].setTextColor(green);
                                Mix_PlayChannel(-1,Beep,0);

                            }
                        } else {
                            if (chosen[i] == true){
                                chosen[i] = false;
                                textMenu[i].setTextColor(white);

                            }
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    for (int i=0; i < numMenu; i++){
                        if(checkSelected(mouseEvent.motion.x, mouseEvent.motion.y, menuRect[i])){
                                return i;
                            }
                        }
                    break;
                case SDL_KEYDOWN:
                    if (mouseEvent.key.keysym.sym == SDLK_ESCAPE){
                        return 2;
                    }
                default:
                    break;
            };

        }
        SDL_RenderPresent(renderer);
    }

    return 0;
}
int Game::createGuide(TTF_Font* font)
{
    imgGuide = texture::LoadTexture("image/guide.png");
    texture::Draw(imgGuide,player->Camera,BgDest);
    SDL_Rect guideRect;

    guideRect.x = 460;
    guideRect.y = 610;
    guideRect.w = 96;
    guideRect.h = 32;

    textObj textGuide;
    textGuide.setText("OK");
    textGuide.setTextColor(black);

    bool chosen = false;

    SDL_Event mouseEvent;
    while(true){
        texture::Draw(imgGuide, player->Camera, BgDest);
        textGuide.loadFromRenderedText(font, renderer);
        textGuide.renderText(renderer,guideRect.x,guideRect.y);
        while(SDL_PollEvent(&mouseEvent)){
            switch(mouseEvent.type){
                case SDL_QUIT:
                    return 3;
                case SDL_MOUSEMOTION:
                    if(checkSelected(mouseEvent.motion.x, mouseEvent.motion.y, guideRect)){
                        if (chosen == false){
                            chosen = true;
                            textGuide.setTextColor(green);
                            Mix_PlayChannel(-1,Beep,0);
                        }
                    }
                    else {
                        if (chosen == true){
                            chosen = false;
                            textGuide.setTextColor(black);
                        }
                    }

                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(checkSelected(mouseEvent.motion.x, mouseEvent.motion.y, guideRect)){
                            return 1;
                    }
                    break;
                case SDL_KEYDOWN:
                    if (mouseEvent.key.keysym.sym == SDLK_ESCAPE){
                        return 3;
                    }
                default:
                    break;
            }
        }
        SDL_RenderPresent(renderer);
    }
    return 0;
}
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    state = MainMenu;
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
            fontMenu = TTF_OpenFont("font/font2.ttf",48);
        }

    }
    else
    {
        isRunning = false;
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Music = Mix_LoadMUS("sound/win.wav");
    Mix_VolumeMusic(MIX_MAX_VOLUME/2);
    BgMusic = Mix_LoadMUS("sound/bgMusic.wav");
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    Beep = Mix_LoadWAV("sound/beep.wav");
    Mix_VolumeChunk(Beep,MIX_MAX_VOLUME/2);
    if (Music==NULL||BgMusic==NULL||Beep==NULL) cout << SDL_GetError();
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
    threats.push_back(new Threat(460, 3150));
    threats.push_back(new Threat (470,2050));
    threats.push_back(new Threat (470,86));
    threats.push_back(new Threat (470,3700));

    isStart = true;

    while (isStart)
    {
        if (state==MainMenu)
        {
            int GameState = createMenu(fontMenu);
            if (GameState==0){
                isStart=false;
                isRunning=true;
                startTime=SDL_GetTicks()/1000;
            }
            if (GameState==2) {
                isRunning = false;
                isStart=false;
            }
            if (GameState==1) {
                state = Guide;
            }

        }
        if (state==Guide)
        {
            int tmp = createGuide(fontMenu);
            if (tmp==1) state = MainMenu;

        }
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
    for (auto& threat : threats) {
        threat->Update(mapper->tile, mapper->mapping, player->Camera);
    }
    for(Threat* threat :threats)
    {
        player->ThreatAttack(*threat);
    }

    if (player->isWin == true)
    {
        win = true;

        if (Mix_PlayingMusic() == 1)
        {
            Mix_HaltMusic();
            Mix_PlayMusic(Music, -1);
        }
        cout << timeVal<<endl;
        record();
    }
    if (player->isWin == false)
    {
        if (!Mix_PlayingMusic())
        {
            Mix_PlayMusic(BgMusic, -1);
        }
    }
}

//bool Game::Start()
//{
//    texture::Draw(imgStart,player->Camera,BgDest);
//    SDL_Event e;
//    while (true)
//    {
//        texture::Draw(imgStart,player->Camera,BgDest);
//        while (SDL_PollEvent(&e))
//        {
//            switch(e.key.keysym.sym)
//            {
//            case SDLK_y:
//                return true;
//            case SDLK_n:
//                return false;
//            }
//
//        }
//        SDL_RenderPresent(renderer);
//    }
//    return true;
//}
void Game::Ending()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, victory, NULL, NULL);
    SDL_RenderPresent(renderer);
}
void Game::record()
{
    score = timeVal;
    ifstream input("bestscore.txt");
    input >> hscore;
    ofstream output("bestscore.txt");
    if (score < hscore || hscore == 0) {
        output << score;
    }
    else output << hscore;
}

void Game::render()
{
    SDL_RenderClear(renderer);
    mapper->DrawMap(player->Camera);
    texture::Draw(background, player->Camera, BgDest);

    for (auto& threat : threats) {
        threat->Render(player->Camera);
    }
    player->Render();
    texture::Draw(foreground, player->Camera, BgDest);
    texture::Draw(babe, babeSrcRect, babeDestRect);



    //Time and high score text
    string strTime = "TIME: ";
    timeVal = SDL_GetTicks() / 1000 - startTime;
    string strHScore = "HIGH SCORE: ";

    ifstream input("bestscore.txt");
    input >> hscore;
    input.close();
    string timeRes = to_string(timeVal);
    string hscoreText = to_string(hscore);

    strTime += timeRes;
    strHScore += hscoreText;

    timeGame.setText(strTime);
    HighestScore.setText(strHScore);
    timeGame.setTextColor(yellow);
    HighestScore.setTextColor(red);
    timeGame.loadFromRenderedText(font, renderer);
    HighestScore.loadFromRenderedText(font, renderer);
    timeGame.renderText(renderer, 25, 15);
    HighestScore.renderText(renderer,SCREEN_WIDTH-200,15);
    SDL_RenderPresent(renderer);

}



void Game::clean()
{
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyTexture(imgStart);
    imgStart = NULL;
    SDL_DestroyTexture(background);
    background = NULL;
    SDL_DestroyTexture(foreground);
    foreground = NULL;
    SDL_DestroyTexture(babe);
    babe = NULL;
    SDL_DestroyTexture(victory);
    victory = NULL;

    for (auto&threat : threats)
    {
        threat->closeThreat();
    }
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
bool Game::checkSelected(const int& x, const int& y, const SDL_Rect& rect)
{
    if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h) return true;
    return false;
}
