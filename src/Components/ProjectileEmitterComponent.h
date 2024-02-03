//
// Created by sanek on 03/02/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_COMPONENTS_PROJECTILEEMITTERCOMPONENT_H_
#define INC_2D_GAME_ENGINE_SRC_COMPONENTS_PROJECTILEEMITTERCOMPONENT_H_
#include <glm/glm.hpp>
struct ProjectileEmitterComponent{
  glm::vec2 projectileVelocity{};
  int repeatedFrequency;
  int projectileDuration;
  int hitPercentDamage;
  bool isFriendly;
  int lastEmissionTime{};

  explicit ProjectileEmitterComponent(const glm::vec2 &projectile_velocity = glm::vec2(0,0),
                             int repeated_frequency = 0,
                             int projectile_duration = 10000,
                             int hit_percent_damage = 10,
                             bool isFriendly = false)
      : projectileVelocity(projectile_velocity),
        repeatedFrequency(repeated_frequency),
        projectileDuration(projectile_duration),
        hitPercentDamage(hit_percent_damage),
        isFriendly(isFriendly){}
};
#endif //INC_2D_GAME_ENGINE_SRC_COMPONENTS_PROJECTILEEMITTERCOMPONENT_H_
