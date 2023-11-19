//
// Created by sanek on 19/11/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_COMPONENTS_ANIMATIONCOMPONENT_H_
#define INC_2D_GAME_ENGINE_SRC_COMPONENTS_ANIMATIONCOMPONENT_H_
#include "SDL_timer.h"
struct AnimationComponent {
    int numFrames;
    int currentFrame;
    int frameSpeedRate;
    bool isLoop;
    int startTime;

    AnimationComponent(int numFrames = 1, int frameSpeedRate = 1, bool isLoop = true){
      this->numFrames = numFrames;
      this->currentFrame = 1;
      this->frameSpeedRate = frameSpeedRate;
      this->isLoop = isLoop;
      this->startTime = SDL_GetTicks();

    }
};
#endif //INC_2D_GAME_ENGINE_SRC_COMPONENTS_ANIMATIONCOMPONENT_H_
