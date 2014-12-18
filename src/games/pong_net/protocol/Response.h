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
#ifndef PROTOCOL_RESPONSE_H
#define PROTOCOL_RESPONSE_H

#include <cstdint>

#include <SFML/Network.hpp>

#include <game/model/Vector.h>

enum class ResponseType : uint16_t {
  PLAYER = 1,
  STATE = 2,
  END = 3,
};

struct PlayerResponse {
  uint16_t id;
};

struct StateResponse {
  game::Vector2f ball_position;
  game::Vector2f ball_velocity;
  game::Vector2f lft_position;
  game::Vector2f lft_velocity;
  uint32_t lft_points;
  game::Vector2f rgt_position;
  game::Vector2f rgt_velocity;
  uint32_t rgt_points;
};

struct Response {
  ResponseType type;
  union {
    PlayerResponse player;
    StateResponse state;
  };
};

sf::Packet& operator<<(sf::Packet& packet, const Response& rep);
sf::Packet& operator>>(sf::Packet& packet, Response& rep);

#endif // PROTOCOL_RESPONSE_H
