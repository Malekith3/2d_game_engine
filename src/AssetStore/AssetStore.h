//
// Created by sanek on 30/09/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_ASSETSTORE_ASSETSTORE_H_
#define INC_2D_GAME_ENGINE_SRC_ASSETSTORE_ASSETSTORE_H_

#include <map>
#include <SDL.h>
#include <string>
#include <filesystem>

class AssetStore {
 private:
  std::map<std::string, SDL_Texture*> m_textures;

 public:
    AssetStore();
    ~AssetStore();

    void ClearAssets();
    void AddTexture(const std::string &assetId, const std::filesystem::path &filePath, SDL_Renderer *renderer);
    SDL_Texture* GetTexture(const std::string &assetId) const;
};

#endif //INC_2D_GAME_ENGINE_SRC_ASSETSTORE_ASSETSTORE_H_
