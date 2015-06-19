/*
 * Copyright (c) 2014-2015, Julien Bernard
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
#include "Event.h"

#include <cassert>

namespace game {

  void EventManager::registerHandler(EventType type, EventHandler handler) {
    assert(handler);
    auto it = m_handlers.find(type);

    if (it == m_handlers.end()) {
      bool inserted;
      std::tie(it, inserted) = m_handlers.insert(std::make_pair(type, std::vector<EventHandler>()));
      assert(inserted);
    }

    it->second.push_back(handler);
  }

  void EventManager::triggerEvent(EventType type, Event *event) {
    auto it = m_handlers.find(type);

    if (it == m_handlers.end()) {
      return;
    }

    std::vector<EventHandler> kept;

    for (auto handler : it->second) {
      if (handler(type, event) == EventStatus::KEEP) {
        kept.push_back(handler);
      }
    }

    std::swap(it->second, kept);
  }

}
