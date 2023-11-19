#include "Game.h"
#include "rapidcsv.h"
#include "Logger/logger.h"
#include "Systems/RenderSystem.h"
#include "Systems/MovementSystem.h"
#include "Components/SpriteComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/TransformComponent.h"
#include "Components/AnimationComponent.h"
#include "Systems/AnimationSystem.h"

Game::Game()
{
    this->isRunning = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
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
  registry->GetSystem<AnimationSystem>().Update();

}

void Game::Render()
{
    SDL_SetRenderDrawColor(this->renderer,21,21,21,255);
    SDL_RenderClear(this->renderer);
    registry->GetSystem<RenderSystem>().Update(renderer,assetStore);
    SDL_RenderPresent(this->renderer);
}

void Game::Destroy()
{
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
}

void Game::LoadLevel(uint32_t level_number){
  // Add the systems that need to be processed in our game
  registry->AddSystem<MovementSystem>();
  registry->AddSystem<RenderSystem>();
  registry->AddSystem<AnimationSystem>();

  //Adding Assets
  assetStore->AddTexture("tank-image","../assets/images/tank-panther-right.png",renderer);
  assetStore->AddTexture("truck-image","../assets/images/truck-ford-right.png",renderer);
  assetStore->AddTexture("chopper-image","../assets/images/chopper.png",renderer);
  assetStore->AddTexture("tilemap-image","../assets/tilemaps/jungle.png",renderer);

  //Load a Map
  rapidcsv::Document doc("../assets/tilemaps/jungle.map", rapidcsv::LabelParams(-1, -1));

  //constants
  int tileSize = 32;
  double tileScale = 2.0;

  for(int y = 0; y < doc.GetRowCount(); y++){
    auto tileRow = doc.GetRow<std::string>(y);
    for(int x = 0; x <tileRow.size();x++){
      int srcRectY = (tileRow[x][0] - '0') * tileSize;
      int srcRectX = (tileRow[x][1] - '0') * tileSize;
      auto newTile = registry->CreateEntity();
      newTile.AddComponent<TransformComponent>(   glm::vec2(x * (tileScale * tileSize),
                                                         y * (tileScale * tileSize)),
                                                    glm::vec2(tileScale, tileScale), 0.0);
      newTile.AddComponent<SpriteComponent>(tileSize, tileSize,"tilemap-image",0, srcRectX, srcRectY);
    }
  }


  // Create an entity
  Entity chopper = registry->CreateEntity();

  // Add some components to that entity
  chopper.AddComponent<TransformComponent>(glm::vec2(80.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
  chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
  chopper.AddComponent<SpriteComponent>(32,32,"chopper-image",1);
  chopper.AddComponent<AnimationComponent>(2,12);

  // Create an entity
  Entity tank = registry->CreateEntity();
  // Add some components to that entity
  tank.AddComponent<TransformComponent>(glm::vec2(10.0, 70.0), glm::vec2(1.0, 1.0), 0.0);
  tank.AddComponent<RigidBodyComponent>(glm::vec2(30.0, 0.0));
  tank.AddComponent<SpriteComponent>(32,32,"tank-image",1);

  Entity track = registry->CreateEntity();
  // Add some components to that entity
  track.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
  track.AddComponent<RigidBodyComponent>(glm::vec2(30.0, 0.0));
  track.AddComponent<SpriteComponent>(32,32, "truck-image",1);
}

void Game::Setup() {
  LoadLevel(1);
}

