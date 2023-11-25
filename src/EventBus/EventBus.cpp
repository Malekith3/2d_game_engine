//
// Created by sanek on 21/11/2023.
//

#include "EventBus.h"
EventBus::EventBus() {
  LOGGER_INFO("[EventBus::EventBus] CTOR was called");
}

EventBus::~EventBus() {
  LOGGER_INFO("[EventBus::EventBus] DTOR was called");
}

void EventBus::Reset() {
  subscribers.clear();
}

void IEventCallback::Execute(Event &e) {
  Call(e);
}
