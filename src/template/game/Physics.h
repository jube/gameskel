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
#ifndef GAME_PHYSICS_H
#define GAME_PHYSICS_H

#include <functional>
#include <map>
#include <vector>

#include "Body.h"

namespace game {

  /**
   * @ingroup model
   */
  class Physics {
  public:
    typedef std::function<void(Body&, Body&)> CollisionCallback;

    void addBody(Body& body);
    void clear();

    void setCallback(Body& body, CollisionCallback callback);
    void removeCallback(Body& body);

    void update(float dt);

  private:
    std::vector<Body *> m_dynamic_bodies;
    std::vector<Body *> m_static_bodies;

    std::map<Body *, CollisionCallback> m_callbacks;
  };

}

#endif // GAME_PHYSICS_H
