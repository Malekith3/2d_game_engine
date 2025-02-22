//
// Created by sanek on 14/09/2024.
//

#include "LevelLoader.h"
#include "rapidcsv.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "Common/TagGroups.h"
#include "sol/sol.hpp"
#include "Game.h"
#include "Components/RigidBodyComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/KeyboardControlledComponent.h"
#include "Components/CameraFollowComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/ScriptComponent.h"

LevelLoader::LevelLoader()
{

}

LevelLoader::~LevelLoader()
{

}

void LevelLoader::LoadLevel(sol::state& lua,
                            const std::unique_ptr<Registry>& registry,
                            const std::unique_ptr<AssetStore>& assetStore,
                            SDL_Renderer *renderer,
                            int level)
{

  auto script = lua.load_file(fmt::format("assets/scripts/Level{}.lua", level));

  if(!script.valid())
  {
    sol::error err = script;
    LOGGER_ERROR("Cant load script ! reason {}", err.what());
    return;
  }

  lua.script_file(fmt::format("assets/scripts/Level{}.lua", level));
  sol::table levelData =  lua["Level"];
  sol::table assets = levelData["assets"];

  int i = 0;
  while (true) {
    sol::optional<sol::table> hasAsset = assets[i];
    if (hasAsset == sol::nullopt) {
      break;
    }
    sol::table asset = assets[i];
    std::string assetType = asset["type"];
    std::string assetId = asset["id"];
    std::string filePath = asset["file"];
    if (assetType == "texture") {
      assetStore->AddTexture( assetId, filePath, renderer);
      LOGGER_DEBUG("A new texture asset was added to the asset store, id: ", assetId);
    }
    if (assetType == "font") {
      int fontSize = asset["font_size"];
      assetStore->AddFont(assetId, filePath, fontSize);
      LOGGER_DEBUG("A new font asset was added to the asset store, id: ", assetId);
    }
    i++;
  }



  sol::table map = levelData["tilemap"];
  int tileSize = map["tile_size"];
  double tileScale = map["scale"];
  std::string mapTextureAssetID = map["texture_asset_id"];
  std::string mapFilePath = map["map_file"];

  //Load a Map
  rapidcsv::Document doc(mapFilePath, rapidcsv::LabelParams(-1, -1));

  for(int y = 0; y < doc.GetRowCount(); y++){
    auto tileRow = doc.GetRow<std::string>(y);
    for(int x = 0; x < tileRow.size(); ++x){
      int srcRectY = (tileRow[x][0] - '0') * tileSize;
      int srcRectX = (tileRow[x][1] - '0') * tileSize;
      auto newTile = registry->CreateEntity();
      newTile.Group("tiles");
      newTile.AddComponent<TransformComponent>(   glm::vec2(x * (tileScale * tileSize),
                                                            y * (tileScale * tileSize)),
                                                  glm::vec2(tileScale, tileScale), 0.0);
      newTile.AddComponent<SpriteComponent>(tileSize, tileSize,mapTextureAssetID,0, srcRectX, srcRectY);
    }
  }

  Game::mapHeight = doc.GetRowCount() * tileSize * tileScale;
  Game::mapWidth = doc.GetColumnCount() * tileSize * tileScale;


  ////////////////////////////////////////////////////////////////////////////
// Read the level entities and their components
////////////////////////////////////////////////////////////////////////////
  sol::table entities = levelData["entities"];
  i = 0;
  while (true) {
    sol::optional<sol::table> hasEntity = entities[i];
    if (hasEntity == sol::nullopt) {
      break;
    }

    sol::table entity = entities[i];

    Entity newEntity = registry->CreateEntity();

    // Tag
    sol::optional<std::string> tag = entity["tag"];
    if (tag != sol::nullopt) {
      newEntity.Tag(*tag);
    }

    // Group
    sol::optional<std::string> group = entity["group"];
    if (group != sol::nullopt) {
      newEntity.Group(*group);
    }

    // Components
    sol::optional<sol::table> hasComponents = entity["components"];
    if (hasComponents != sol::nullopt) {
      // Transform
      sol::optional<sol::table> transform = entity["components"]["transform"];
      if (transform != sol::nullopt) {
        newEntity.AddComponent<TransformComponent>(
            glm::vec2(
                entity["components"]["transform"]["position"]["x"],
                entity["components"]["transform"]["position"]["y"]
            ),
            glm::vec2(
                entity["components"]["transform"]["scale"]["x"].get_or(1.0),
                entity["components"]["transform"]["scale"]["y"].get_or(1.0)
            ),
            entity["components"]["transform"]["rotation"].get_or(0.0)
        );
      }

      // RigidBody
      sol::optional<sol::table> rigidbody = entity["components"]["rigidbody"];
      if (rigidbody != sol::nullopt) {
        newEntity.AddComponent<RigidBodyComponent>(
            glm::vec2(
                entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
                entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)
            )
        );
      }

      // Sprite
      sol::optional<sol::table> sprite = entity["components"]["sprite"];
      if (sprite != sol::nullopt) {
        newEntity.AddComponent<SpriteComponent>(
            entity["components"]["sprite"]["width"],
            entity["components"]["sprite"]["height"],
            entity["components"]["sprite"]["texture_asset_id"],
            entity["components"]["sprite"]["z_index"].get_or(1),
            entity["components"]["sprite"]["src_rect_x"].get_or(0),
            entity["components"]["sprite"]["src_rect_y"].get_or(0),
            entity["components"]["sprite"]["fixed"].get_or(false)
        );
      }

      // Animation
      sol::optional<sol::table> animation = entity["components"]["animation"];
      if (animation != sol::nullopt) {
        newEntity.AddComponent<AnimationComponent>(
            entity["components"]["animation"]["num_frames"].get_or(1),
            entity["components"]["animation"]["speed_rate"].get_or(1)
        );
      }

      // BoxCollider
      sol::optional<sol::table> collider = entity["components"]["boxcollider"];
      if (collider != sol::nullopt) {
        newEntity.AddComponent<BoxColliderComponent>(
            entity["components"]["boxcollider"]["height"],
            entity["components"]["boxcollider"]["width"],
            glm::vec2(
                entity["components"]["boxcollider"]["offset"]["x"].get_or(0),
                entity["components"]["boxcollider"]["offset"]["y"].get_or(0)
            )
        );
      }

      // Health
      sol::optional<sol::table> health = entity["components"]["health"];
      if (health != sol::nullopt) {
        newEntity.AddComponent<HealthComponent>(
            static_cast<int>(entity["components"]["health"]["health_percentage"].get_or(100))
        );
      }

      // ProjectileEmitter
      sol::optional<sol::table> projectileEmitter = entity["components"]["projectile_emitter"];
      if (projectileEmitter != sol::nullopt) {
        newEntity.AddComponent<ProjectileEmitterComponent>(
            glm::vec2(
                entity["components"]["projectile_emitter"]["projectile_velocity"]["x"],
                entity["components"]["projectile_emitter"]["projectile_velocity"]["y"]
            ),
            static_cast<int>(entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(1)) * 1000,
            static_cast<int>(entity["components"]["projectile_emitter"]["projectile_duration"].get_or(10)) * 1000,
            static_cast<int>(entity["components"]["projectile_emitter"]["hit_percentage_damage"].get_or(10)),
            entity["components"]["projectile_emitter"]["friendly"].get_or(false)
        );
      }

      // CameraFollow
      sol::optional<sol::table> cameraFollow = entity["components"]["camera_follow"];
      if (cameraFollow != sol::nullopt) {
        newEntity.AddComponent<CameraFollowComponent>();
      }

      // KeyboardControlled
      sol::optional<sol::table> keyboardControlled = entity["components"]["keyboard_controller"];
      if (keyboardControlled != sol::nullopt) {
        newEntity.AddComponent<KeyboardControlledComponent>(
            glm::vec2(
                entity["components"]["keyboard_controller"]["up_velocity"]["x"],
                entity["components"]["keyboard_controller"]["up_velocity"]["y"]
            ),
            glm::vec2(
                entity["components"]["keyboard_controller"]["right_velocity"]["x"],
                entity["components"]["keyboard_controller"]["right_velocity"]["y"]
            ),
            glm::vec2(
                entity["components"]["keyboard_controller"]["left_velocity"]["x"],
                entity["components"]["keyboard_controller"]["left_velocity"]["y"]
            ),
            glm::vec2(
            entity["components"]["keyboard_controller"]["down_velocity"]["x"],
            entity["components"]["keyboard_controller"]["down_velocity"]["y"]
            )
        );
      }

      sol::optional<sol::table> scriptComponent = entity["components"]["on_update_script"];
      if(scriptComponent != sol::nullopt)
      {
        sol::function scriptFunction = entity["components"]["on_update_script"][0];
        newEntity.AddComponent<ScriptComponent>(scriptFunction);
      }
    }
    i++;
  }
}
