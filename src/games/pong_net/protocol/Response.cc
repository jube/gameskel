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
#include "Response.h"

static sf::Packet& operator<<(sf::Packet& packet, const game::Vector2f& v) {
  return packet << v.x << v.y;
}

static sf::Packet& operator>>(sf::Packet& packet, game::Vector2f& v) {
  return packet >> v.x >> v.y;
}


sf::Packet& operator<<(sf::Packet& packet, const Response& rep) {
  packet << static_cast<uint16_t>(rep.type);

  switch (rep.type) {
    case ResponseType::PLAYER:
      packet << rep.player.id;
      break;

    case ResponseType::STATE:
      packet << rep.state.ball_position << rep.state.ball_velocity;
      packet << rep.state.lft_position << rep.state.lft_velocity << rep.state.lft_points;
      packet << rep.state.rgt_position << rep.state.rgt_velocity << rep.state.rgt_points;
      break;

    case ResponseType::END:
      break;
  }

  return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Response& rep) {
  uint16_t type;
  packet >> type;

  rep.type = static_cast<ResponseType>(type);

  switch (rep.type) {
    case ResponseType::PLAYER:
      packet >> rep.player.id;
      break;

    case ResponseType::STATE:
      packet >> rep.state.ball_position >> rep.state.ball_velocity;
      packet >> rep.state.lft_position >> rep.state.lft_velocity >> rep.state.lft_points;
      packet >> rep.state.rgt_position >> rep.state.rgt_velocity >> rep.state.rgt_points;
      break;

    case ResponseType::END:
      break;
  }

  return packet;
}
