#include "./Game/Game.h"
#include "Logger/logger.h"
#include <SDL.h>

int main(int argc, char* argv[]) {
  Logger::Initialize();
  auto game = std::make_unique<Game>();
  game->Initialize();
  game->Run();
  game->Destroy();
  return 0;
}
