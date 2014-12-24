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
#include "ClientRacket.h"

#include "../common/CommonGround.h"
#include "../common/CommonRacket.h"

ClientRacket::ClientRacket(Location loc)
  : m_pos({ loc == Location::LEFT ? - CommonRacket::POSITION : CommonRacket::POSITION, 0.0f })
, m_velocity(0.0f, 0.0f) {
}

void ClientRacket::fixState(const game::Vector2f& pos, game::Vector2f& velocity) {
  m_pos.x = pos.x;
  m_pos.y = pos.y;
  m_velocity.x = velocity.x;
  m_velocity.y = velocity.y;
}

void ClientRacket::update(float dt) {
  static constexpr float Y_LIMIT = (CommonGround::HEIGHT - CommonRacket::HEIGHT) / 2;

  m_pos += m_velocity * dt;

#if 1

  if (m_pos.y > Y_LIMIT) {
    m_pos.y = Y_LIMIT;
  }

  if (m_pos.y < - Y_LIMIT) {
    m_pos.y = - Y_LIMIT;
  }

#endif
}

void ClientRacket::render(sf::RenderWindow& window) {
  sf::RectangleShape shape({ CommonRacket::WIDTH, CommonRacket::HEIGHT });
  shape.setOrigin(CommonRacket::WIDTH / 2, CommonRacket::HEIGHT / 2);
  shape.setPosition(m_pos);
  shape.setFillColor(sf::Color::White);
  window.draw(shape);
}
