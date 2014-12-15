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
#include <game/model/Body.h>

#include <cassert>
#include <cmath>

#include <game/base/Log.h>

namespace game {

  static constexpr float skin = 0.15;

  static float lengthSquared(const Vector2f& v) {
    return v.x * v.x + v.y * v.y;
  }

  static float clamp(float value, float min, float max) {
    assert(min < max);

    if (value > max) {
      return max;
    }

    if (value < min) {
      return min;
    }

    return value;
  }

  static bool collidesCircleVsCircle(const Body& lhs, const Body& rhs, Manifold *m) {
    Vector2f n = rhs.pos - lhs.pos;
    float r = lhs.shape.circle.radius + rhs.shape.circle.radius;
    float d2 = lengthSquared(n);

    if (d2 > r * r) {
      return false;
    }

    float d = std::sqrt(d2);

    if (d != 0) {
      m->penetration = r - d;
      m->normal = n / d;
    } else {
      m->penetration = r / 2;
      m->normal = { 1.0f, 0.0f };
    }

    return m->penetration > skin;
  }

  static bool collidesRectVsCircle(const Body& lhs, const Body& rhs, Manifold *m) {
    Vector2f n = rhs.pos - lhs.pos;

    Vector2f closest = n;

    float x_extent = lhs.shape.rectangle.width / 2;
    float y_extent = lhs.shape.rectangle.height / 2;

    closest.x = clamp(closest.x, -x_extent, x_extent);
    closest.y = clamp(closest.y, -y_extent, y_extent);

    bool inside = false;

    if (n == closest) {
      inside = true;

      if (std::abs(n.x) > std::abs(n.y)) {
        if (closest.x > 0) {
          closest.x = x_extent;
        } else {
          closest.x = -x_extent;
        }
      } else {
        if (closest.y > 0) {
          closest.y = y_extent;
        } else {
          closest.y = -y_extent;
        }
      }
    }

    Vector2f normal = n - closest;

    float d2 = lengthSquared(normal);
    float r = rhs.shape.circle.radius;

    if (d2 > r * r && !inside) {
      return false;
    }

    float d = std::sqrt(d2);

    Log::debug(Log::PHYSICS, "Collision Rectangle vs Circle\n");
    Log::debug(Log::PHYSICS, "  r: %f / d: %f\n", r, d);
    Log::debug(Log::PHYSICS, "  n: (%f, %f)\n", n.x, n.y);
    Log::debug(Log::PHYSICS, "  closest: (%f, %f)\n", closest.x, closest.y);

    if (inside) {
      m->normal = -normal / d;
      m->penetration = r + d;
    } else {
      m->normal = normal / d;
      m->penetration = r - d;
    }

    return m->penetration > skin;
  }

  static bool collidesCircleVsRect(const Body& lhs, const Body& rhs, Manifold *m) {
    bool collide = collidesRectVsCircle(rhs, lhs, m);
    m->normal = -m->normal;
    return collide;
  }

  static bool collidesRectVsRect(const Body& lhs, const Body& rhs, Manifold *m) {
    Vector2f n = rhs.pos - lhs.pos;

    float l_extent_x = lhs.shape.rectangle.width / 2;
    float r_extent_x = rhs.shape.rectangle.width / 2;

    float x_overlap = l_extent_x + r_extent_x - std::abs(n.x);

    if (x_overlap <= 0) {
      return false;
    }

    float l_extent_y = lhs.shape.rectangle.height / 2;
    float r_extent_y = rhs.shape.rectangle.height / 2;

    float y_overlap = l_extent_y + r_extent_y - std::abs(n.y);

    if (y_overlap <= 0) {
      return false;
    }

    if (x_overlap > y_overlap) {
      if (n.x < 0) {
        m->normal = { -1.0f, 0.0f };
      } else {
        m->normal = {  1.0f, 0.0f };
      }

      m->penetration = x_overlap;
    } else {
      if (n.y < 0) {
        m->normal = { 0.0f, -1.0f };
      } else {
        m->normal = { 0.0f,  1.0f };
      }

      m->penetration = y_overlap;
    }

    return m->penetration > skin;
  }

  bool Body::collides(const Body& lhs, const Body& rhs, Manifold *m) {
    assert(m);

    switch (lhs.shape.kind) {
      case Shape::CIRCLE:
        switch (rhs.shape.kind) {
          case Shape::CIRCLE:
            return collidesCircleVsCircle(lhs, rhs, m);

          case Shape::RECTANGLE:
            return collidesCircleVsRect(lhs, rhs, m);
        }

      case Shape::RECTANGLE:
        switch (rhs.shape.kind) {
          case Shape::CIRCLE:
            return collidesRectVsCircle(lhs, rhs, m);

          case Shape::RECTANGLE:
            return collidesRectVsRect(lhs, rhs, m);
        }

    }

    assert(false);
    return false;
  }

}
