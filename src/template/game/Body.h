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
#ifndef GAME_BODY_H
#define GAME_BODY_H

#include <cstdint>

#include "Vector.h"

namespace game {

  /**
   * @ingroup model
   */
  struct Shape {
    enum Kind : uint32_t {
      CIRCLE = 0,
      RECTANGLE = 1,
    };

    Kind kind;

    union {

      struct {
        float radius;
      } circle;

      struct {
        float width;
        float height;
      } rectangle;
    };
  };

  struct Body;

  /**
   * @ingroup model
   */
  struct Manifold {
    Body *a;
    Body *b;
    float penetration;
    Vector2f normal;
  };

  /**
   * @ingroup model
   */
  struct Body {

    enum Type {
      STATIC,
      DYNAMIC,
    };

    Type type;
    Vector2f pos; /**< center of the body */
    Vector2f velocity;
    Shape shape;
    float inverse_mass;
    float restitution;
    uint32_t layers;

    static constexpr uint32_t ALL_LAYERS = static_cast<uint32_t>(-1);

    static bool collides(const Body& lhs, const Body& rhs, Manifold *m);
  };

}

#endif // GAME_BODY_H
