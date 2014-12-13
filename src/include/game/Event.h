/*
 * Copyright (c) 2014, Julien Bernard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <cstdint>
#include <functional>
#include <map>

#include <game/Entity.h>

namespace game {
  typedef uint64_t EventType;

#define INVALID_EVENT 0

  constexpr EventType Hash(const char *str, std::size_t sz) {
    return sz == 0 ? 0xcbf29ce484222325 : (str[0] ^ Hash(str + 1, sz - 1)) * 0x100000001b3;
  }

  inline EventType Hash(const std::string& str) {
    return Hash(str.c_str(), str.size());
  }


  struct Event {
    static const EventType type = INVALID_EVENT;
  };

  enum class EventStatus {
    KEEP, /**< The handler must be kept */
    DIE,  /**< The handler can be removed */
  };

  typedef std::function<EventStatus(Entity*, EventType, Event*)> EventHandler;

  class EventManager {
  public:

    void registerHandler(EventType type, EventHandler handler);

    template<typename E>
    void registerHandler(EventHandler handler) {
      static_assert(std::is_base_of<Event, E>::value, "E must be an Event");
      static_assert(E::type != INVALID_EVENT, "E must define its type");
      registerHandler(E::type, handler);
    }

    template<typename E, typename R, typename T>
    void registerHandler(R T::*pm, T *obj) {
      static_assert(std::is_base_of<Event, E>::value, "E must be an Event");
      static_assert(E::type != INVALID_EVENT, "E must define its type");
      registerHandler(E::type, std::bind(pm, obj));
    }

    void triggerEvent(Entity *origin, EventType type, Event *event);

    template<typename E>
    void triggerEvent(Entity *origin, E *event) {
      static_assert(std::is_base_of<Event, E>::value, "E must be an Event");
      static_assert(E::type != INVALID_EVENT, "E must define its type");
      triggerEvent(origin, E::type, event);
    }

  private:
    std::map<EventType, std::vector<EventHandler>> m_handlers;
  };

}

constexpr game::EventType operator"" _type(const char *str, std::size_t sz) {
  return game::Hash(str, sz);
}

#endif // GAME_EVENT_H
