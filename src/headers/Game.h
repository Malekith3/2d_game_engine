#ifndef INC_2D_GAME_ENGINE_GAME_H
#define INC_2D_GAME_ENGINE_GAME_H


class Game {
private:
    //...
public:
    Game();
    ~Game();
    void Initialize();
    void Run();
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();



};


#endif //INC_2D_GAME_ENGINE_GAME_H
