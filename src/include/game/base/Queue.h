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
#ifndef GAME_QUEUE_H
#define GAME_QUEUE_H

#include <deque>
#include <mutex>

namespace game {

  /**
   * @ingroup base
   */
  template<typename T>
  class Queue {
  public:
    bool poll(T& value) {
      std::lock_guard<std::mutex> lock(m_mutex);

      if (m_queue.empty()) {
        return false;
      }

      value = m_queue.front();
      m_queue.pop();
    }

    void push(const T& value) {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_queue.push_back(value);
    }

    void clear() {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_queue.clear();
    }

  private:
    std::mutex m_mutex;
    std::deque<T> m_queue;
  };

}

#endif // GAME_QUEUE_H