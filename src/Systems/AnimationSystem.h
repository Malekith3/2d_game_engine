//
// Created by sanek on 19/11/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_ANIMATIONSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_ANIMATIONSYSTEM_H_

#include "ECS/ECS.h"
class AnimationSystem : public System{
 public:
    AnimationSystem();
    void Update();
};

#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_ANIMATIONSYSTEM_H_
