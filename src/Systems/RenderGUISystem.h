//
// Created by sanek on 31/08/2024.
//

#ifndef INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERGUISYSTEM_H_
#define INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERGUISYSTEM_H_

#include "ECS/ECS.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include "imgui/imgui_impl_sdl2.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/HealthComponent.h"

class RenderGUISystem : public System
{
public:
  RenderGUISystem() = default;

  void Update(std::unique_ptr<Registry>& registry, const SDL_Rect& camera)
  {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    if(ImGui::Begin("Spawn enemies "))
    {
      static int enemyPos[2] = {0,0};
      static float enemyScale[2] = {1.0,1.0};
      static float enemyRotation;

      if(ImGui::CollapsingHeader("enemy transform config"))
      {
        ImGui::InputInt2("Position X,Y", enemyPos);
        ImGui::InputFloat2("Scale X,Y", enemyScale);
        ImGui::SliderAngle("Rotation (Deg)", &enemyRotation, 0 ,360);
      }
      ImGui::Spacing();

      static float projAngle;
      static int projVelocity;
      static int repFreq{10}, projDur{10}, damage{10};
      if(ImGui::CollapsingHeader("projectile config"))
      {
        ImGui::SliderInt("Projectile Velocity", &projVelocity,0,100);
        ImGui::SliderAngle("projectile angle", &projAngle, 0 ,360);
        ImGui::InputInt("Projectile repetition freq", &repFreq);
        ImGui::InputInt("Projectile duration", &projDur);
        ImGui::InputInt("Projectile damage", &damage);
      }
      ImGui::Spacing();

      static float enemyVelocity[2] = {0.0,0.0};
      if(ImGui::CollapsingHeader("rigid body config"))
      {
        ImGui::InputFloat2("velocity X,Y", enemyVelocity);
      }ImGui::Spacing();


      static int colliderSize[2] = {32,32};
      if(ImGui::CollapsingHeader("collider component"))
      {
        ImGui::InputInt2("collider size height, width", colliderSize);
      }
      ImGui::Spacing();

      const char* sprites[] = {"tank-image", "truck-image"};
      static int selectedSpriteIndex = 0;
      static int spriteSize[2] = {32,32};
      if(ImGui::CollapsingHeader("sprite settings"))
      {
        ImGui::Combo("texture is", &selectedSpriteIndex, sprites, IM_ARRAYSIZE(sprites));
        ImGui::InputInt2("Sprite size width, height", spriteSize);
      }
      ImGui::Spacing();

      static int enemyHealth{100};
      if(ImGui::CollapsingHeader("enemy health"))
      {
        ImGui::SliderInt("%", &enemyHealth, 0,100);
      }
      ImGui::Spacing();


      if(ImGui::Button("Spawn Enemy"))
      {
        auto newEnemy = registry->CreateEntity();
        newEnemy.Group("enemies");
        // Add some components to that entity
        newEnemy.AddComponent<TransformComponent>(glm::vec2(enemyPos[0],enemyPos[1])
            ,glm::vec2(enemyScale[0], enemyScale[1]), glm::degrees(enemyRotation));
        newEnemy.AddComponent<RigidBodyComponent>(glm::vec2(enemyVelocity[0], enemyVelocity[1]));
        newEnemy.AddComponent<SpriteComponent>(spriteSize[0],spriteSize[1],sprites[selectedSpriteIndex],1);
        newEnemy.AddComponent<BoxColliderComponent>(colliderSize[0],colliderSize[1]);
        auto projVelX = projVelocity * cos(projAngle);
        auto projVelY = projVelocity * sin(projAngle);
        newEnemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projVelX,projVelY),repFreq * 1000,projDur * 1000,damage);
        newEnemy.AddComponent<HealthComponent>(enemyHealth);
      }
    }
    ImGui::End();

    // Display a small overlay window to display the map position using the mouse
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
    ImGui::SetNextWindowBgAlpha(0.9f);
    if (ImGui::Begin("Map coordinates", NULL, windowFlags)) {
      ImGui::Text(
          "Map coordinates (x=%.1f, y=%.1f)",
          ImGui::GetIO().MousePos.x + camera.x,
          ImGui::GetIO().MousePos.y + camera.y
      );
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  }

};


#endif //INC_2D_GAME_ENGINE_SRC_SYSTEMS_RENDERGUISYSTEM_H_
