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
#include "Ball.h"

#include "Racket.h"



void Ball::update(float dt) {
  static constexpr float X_LIMIT = Ground::WIDTH / 2 - RADIUS;
  static constexpr float Y_LIMIT = Ground::HEIGHT / 2 - RADIUS;

  m_pos += m_velocity * dt;

  if (m_pos.y > Y_LIMIT) {
    m_velocity.y = -m_velocity.y;
    m_pos.y = Y_LIMIT;
  }

  if (m_pos.y < - Y_LIMIT) {
    m_velocity.y = -m_velocity.y;
    m_pos.y = - Y_LIMIT;
  }

  if (m_pos.x > X_LIMIT) {
    m_velocity.x = - m_velocity.x;
    m_pos.x = X_LIMIT;
  }

  if (m_pos.x < - X_LIMIT) {
    m_velocity.x = - m_velocity.x;
    m_pos.x = - X_LIMIT;
  }
}

void Ball::render(sf::RenderWindow& window) {
  sf::CircleShape shape(RADIUS);
  shape.setOrigin(RADIUS, RADIUS);
  shape.setPosition(m_pos);
  shape.setFillColor(sf::Color::White);
  window.draw(shape);
}
