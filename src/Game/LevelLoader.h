//
// Created by sanek on 14/09/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_GAME_LEVELLOADER_H_
#define INC_2D_GAME_ENGINE_SRC_GAME_LEVELLOADER_H_

#include "ECS/ECS.h"
#include "AssetStore/AssetStore.h"
#include "sol/state.hpp"

class LevelLoader
{
 public:
  LevelLoader();
  virtual ~LevelLoader();

  void LoadLevel(sol::state& lua,
                 const std::unique_ptr<Registry>& registry,
                 const std::unique_ptr<AssetStore>& assetStore,
                 SDL_Renderer *renderer,
                 int level);
};

#endif //INC_2D_GAME_ENGINE_SRC_GAME_LOADLEVEL_H_
