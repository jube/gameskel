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
#ifndef PROTOCOL_REQUEST_H
#define PROTOCOL_REQUEST_H

#include <cstdint>

#include <SFML/Network.hpp>

enum class RequestType : uint16_t {
  PLAY = 1,
  MOVE = 2,
  END = 3,
};

struct PlayRequest {
  uint16_t port;
};

enum class Direction : uint8_t {
  UP = 1,
  DOWN = 2,
  HERE = 3,
};

struct MoveRequest {
  Direction dir;
};

struct Request {
  RequestType type;
  uint16_t origin;
  union {
    PlayRequest play;
    MoveRequest move;
  };
};

sf::Packet& operator<<(sf::Packet& packet, const Request& req);
sf::Packet& operator>>(sf::Packet& packet, Request& req);

#endif // PROTOCOL_REQUEST_H
