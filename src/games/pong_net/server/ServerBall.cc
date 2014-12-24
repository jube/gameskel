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
#include "ServerBall.h"

#include "../common/CommonBall.h"
#include "../common/CommonGround.h"

#include "ServerConfig.h"

ServerBall::ServerBall()
  : m_step(0)
  , m_pos {0.0f, 0.0f}
, m_velocity { - CommonBall::VELOCITY, 0.0f} {
}

void ServerBall::update(float dt) {
  static constexpr float Y_LIMIT = CommonGround::HEIGHT / 2 - CommonBall::RADIUS;

  for (m_step += dt; m_step > ServerConfig::STEP; m_step -= ServerConfig::STEP) {

    m_pos += m_velocity * ServerConfig::STEP;

    if (m_pos.y > Y_LIMIT) {
      m_velocity.y = -m_velocity.y;
      m_pos.y = Y_LIMIT;
    }

    if (m_pos.y < - Y_LIMIT) {
      m_velocity.y = -m_velocity.y;
      m_pos.y = - Y_LIMIT;
    }
  }

}
