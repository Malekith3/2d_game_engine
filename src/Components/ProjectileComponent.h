//
// Created by sanek on 31/05/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_COMPONENTS_PROJECTILECOMPONENT_H_
#define INC_2D_GAME_ENGINE_SRC_COMPONENTS_PROJECTILECOMPONENT_H_

#include "SDL.h"

struct ProjectileComponent {
    bool isFriendly;
    int hitPercentileDamage;
    int duration;
    int startTime;

    explicit ProjectileComponent(bool isFriendly = false, int hitPercentileDamage = 0, int duration = 0) :
        isFriendly(isFriendly),
        hitPercentileDamage(hitPercentileDamage),
        duration(duration),
        startTime(SDL_GetTicks()) {}
};

#endif //INC_2D_GAME_ENGINE_SRC_COMPONENTS_PROJECTILECOMPONENT_H_