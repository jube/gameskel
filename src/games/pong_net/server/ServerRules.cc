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
#include "ServerRules.h"

#include <cassert>
#include <cmath>

#include "../common/CommonBall.h"
#include "../common/CommonGround.h"
#include "../common/CommonRacket.h"

ServerRules::ServerRules(const ServerRacket& lft, const ServerRacket& rgt, ServerBall& ball)
  : l(&lft), r(&rgt), b(&ball)
  , m_l_points(0), m_r_points(0) {
}

static bool isTouchingX(const ServerBall& ball, const ServerRacket& racket) {
  float dx = std::abs(ball.getPosition().x - racket.getPosition().x);
  return dx < CommonBall::RADIUS + CommonRacket::WIDTH / 2;
}

static bool isTouchingY(const ServerBall& ball, const ServerRacket& racket) {
  float dy = std::abs(ball.getPosition().y - racket.getPosition().y);
  return dy < CommonRacket::HEIGHT / 2;
}

static constexpr float PI = 3.14159265358979323846f;

static float computeNewAngle(const ServerBall& ball, const ServerRacket& racket) {
  float dy = ball.getPosition().y - racket.getPosition().y;
  return 0.7 * dy / CommonRacket::HEIGHT * PI;
}

void ServerRules::update(float dt) {
  static constexpr float X_LIMIT = CommonGround::WIDTH / 2 - CommonBall::RADIUS;

  auto velocity = b->getVelocity();
  auto position = b->getPosition();

  if (velocity.x > 0) {
    if (position.x < r->getPosition().x && isTouchingX(*b, *r) && isTouchingY(*b, *r)) {
      float angle = computeNewAngle(*b, *r);
      velocity.x = std::cos(PI - angle) * CommonBall::VELOCITY;
      velocity.y = std::sin(PI - angle) * CommonBall::VELOCITY;
//       velocity.x = -velocity.x;
    }

    if (position.x > X_LIMIT) {
      m_l_points++;
      velocity.x = -velocity.x;
      b->resetPosition();
    }
  } else {
    if (position.x > l->getPosition().x && isTouchingX(*b, *l) && isTouchingY(*b, *l)) {
      float angle = computeNewAngle(*b, *l);
      velocity.x = std::cos(angle) * CommonBall::VELOCITY;
      velocity.y = std::sin(angle) * CommonBall::VELOCITY;
//       velocity.x = -velocity.x;
    }

    if (position.x < - X_LIMIT) {
      m_r_points++;
      velocity.x = -velocity.x;
      b->resetPosition();
    }
  }

  b->setVelocity(velocity);
}
