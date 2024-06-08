//
// Created by sanek on 30/09/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_ASSETSTORE_ASSETSTORE_H_
#define INC_2D_GAME_ENGINE_SRC_ASSETSTORE_ASSETSTORE_H_

#include <map>
#include <SDL.h>
#include <string>
#include <filesystem>
#include <SDL_ttf.h>

class AssetStore {
 private:
  std::map<std::string, SDL_Texture*> _textures;
  std::map<std::string, TTF_Font*> _fonts;

 public:
    AssetStore();
    ~AssetStore();

    void ClearAssets();
    void AddTexture(const std::string &assetId, const std::filesystem::path &filePath, SDL_Renderer *renderer);
    SDL_Texture* GetTexture(const std::string &assetId) const;
    void AddFont(std::string_view fontID, const std::filesystem::path &filePath, int fontSize);
    TTF_Font* GetFont(std::string_view fontID);
};

#endif //INC_2D_GAME_ENGINE_SRC_ASSETSTORE_ASSETSTORE_H_
