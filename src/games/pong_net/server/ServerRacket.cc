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
#include "ServerRacket.h"

#include "../common/CommonGround.h"
#include "../common/CommonRacket.h"

#include "ServerConfig.h"

ServerRacket::ServerRacket(Location loc)
  : m_step(0)
  , m_move(Move::STOP)
  , m_pos{ loc == Location::LEFT ? - CommonRacket::POSITION : CommonRacket::POSITION, 0.0f }
  , m_velocity{0.0f, 0.0f} {
}

void ServerRacket::update(float dt) {
  static constexpr float Y_LIMIT = (CommonGround::HEIGHT - CommonRacket::HEIGHT) / 2;

  for (m_step += dt; m_step > ServerConfig::STEP; m_step -= ServerConfig::STEP) {
    switch (m_move) {
      case Move::UP:
        m_velocity.y = - CommonRacket::VELOCITY;
        break;

      case Move::DOWN:
        m_velocity.y = + CommonRacket::VELOCITY;
        break;

      case Move::STOP:
        m_velocity.y = 0.0f;
        break;
    }

    m_pos += m_velocity * ServerConfig::STEP;

    if (m_pos.y > Y_LIMIT) {
      m_pos.y = Y_LIMIT;
    }

    if (m_pos.y < - Y_LIMIT) {
      m_pos.y = -Y_LIMIT;
    }
  }

}
