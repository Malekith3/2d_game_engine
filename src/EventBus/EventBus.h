//
// Created by sanek on 21/11/2023.
//

#ifndef INC_2D_GAME_ENGINE_SRC_EVENTBUS_EVENTBUS_H_
#define INC_2D_GAME_ENGINE_SRC_EVENTBUS_EVENTBUS_H_
#include <map>
#include <list>
#include <typeindex>
#include <functional>
#include "Logger/logger.h"
#include "EventBus/Event.h"

class IEventCallback{
 private:
  virtual void Call(Event& e) = 0;

 public:
  virtual ~IEventCallback() = default;
  void Execute(Event& e);
};

template<typename TOwner, typename TEvent>
class EventCallBack : public IEventCallback{
 private:
  typedef void (TOwner::*CallbackFunction)(TEvent&);

  TOwner* ownerInstance;
  CallbackFunction callbackFunction;

  void Call(Event &e) override {
    std::invoke(callbackFunction,ownerInstance,static_cast<TEvent&>(e));
  }
 public:
  EventCallBack(TOwner* ownerInstance, CallbackFunction callbackFunction){
    this->ownerInstance = ownerInstance;
    this->callbackFunction = callbackFunction;
  }

  ~EventCallBack() override = default;
};

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
 public:
  EventBus();
  ~EventBus();
  template<class TEvent, class  TOwner>
  void SubscribeToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)){
    if(!subscribers[typeid(TEvent)].get()){
      subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
    }

      auto subscriber = std::make_unique<EventCallBack<TOwner,TEvent>>(ownerInstance,callbackFunction);
      subscribers[typeid(TEvent)]->emplace_back(std::move(subscriber));
  };

  template<class TEvent, typename  ...TArgs>
  void EmitEvent(TArgs&& ...args){
    auto handlers = subscribers[typeid(TEvent)].get();

    if(!handlers)
      return;

    for(auto it = handlers->begin(); it != handlers->end(); it++){
        auto handler = it->get();
        TEvent event(std::forward<TArgs>(args)...);
        handler->Execute(event);
    }
  };

  void Reset();

 private:
  std::map<std::type_index,std::unique_ptr<HandlerList>> subscribers;
};

#endif //INC_2D_GAME_ENGINE_SRC_EVENTBUS_EVENTBUS_H_
