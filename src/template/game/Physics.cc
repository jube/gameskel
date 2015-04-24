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
#include "Physics.h"

#include <cassert>

#include "Log.h"

namespace game {

  static float dotProduct(const Vector2f& rhs, const Vector2f& lhs) {
    return rhs.x * lhs.x + rhs.y * lhs.y;
  }

  static void resolveCollision(Manifold& m) {
    assert(m.a && m.b);

    Vector2f relative_velocity = m.b->velocity - m.a->velocity;
    float velocity_along_normal = dotProduct(relative_velocity, m.normal);

    Log::debug(Log::PHYSICS, "relative_velocity: (%f, %f)\n", relative_velocity.x, relative_velocity.y);

    if (velocity_along_normal > 0) {
      return;
    }

    Log::debug(Log::PHYSICS, "velocity_along_normal: %f\n", velocity_along_normal);

    float a_inverse_mass = m.a->inverse_mass;
    float b_inverse_mass = m.b->inverse_mass;

    float restitution = std::max(m.a->restitution, m.b->restitution);

    float j = -(1 + restitution) * velocity_along_normal / (a_inverse_mass + b_inverse_mass);

    Vector2f impulse = j * m.normal;
    m.a->velocity -= a_inverse_mass * impulse;
    m.b->velocity += b_inverse_mass * impulse;
  }

  static void correctPosition(Manifold& m) {
    const float percent = 0.9;
    const float slop = 0.1;

    float a_inverse_mass = m.a->inverse_mass;
    float b_inverse_mass = m.b->inverse_mass;

    Vector2f correction = std::max(m.penetration - slop, 0.0f) / (a_inverse_mass + b_inverse_mass) * percent * m.normal;

    m.a->pos -= a_inverse_mass * correction;
    m.b->pos += b_inverse_mass * correction;
  }

  void Physics::addBody(Body *body) {
    switch (body->type) {
      case Body::DYNAMIC:
        m_dynamic_bodies.push_back(body);
        break;

      case Body::STATIC:
        m_static_bodies.push_back(body);
        break;
    }
  }

  void Physics::clear() {
    m_dynamic_bodies.clear();
    m_static_bodies.clear();
    m_callbacks.clear();
  }

  void Physics::setCallback(Body *body, CollisionCallback callback) {
    m_callbacks[body] = callback;
  }

  void Physics::removeCallback(Body *body) {
    m_callbacks.erase(body);
  }

  static void logBody(const Body& body) {
    Log::debug(Log::PHYSICS, "- body %p\n", &body);
    Log::debug(Log::PHYSICS, "\t(%f, %f) @ (%f, %f)\n", body.pos.x, body.pos.y, body.velocity.x, body.velocity.y);

    switch (body.shape.kind) {
      case Shape::CIRCLE:
        Log::debug(Log::PHYSICS, "\tcircle: %f\n", body.shape.circle.radius);
        break;

      case Shape::RECTANGLE:
        Log::debug(Log::PHYSICS, "\trectangle: %f x %f\n", body.shape.rectangle.width, body.shape.rectangle.height);
        break;
    }

    Log::debug(Log::PHYSICS, "\trestitution: %f\n", body.restitution);
  }

  void Physics::update(float dt) {
    // speed simulation
    for (auto body : m_dynamic_bodies) {
      body->pos.x += body->velocity.x * dt;
      body->pos.y += body->velocity.y * dt;
    }

    // collision resolution
    Manifold m;
    std::vector<Manifold> manifolds;

    for (auto body : m_dynamic_bodies) {
      m.a = body;

      for (auto other_body : m_dynamic_bodies) {
        if (body == other_body) {
          continue;
        }

        if (body < other_body) {
          // avoid adding the manifold twice
          continue;
        }

        if ((body->layers & other_body->layers) == 0) {
          continue;
        }

        if (Body::collides(*body, *other_body, &m)) {
          m.b = other_body;
          manifolds.push_back(m);
          Log::debug(Log::PHYSICS, "Collision detected!\n");
          logBody(*body);
          logBody(*other_body);
          Log::debug(Log::PHYSICS, "Manifold: %f (%f, %f)\n", m.penetration, m.normal.x, m.normal.y);
        }
      }

      for (auto other_body : m_static_bodies) {
        if (Body::collides(*body, *other_body, &m)) {
          m.b = other_body;
          manifolds.push_back(m);
          Log::debug(Log::PHYSICS, "Collision detected!\n");
          logBody(*body);
          logBody(*other_body);
          Log::debug(Log::PHYSICS, "Manifold: %f (%f, %f)\n", m.penetration, m.normal.x, m.normal.y);
        }
      }
    }

    for (auto& m : manifolds) {
      resolveCollision(m);
      Log::debug(Log::PHYSICS, "After collision resolution:\n");
      logBody(*m.a);
      logBody(*m.b);
    }

    for (auto& m : manifolds) {
      correctPosition(m);
      Log::debug(Log::PHYSICS, "After position correction:\n");
      logBody(*m.a);
      logBody(*m.b);
    }

    for (auto& m : manifolds) {
      auto it = m_callbacks.find(m.a);

      if (it != m_callbacks.end()) {
        assert(it->second);
        it->second(m.a, m.b);
      }

      it = m_callbacks.find(m.b);

      if (it != m_callbacks.end()) {
        assert(it->second);
        it->second(m.b, m.a);
      }
    }
  }

}
