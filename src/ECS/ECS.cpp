#include "ECS.h"
#include <algorithm>
#include "Logger/logger.h"

uint32_t Entity::GetId() const {
  return id;
}

bool Entity::operator==(const Entity &rhs) const {
  return id == rhs.id;
}

bool Entity::operator!=(const Entity &rhs) const {
  return !(rhs == *this);
}
bool Entity::operator<(const Entity &other) const {
  return id < other.id;
}

template<typename T>
uint32_t Component<T>::GetId() {
  static auto id = nextId++;
  return id;
}

const std::vector<Entity> &System::GetEntities() const {
  return entities;
}

bool System::RemoveEntityFromSystem(Entity &entity) {
  auto find_lambda = [entity](Entity &current_entity){return current_entity == entity;};
  auto result = std::remove_if(entities.begin(),entities.end(),find_lambda);
  if(result == entities.end()){
    LOGGER_WARN("[System::RemoveEntityFromSystem] Tried to remove entity that dose not exist, ID: = {}",entity.GetId());
    return false;
  }
  entities.erase(result,entities.end());
  return true;
}

void System::AddEntityToSystem(Entity &entity) {
  entities.emplace_back(entity);
}
const Signature &System::GetComponentSignature() const {
  return componentSignature;
}

template<typename T>
void System::RequreComponent() {
    const auto componentID = Component<T>::GetId();
    if(componentID > MAX_COMPONENTS){
      LOGGER_WARN("[System::RequreComponent] God Component ID: {} Bigger than max component",componentID);
      return;
    }
  componentSignature.set(componentID);
}

Entity Registry::CreateEntity() {
  auto entityId = numEntities++;
  Entity entity(entityId);
  if(entityId >= entityComponentSignatures.size()){
    entityComponentSignatures.resize(entityId+1);
  }
  entitiesToBeAdded.emplace(entity);
  LOGGER_TRACE("Entity created with ID = {}", entityId);
  return entity;
}

void Registry::Update() {

}

void Registry::AddEntityToSystem(Entity &entity) {
  const auto entityId = entity.GetId();

  const auto& entityComponentSignature = entityComponentSignatures[entityId];

  for (auto& system: systems) {
    const auto& systemComponentSignature = system.second->GetComponentSignature();

    bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

    if (isInterested) {
      system.second->AddEntityToSystem(entity);
    }
  }
}

template<typename TSystem>
TSystem &Registry::GetSystem() const {
  auto type_id = std::type_index(typeid(TSystem));
  auto it = systems.find(type_id);
  return static_cast<std::shared_ptr<TSystem>>(it->second);
}

template<typename TSystem>
bool Registry::HasSystem() const {
  auto type_id = std::type_index(typeid(TSystem));
  return systems.find(type_id) != systems.end();
}

template<typename TSystem>
void Registry::RemoveSystem() {
  auto type_id = std::type_index(typeid(TSystem));
  systems.erase(type_id);
}

template<typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs &&... args) {
  auto newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
  auto type_id = std::type_index(typeid(newSystem));
  systems.emplace(std::make_pair(type_id,newSystem));

}

template<typename TComponent>
bool Registry::HasComponent(Entity entity) {
  return entityComponentSignatures[entity.GetId()].test(Component<TComponent>::GetId());
}

template<typename TComponent>
void Registry::RemoveComponent(Entity entity) {
  entityComponentSignatures[entity.GetId()].set(Component<TComponent>::GetId(),false);
}

template<typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity &entity, TArgs&& ...args) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  if(componentId >= componentPools.size()){
    componentPools.resize(componentId + 1, nullptr);
  }

  if(!componentPools[componentId]){
    componentPools[componentId] = std::make_shared<Pool<TComponent>>();
  }

  auto componentPool = static_cast<Pool<TComponent>>(componentPools[componentId]);

  if(entityId >= componentPool.GetSize()){
    componentPool.Resize(numEntities);
  }

  TComponent newComponent(std::forward<TArgs>(args)...);

  componentPool.Set(entityId, newComponent);

  entityComponentSignatures[entityId].set(componentId);
}
