#ifndef ECS_H
#define ECS_H

#include <set>
#include <cstdint>
#include <bitset>
#include <vector>
#include <typeindex>
#include <unordered_map>

const uint32_t  MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

struct BComponent{
 protected:
  static  uint32_t  nextId;
};

template<typename T>
class Component : BComponent{
 private:
  uint32_t id;
 public:
  static uint32_t GetId();
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
};
class IPool{
  virtual ~IPool(){};
};

template <typename TObject>
class Pool: public IPool{
 private:
  std::vector<TObject> data;

 public:
  explicit Pool(uint32_t size = 100){data.resize(size);}
  virtual ~Pool() = default;
  void Add(TObject object){data.emplace_back(object);}
  bool IsEmpty() const {return data.empty();}
  int GetSize() const {return  data.size();}
  void Resize(uint32_t size){data.resize(size);}
  void Clear() {data.clear();}
  void Set(uint32_t index, TObject object){data[index] = object;}
  TObject& Get(uint32_t index){return static_cast<TObject&>(data[index]);}
  TObject& operator [](uint32_t index){return data[index];}
};


class System{
 private:
  Signature componentSignature;
  std::vector<Entity> entities;


 public:
  System()  = default;
  virtual ~System() = default;
  void AddEntityToSystem(Entity &entity);
  bool RemoveEntityFromSystem(Entity &entity);
  const std::vector<Entity> &GetEntities() const;
  const Signature &GetComponentSignature() const;

  template<typename TComponent> void RequreComponent();
};

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
  void AddEntityToSystem(Entity &entity);
  template<typename TComponent, typename ...TArgs>void AddComponent(Entity &entity, TArgs&& ...args);
  template<typename TComponent> void RemoveComponent(Entity entity);
  template<typename TComponent> bool HasComponent(Entity entity);

  template<typename  TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
  template<typename TSystem>  void RemoveSystem();
  template<typename  TSystem> bool HasSystem() const;
  template<typename  TSystem> TSystem& GetSystem() const;
  void Update();
};

#endif