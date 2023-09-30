//
// Created by sanek on 30/09/2023.
//

#include "AssetStore.h"
#include <SDL_image.h>
#include "Logger/logger.h"

void AssetStore::ClearAssets() {
  for(auto& texture: m_textures){
    SDL_DestroyTexture(texture.second);
  }
  m_textures.clear();
}

void AssetStore::AddTexture(const std::string &assetId, const std::filesystem::path &filePath, SDL_Renderer *renderer) {
  auto* surface = IMG_Load(filePath.string().c_str());
  auto* texture = SDL_CreateTextureFromSurface(renderer,surface);
  m_textures.emplace(std::pair<std::string,SDL_Texture*>(assetId,texture));
  SDL_FreeSurface(surface);

  LOGGER_TRACE("[AssetStore::AddTexture] new texture was added to asset store with id = {}",assetId);
}

AssetStore::AssetStore() {
  LOGGER_TRACE("[AssetStore::AssetStore] ctor was called");
}

SDL_Texture *AssetStore::GetTexture(const std::string &assetId) const {
  auto it = m_textures.find(assetId);
  if(it!= m_textures.end()){
    return it->second;
  }
  return nullptr;
}

AssetStore::~AssetStore() {
  ClearAssets();
  LOGGER_TRACE("[AssetStore::AssetStore] dtor was called");
}
