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
#ifndef LOCAL_BALL_H
#define LOCAL_BALL_H

#include <game/graphics/Entity.h>

#include "Racket.h"
#include "Ground.h"

class Ball : public game::Entity {
public:

  static constexpr float RADIUS = 2.0f;
  static constexpr float VELOCITY = 100.0f;

  Ball()
    : m_pos({ 0.0f, 0.0f })
    , m_velocity(VELOCITY, 0.0f) {
  }

  const sf::Vector2f& getPosition() const {
    return m_pos;
  }

  void resetPosition() {
    m_pos = { 0.0f, 0.0f };
  }

  const sf::Vector2f& getVelocity() const {
    return m_velocity;
  }

  void setVelocity(const sf::Vector2f& velocity) {
    m_velocity = velocity;
  }

  virtual void update(float dt) override;

  virtual void render(sf::RenderWindow& window) override;

private:
  sf::Vector2f m_pos;
  sf::Vector2f m_velocity;
};

#endif // LOCAL_BALL_H
