#ifndef INC_2D_GAME_ENGINE_GAME_H
#define INC_2D_GAME_ENGINE_GAME_H

#include <memory>
#include "ECS/ECS.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "AssetStore/AssetStore.h"
#include "EventBus/EventBus.h"
#include "sol/state.hpp"

const int FPS = 30;
const int MILLISEC_PER_FRAME = 1000 / FPS;

class Game {
private:
    bool isRunning;
    bool isDebug;
    SDL_Window* window ;
    SDL_Renderer* renderer;
    int millisec_previous_frame = 0;
    SDL_Rect camera;

    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetStore> assetStore;
    std::unique_ptr<EventBus> eventBus;
    sol::state _lua;

public:
    Game();
    ~Game();
    void Initialize();
    void Run();
    void Setup();
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();

    static int windowWidth;
    static int windowHeight;
    static int mapWidth;
    static int mapHeight;




};


#endif //INC_2D_GAME_ENGINE_GAME_H
