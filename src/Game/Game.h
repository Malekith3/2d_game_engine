#ifndef INC_2D_GAME_ENGINE_GAME_H
#define INC_2D_GAME_ENGINE_GAME_H

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "ECS/ECS.h"

const int FPS = 30;
const int MILLISEC_PER_FRAME = 1000 / FPS;

class Game {
private:
    bool isRunning;
    SDL_Window* window ;
    SDL_Renderer* renderer;
    int millisec_previous_frame = 0;
    std::unique_ptr<Registry> registry;

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

    int windowWidth;
    int windowHeight;


};


#endif //INC_2D_GAME_ENGINE_GAME_H
