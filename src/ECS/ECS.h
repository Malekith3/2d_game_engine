#ifndef ECS_H
#define ECS_H

#include <set>
#include <cstdint>
#include <bitset>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include "Logger/logger.h"

const uint32_t  MAX_COMPONENTS = 32;
////////////////////////////////////////////////////////////////////////////////
// Signature
////////////////////////////////////////////////////////////////////////////////
// We use a bitset (1s and 0s) to keep track of which components an entity has,
// and also helps keep track of which entities a system is interested in.
////////////////////////////////////////////////////////////////////////////////
using Signature = std::bitset<MAX_COMPONENTS>;

struct BComponent{
 protected:
  inline static  uint32_t  nextId = 0;
};

template<typename T>
class Component : BComponent{
 public:
  static uint32_t GetId() {
    static auto id = nextId++;
    return id;
  }
};


class Entity{
 private:
  uint32_t id;
 public:
  explicit Entity(uint32_t id) : id(id){};
  uint32_t GetId() const;
  bool operator==(const Entity &rhs) const;
  bool operator!=(const Entity &rhs) const;
  bool operator<(const Entity& other) const;

  template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
  template <typename TComponent> void RemoveComponent();
  template <typename TComponent> bool HasComponent() const;
  template <typename TComponent> TComponent& GetComponent() const;

  // Hold a pointer to the entity's owner registry
  class Registry* registry;
};
////////////////////////////////////////////////////////////////////////////////
// Pool
////////////////////////////////////////////////////////////////////////////////
// A pool is just a vector (contiguous data) of objects of type T
////////////////////////////////////////////////////////////////////////////////
class IPool{
 public:
  virtual ~IPool(){};
};

template <typename TObject>
class Pool: public IPool{
 private:
  std::vector<TObject> data;

 public:
  explicit Pool(uint32_t size = 100){data.resize(size);}
  void Add(TObject object){data.emplace_back(object);}
  bool IsEmpty() const {return data.empty();}
  int GetSize() const {return  data.size();}
  void Resize(uint32_t size){data.resize(size);}
  void Clear() {data.clear();}
  void Set(uint32_t index, TObject object){data[index] = object;}
  TObject& Get(uint32_t index){return static_cast<TObject&>(data[index]);}
  TObject& operator [](uint32_t index){return data[index];}
};

////////////////////////////////////////////////////////////////////////////////
// System
////////////////////////////////////////////////////////////////////////////////
// The system processes entities that contain a specific signature
////////////////////////////////////////////////////////////////////////////////
class System{
 private:
  Signature componentSignature;
  std::vector<Entity> entities;


 public:
  System()  = default;
  virtual ~System() = default;
  void AddEntityToSystem(const Entity &entity);
  bool RemoveEntityFromSystem(Entity &entity);
  const std::vector<Entity> &GetEntities() const;
  const Signature &GetComponentSignature() const;

  template<typename TComponent> void RequreComponent();
};
////////////////////////////////////////////////////////////////////////////////
// Registry
////////////////////////////////////////////////////////////////////////////////
// The registry manages the creation and destruction of entities, add systems,
// and components.
////////////////////////////////////////////////////////////////////////////////

class Registry{
 private:
  uint32_t numEntities = 0;
  std::set<Entity> entitiesToBeAdded;
  std::set<Entity> entitiesToBeKilled;

  std::vector<std::shared_ptr<IPool>> componentPools;
  std::vector<Signature> entityComponentSignatures;
  std::unordered_map<std::type_index,std::shared_ptr<System>> systems;

 public:
  Registry() = default;
  Entity CreateEntity();
  void AddEntityToSystem(const Entity &entity);
  template<typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
  template<typename TComponent> void RemoveComponent(Entity entity);
  template<typename TComponent> bool HasComponent(Entity entity);
  template<typename  TComponent> TComponent& GetComponent(Entity entity);

  template<typename  TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
  template<typename TSystem>  void RemoveSystem();
  template<typename  TSystem> bool HasSystem() const;
  template<typename  TSystem> TSystem& GetSystem() const;
  void Update();
};

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  if (componentId >= componentPools.size()) {
    componentPools.resize(componentId + 1, nullptr);
  }

  if (!componentPools[componentId]) {
    std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
    componentPools[componentId] = newComponentPool;
  }

  std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

  if (entityId >= componentPool->GetSize()) {
    componentPool->Resize(numEntities);
  }

  TComponent newComponent(std::forward<TArgs>(args)...);

  componentPool->Set(entityId, newComponent);

  entityComponentSignatures[entityId].set(componentId);
  LOGGER_TRACE("[Registry::AddComponent] Component ID = {} was added to Entity ID = {}", componentId, entityId);
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
template<typename TComponent>
TComponent &Registry::GetComponent(Entity entity) {
  Pool<TComponent>* componentPool = componentPools[Component<TComponent>::GetId()];
  return componentPool->Get(entity.GetId());
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

template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...args) {
  registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::RemoveComponent() {
  registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const {
  return registry->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const {
  return registry->GetComponent<TComponent>(*this);
}

#endif