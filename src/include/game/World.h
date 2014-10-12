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
#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <functional>
#include <map>
#include <type_traits>
#include <vector>

#include <SFML/Graphics.hpp>

#include <game/Entity.h>
#include <game/Event.h>

namespace game {
  enum class Memory {
    FROM_HEAP,
    FROM_STACK,
  };

  class World {
  public:

    void update(float dt);
    void render(sf::RenderWindow& window);

    void addEntity(Entity *e, Memory from = Memory::FROM_HEAP);
    void removeEntity(Entity *e);

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

    class EntityPtr {
    public:
      EntityPtr(Entity *entity, std::function<void(Entity*)> deleter)
      : m_entity(entity)
      , m_deleter(deleter)
      {
      }

      EntityPtr(const EntityPtr&) = delete;
      EntityPtr& operator=(const EntityPtr&) = delete;

      EntityPtr(EntityPtr&& other)
      : m_entity(other.m_entity)
      , m_deleter(std::move(other.m_deleter)) {
      }

      EntityPtr& operator=(EntityPtr&& other) {
        m_deleter(m_entity);
        m_entity = other.m_entity;
        m_deleter = std::move(other.m_deleter);
        return *this;
      }

      ~EntityPtr() {
        m_deleter(m_entity);
      }

      Entity *get() {
        return m_entity;
      }

      const Entity *get() const {
        return m_entity;
      }

      Entity *operator->() {
        return get();
      }

      const Entity *operator->() const {
        return get();
      }

      Entity& operator*() {
        return *get();
      }

      const Entity& operator*() const {
        return *get();
      }

    private:
      Entity *m_entity;
      std::function<void(Entity*)> m_deleter;
    };

    std::vector<EntityPtr> m_entities;
    std::map<EventType, std::vector<EventHandler>> m_handlers;
  };


}


#endif // GAME_WORLD_H
