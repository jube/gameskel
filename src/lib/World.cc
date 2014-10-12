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
#include <game/World.h>

#include <cassert>
#include <algorithm>
#include <memory>

namespace game {

  namespace {

    template<typename T>
    class null_delete {
    public:
      void operator()(T*) const {
        // nothing to do
      }
    };

  }

  void World::update(float dt) {
    std::sort(m_entities.begin(), m_entities.end(), [](const EntityPtr& e1, const EntityPtr& e2) {
      return e1->priority() < e2->priority();
    });

    for (auto& entity : m_entities) {
      entity->update(dt);
    }
  }

  void World::render(sf::RenderWindow& window) {
    for (auto& entity : m_entities) {
      entity->render(window);
    }
  }

  void World::addEntity(Entity *e, Memory from) {
    switch (from) {
      case Memory::FROM_HEAP:
        m_entities.emplace_back(e, std::default_delete<Entity>());
        break;
      case Memory::FROM_STACK:
        m_entities.emplace_back(e, null_delete<Entity>());
        break;
    }
  }

  void World::removeEntity(Entity *e) {
    std::remove_if(m_entities.begin(), m_entities.end(), [=](const EntityPtr& ptr) {
      return e == ptr.get();
    });
  }

  void World::registerHandler(EventType type, EventHandler handler) {
    assert(handler);
    auto it = m_handlers.find(type);

    if (it == m_handlers.end()) {
      bool inserted;
      std::tie(it, inserted) = m_handlers.insert(std::make_pair(type, std::vector<EventHandler>()));
      assert(inserted);
    }

    it->second.push_back(handler);
  }

  void World::triggerEvent(Entity *origin, EventType type, Event *event) {
    auto it = m_handlers.find(type);

    if (it == m_handlers.end()) {
      return;
    }

    std::vector<EventHandler> kept;

    for (auto handler : it->second) {
      if (handler(origin, type, event) == EventStatus::KEEP) {
        kept.push_back(handler);
      }
    }

    std::swap(it->second, kept);
  }

}
