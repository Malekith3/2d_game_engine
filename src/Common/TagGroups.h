//
// Created by sanek on 14/09/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_COMMON_TAGGROUPS_H_
#define INC_2D_GAME_ENGINE_SRC_COMMON_TAGGROUPS_H_

#include <string_view>
#include <array>
enum class TAG_GROUPS
{
  PLAYER,
  ENEMIES,
  OBSTACLES,
  PROJECTILES,
  COUNT
};

inline std::string getTagNameString(TAG_GROUPS tagGroup)
{
  static const std::array<std::string, static_cast<size_t>(TAG_GROUPS::COUNT)> tagNames = {
      "player",
      "enemies",
      "obstacles",
      "projectiles"
  };

  return tagNames[static_cast<size_t>(tagGroup)];
}


#endif //INC_2D_GAME_ENGINE_SRC_COMMON_TAGGROUPS_H_
