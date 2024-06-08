//
// Created by sanek on 30/09/2023.
//

#include "AssetStore.h"
#include <SDL_image.h>
#include "Logger/logger.h"

void AssetStore::ClearAssets() {
  for(auto& texture: _textures){
    SDL_DestroyTexture(texture.second);
  }

  for(auto& font: _fonts){
    TTF_CloseFont(font.second);
  }

  _textures.clear();
  _fonts.clear();
}

void AssetStore::AddTexture(const std::string &assetId, const std::filesystem::path &filePath, SDL_Renderer *renderer) {
  auto* surface = IMG_Load(filePath.string().c_str());
  auto* texture = SDL_CreateTextureFromSurface(renderer,surface);
  _textures.emplace(std::pair<std::string, SDL_Texture*>(assetId, texture));
  SDL_FreeSurface(surface);

  LOGGER_TRACE("[AssetStore::AddTexture] new texture was added to asset store with id = {}",assetId);
}

AssetStore::AssetStore() {
  LOGGER_TRACE("[AssetStore::AssetStore] ctor was called");
}

SDL_Texture *AssetStore::GetTexture(const std::string &assetId) const {
  auto it = _textures.find(assetId);
  if(it!= _textures.end()){
    return it->second;
  }
  return nullptr;
}

AssetStore::~AssetStore() {
  ClearAssets();
  LOGGER_TRACE("[AssetStore::AssetStore] dtor was called");
}

void AssetStore::AddFont(std::string_view fontID, const std::filesystem::path &filePath, int fontSize)
{
  _fonts.emplace(fontID, TTF_OpenFont(filePath.string().c_str(),fontSize));
}

TTF_Font *AssetStore::GetFont(std::string_view fontID)
{
  return _fonts[fontID.data()];
}
