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
#ifndef SERVER_BALL_H
#define SERVER_BALL_H

#include <game/base/Vector.h>
#include <game/model/Model.h>

class ServerBall : public game::Model {
public:

  ServerBall();

  const game::Vector2f& getPosition() const {
    return m_pos;
  }

  void resetPosition() {
    m_pos = { 0.0f, 0.0f };
  }

  const game::Vector2f& getVelocity() const {
    return m_velocity;
  }

  void setVelocity(const game::Vector2f& velocity) {
    m_velocity = velocity;
  }

  virtual void update(float dt) override;

private:
  float m_step;
  game::Vector2f m_pos;
  game::Vector2f m_velocity;
};



#endif // SERVER_BALL_H
