//
// Created by sanek on 08/06/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERHEALTHBARSYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERHEALTHBARSYSTEM_H_

#include "ECS/ECS.h"
#include "Components/HealthComponent.h"
#include "Components/TextLabelComponent.h"
#include "AssetStore/AssetStore.h"
#include "Components/TransformComponent.h"
#include "Components/SpriteComponent.h"
#include "glm/glm.hpp"

class RenderHealthBarSystem : public System
{
 public:
  RenderHealthBarSystem()
  {
    RequreComponent<HealthComponent>();
    RequreComponent<TransformComponent>();
    RequreComponent<SpriteComponent>();
  }

  void Update(std::unique_ptr<AssetStore> &assetStore, SDL_Renderer *renderer, const SDL_Rect &camera)
  {
    for(auto entity : GetSystemEntities())
    {
      const auto health = entity.GetComponent<HealthComponent>();
      const auto sprite = entity.GetComponent<SpriteComponent>();
      const auto transform = entity.GetComponent<TransformComponent>();

      SDL_Color healthBar = GetHealthColor(health.health, 100);

      int healthBarWidth = 15;
      int healthBarHeight = 3;

      double  healthBarPosX = (transform.m_position.x + (sprite.m_width * transform.m_scale.x)) - camera.x;
      double  healthBarPosY = (transform.m_position.y) - camera.y;

      SDL_Rect healthBarRectangle =
      {
       static_cast<int>(healthBarPosX),
       static_cast<int>(healthBarPosY),
       static_cast<int>(healthBarWidth * (health.health / 100.0)),
       static_cast<int>(healthBarHeight)
      };

      SDL_SetRenderDrawColor(renderer, healthBar.r, healthBar.g, healthBar.b, 255);
      SDL_RenderFillRect(renderer, &healthBarRectangle);


      auto textLabel = std::to_string(health.health) + "%";
      SDL_Surface* surface = TTF_RenderText_Blended(assetStore->GetFont("health-font"),
                                                    textLabel.c_str(), healthBar);

      SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
      SDL_FreeSurface(surface);

      int labelWidth = 0;
      int labelHeight = 0;
      SDL_QueryTexture(texture, nullptr,nullptr, &labelWidth, &labelHeight);

      SDL_Rect dstRect = {static_cast<int>(healthBarPosX),
                          static_cast<int>(healthBarPosY) + 5,
                          labelWidth,
                          labelHeight};

      SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

      SDL_DestroyTexture(texture);
    }
    }

  static SDL_Color GetHealthColor(int currentHealth, int maxHealth)
  {
    auto invLerp = [](float value, float min, float max)
    {
      return(value-min) / ( max - min);
    };
    auto lerp = [](const glm::vec3& a, const glm::vec3& b, float t)
    {
      return glm::mix(a, b, t);
    };

    // Define the colors for full health (green), middle health (yellow), and no health (red)
    glm::vec3 green(0.0f, 1.0f, 0.0f);  // RGB for green
    glm::vec3 yellow(1.0f, 1.0f, 0.0f); // RGB for yellow
    glm::vec3 red(1.0f, 0.0f, 0.0f);    // RGB for red

    // Calculate the health percentage
    float healthPercentage = invLerp(currentHealth, 0.0f, maxHealth);

    // Interpolate between green and yellow for the first half of the health range
    glm::vec3 interpolatedColor;
    if (healthPercentage > 0.5f) {
      float t = invLerp(healthPercentage, 0.5f, 1.0f);
      interpolatedColor = lerp(yellow, green, t);
    }
      // Interpolate between yellow and red for the second half of the health range
    else {
      float t = invLerp(healthPercentage, 0.0f, 0.5f);
      interpolatedColor = lerp(red, yellow, t);
    }

    SDL_Color sdlColor;
    sdlColor.r = static_cast<Uint8>(interpolatedColor.r * 255);
    sdlColor.g = static_cast<Uint8>(interpolatedColor.g * 255);
    sdlColor.b = static_cast<Uint8>(interpolatedColor.b * 255);

    return sdlColor;
  }
};

#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERHEALTHBARSYSTEM_H_
