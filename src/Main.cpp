#include "headers/Game.h"


int main() {
    Game* game = new Game();

    game->Initialize();
    game->Run();
    game->Destroy();
}
