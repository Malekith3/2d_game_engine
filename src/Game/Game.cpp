#include "Game.h"
#include "rapidcsv.h"
#include "Logger/logger.h"
#include "Systems/DamageSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/AnimationSystem.h"
#include "Components/SpriteComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/TransformComponent.h"
#include "Components/AnimationComponent.h"
#include "Systems/RenderCollisionSystem.h"
#include "Components/BoxColliderComponent.h"
#include "Events/KeyPressedEvent.h"
#include "Systems/KeyboardControlSystem.h"
#include "Components/KeyboardControlledComponent.h"
#include "Components/CameraFollowComponent.h"
#include "Systems/CameraMovementSystem.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Systems/ProjectileEmitSystem.h"
#include "Components/HealthComponent.h"
#include "Systems/ProjectileLifecycleSystem.h"
#include "Components/TextLabelComponent.h"
#include "Systems/RenderTextSystem.h"
#include "Systems/RenderHealthBarSystem.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include "imgui/imgui_impl_sdl2.h"
#include "Systems/RenderGUISystem.h"
#include "Common/TagGroups.h"
#include "LevelLoader.h"
#include "Systems/ScriptSystem.h"

int Game::windowHeight;
int Game::windowWidth;
int Game::mapHeight;
int Game::mapWidth;


Game::Game()
{
    this->isRunning = false;
    this->isDebug = true;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    eventBus = std::make_unique<EventBus>();
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
    if(TTF_Init() != 0)
    {
      LOGGER_ERROR("ERROR to load TTF");
      return;
    }

    SDL_DisplayMode  displayMode;
    SDL_GetCurrentDisplayMode(0,&displayMode);
    this->windowWidth   = 1280;
    this->windowHeight  = 700;
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
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(window,renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // TODO Init camera view with the entire screen area
    camera.x = 0;
    camera.y = 0;
    camera.w = windowWidth;
    camera.h = windowHeight;

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
        //ImGui SDL input
      ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
      ImGuiIO io  = ImGui::GetIO();
      int mouseX, mouseY;
      const int buttons = SDL_GetMouseState(&mouseX,&mouseY);
      io.MousePos = ImVec2(mouseX,mouseY);
      io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
      io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

        switch (sdlEvent.type)
        {
            case SDL_QUIT:
                this->isRunning = false;
                break;
            case SDL_KEYDOWN:
                if(sdlEvent.key.keysym.sym == SDLK_ESCAPE)
                {
                  this->isRunning = false;
                }
                else if(sdlEvent.key.keysym.sym == SDLK_d || sdlEvent.key.keysym.sym == SDLK_KP_D)
                {
                  isDebug = !isDebug;
                }
                eventBus->EmitEvent<KeyPressedEvent>(SDL_KeyCode(sdlEvent.key.keysym.sym));
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
    eventBus->Reset();
    //Perform Subscription of all systems
    registry->GetSystem<DamageSystem>().SubscribeToEvent(eventBus);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvent(eventBus);
    registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<MovementSystem>().SubscribeToEvent(eventBus);

    //Update Systems
    registry->Update();
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<ProjectileEmitSystem>().Update(registry);
    registry->GetSystem<ProjectileLifecycleSystem>().Update();
    registry->GetSystem<ScriptSystem>().Update(deltaTime, SDL_GetTicks());
}

void Game::Render()
{
  SDL_SetRenderDrawColor(this->renderer,21,21,21,255);
  SDL_RenderClear(this->renderer);
  registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
  registry->GetSystem<RenderTextSystem>().Update(assetStore, renderer, camera);
  registry->GetSystem<RenderHealthBarSystem>().Update(assetStore, renderer, camera);
  registry->GetSystem<RenderHealthBarSystem>().Update(assetStore, renderer, camera);

  if(isDebug)
  {
    registry->GetSystem<RenderCollisionSystem>().Update(renderer, camera);
    registry->GetSystem<RenderGUISystem>().Update(registry, camera);
  }

  SDL_RenderPresent(this->renderer);

}

void Game::Destroy()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

void Game::Setup() {
  // Add the systems that need to be processed in our game
  registry->AddSystem<RenderSystem>();
  registry->AddSystem<DamageSystem>();
  registry->AddSystem<KeyboardControlSystem>();
  registry->AddSystem<MovementSystem>();
  registry->AddSystem<AnimationSystem>();
  registry->AddSystem<CollisionSystem>();
  registry->AddSystem<RenderCollisionSystem>();
  registry->AddSystem<CameraMovementSystem>();
  registry->AddSystem<ProjectileEmitSystem>();
  registry->AddSystem<ProjectileLifecycleSystem>();
  registry->AddSystem<RenderTextSystem>();
  registry->AddSystem<RenderHealthBarSystem>();
  registry->AddSystem<RenderGUISystem>();
  registry->AddSystem<ScriptSystem>();

  registry->GetSystem<ScriptSystem>().CreateLuaBindings(_lua);

  LevelLoader loader;
  _lua.open_libraries(sol::lib::base, sol::lib::math);
  loader.LoadLevel(_lua, registry, assetStore, renderer, 1);
}

