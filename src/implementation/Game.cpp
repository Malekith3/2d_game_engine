#include "../headers/Game.h"
#include <iostream>

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

Game::Game()
{
    this->isRunning = false;
    std::cout << "Game Constructor got called" << std::endl;
}

Game::~Game()
{
    std::cout << "Game Destructor got called" << std::endl;
}

void Game::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "Error initializing SDL" << std::endl;
        return;
    }
    SDL_DisplayMode  displayMode;
    SDL_GetCurrentDisplayMode(0,&displayMode);
    this->windowWidth   = 800;
    this->windowHeight  = 600;
    this->window = SDL_CreateWindow(
            "Game Engine",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            this->windowWidth,
            this->windowHeight,
            SDL_WINDOW_BORDERLESS);
    if(!this->window)
    {
     std::cerr << "Error creating SDL window" << std::endl;
     return;
    }
    this->renderer = SDL_CreateRenderer(window,-1,0);
    if (!this->renderer)
    {
        std::cerr << "Error creating SDL renderer" << std::endl;
    }
    SDL_SetWindowFullscreen(this->window,SDL_WINDOW_FULLSCREEN);
    this->isRunning = true;
}

void Game::Run()
{
    this->Setup();
    while(this->isRunning)
    {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                this->isRunning = false;
                break;
            case SDL_KEYDOWN:
                if(sdlEvent.key.keysym.sym == SDLK_ESCAPE)
                    this->isRunning = false;
                break;
        }
    }
}

void Game::Update()
{
    // If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if(timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
        SDL_Delay(timeToWait);

    float deltaTime = (SDL_GetTicks() - this->millisecsPreviousFrame)/1000.0f;
    
    //Store the current frame time
    this->millisecsPreviousFrame = SDL_GetTicks();

    glm::vec2 displacment = playerVelocity * deltaTime;
    playerPosition += displacment;
}

void Game::Render()
{
    SDL_SetRenderDrawColor(this->renderer,21,21,21,255);
    SDL_RenderClear(this->renderer);

    //Load a PNG texture
    SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderer, surface);
    SDL_FreeSurface(surface);

    // Render texture
    SDL_Rect destRect = {
            static_cast<int>(playerPosition.x),
            static_cast<int>(playerPosition.y),
            32,
            32};
    SDL_RenderCopy(this->renderer,texture,nullptr,&destRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(this->renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
}

void Game::Setup() {
    playerPosition = {20.0,10.0};
    playerVelocity = {10.0,5.0};
}

