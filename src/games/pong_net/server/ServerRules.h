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
#ifndef SERVER_RULES_H
#define SERVER_RULES_H

#include <cstdint>

#include <game/model/Model.h>

#include "ServerBall.h"
#include "ServerRacket.h"

class ServerRules : public game::Model {
public:
  ServerRules(const ServerRacket& lft, const ServerRacket& rgt, ServerBall& ball);

  uint32_t getLPoints() const {
    return m_l_points;
  }

  uint32_t getRPoints() const {
    return m_r_points;
  }

  virtual void update(float dt) override;

private:
  const ServerRacket * const l;
  const ServerRacket * const r;
  ServerBall * const b;
  uint32_t m_l_points;
  uint32_t m_r_points;
};

#endif // SERVER_RULES_H
