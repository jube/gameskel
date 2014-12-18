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
#include "ClientBall.h"

#include "../common/CommonBall.h"
#include "../common/CommonGround.h"

void ClientBall::fixState(const game::Vector2f& pos, game::Vector2f& velocity) {
  m_pos.x = pos.x;
  m_pos.y = pos.y;
  m_velocity.x = velocity.x;
  m_velocity.y = velocity.y;
}

void ClientBall::update(float dt) {
  static constexpr float Y_LIMIT = CommonGround::HEIGHT / 2 - CommonBall::RADIUS;

  m_pos += m_velocity * dt;

#if 1
  if (m_pos.y > Y_LIMIT) {
    m_velocity.y = -m_velocity.y;
    m_pos.y = Y_LIMIT;
  }

  if (m_pos.y < - Y_LIMIT) {
    m_velocity.y = -m_velocity.y;
    m_pos.y = - Y_LIMIT;
  }
#endif
}

void ClientBall::render(sf::RenderWindow& window) {
  sf::CircleShape shape(CommonBall::RADIUS);
  shape.setOrigin(CommonBall::RADIUS, CommonBall::RADIUS);
  shape.setPosition(m_pos);
  shape.setFillColor(sf::Color::White);
  window.draw(shape);
}
