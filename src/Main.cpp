#include "./Game/Game.h"
#include "Logger/logger.h"


int main(int argc,char* argv[]) {
  Logger::Initialize();
  Game* game = new Game();
  game->Initialize();
  game->Run();
  game->Destroy();
  delete game;
}
