#include "Game.h"
#include <string>
#include "Logger/logger.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Systems/MovementSystem.h"

Game::Game()
{
    this->isRunning = false;
    registry = std::make_unique<Registry>();
    LOGGER_TRACE("Game Constructor got called");
}

Game::~Game()
{
    LOGGER_ERROR("Game Destructor got called");
}

void Game::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        LOGGER_ERROR("Error initializing SDL");
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
            SDL_WINDOW_RESIZABLE);
    if(!this->window)
    {
      LOGGER_ERROR("Error creating SDL window");
      return;
    }
    this->renderer = SDL_CreateRenderer(window,-1,0);
    if (!this->renderer)
    {
      LOGGER_ERROR("Error creating SDL renderer");
    }
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
    // If we are too fast, waste some time until we reach the MILLISEC_PER_FRAME
    int timeToWait = MILLISEC_PER_FRAME - (SDL_GetTicks() - millisec_previous_frame);
    if(timeToWait > 0 && timeToWait <= MILLISEC_PER_FRAME)
        SDL_Delay(timeToWait);

    double deltaTime = (SDL_GetTicks() - millisec_previous_frame)/1000.0f;
    
    //Store the current frame time
    millisec_previous_frame = SDL_GetTicks();

    registry->Update();
  registry->GetSystem<MovementSystem>().Update(deltaTime);

}

void Game::Render()
{
    SDL_SetRenderDrawColor(this->renderer,21,21,21,255);
    SDL_RenderClear(this->renderer);

    //Load a PNG texture
    SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderer, surface);
    SDL_FreeSurface(surface);
    
    // TODO: Render game objects... 

    SDL_RenderPresent(this->renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
}

void Game::Setup() {
  // Add the systems that need to be processed in our game
  registry->AddSystem<MovementSystem>();

  // Create an entity
  Entity tank = registry->CreateEntity();

  // Add some components to that entity
  tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
  tank.AddComponent<RigidBodyComponent>(glm::vec2(10.0, 50.0));
}

