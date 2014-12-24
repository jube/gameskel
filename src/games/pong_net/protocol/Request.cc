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
#include "Request.h"

sf::Packet& operator<<(sf::Packet& packet, const Request& req) {
  packet << static_cast<uint16_t>(req.type);
  packet << req.origin;

  switch (req.type) {
    case RequestType::PLAY:
      packet << req.play.port;
      break;

    case RequestType::MOVE:
      packet << static_cast<uint8_t>(req.move.dir);
      break;

    case RequestType::END:
      break;
  }

  return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Request& req) {
  uint16_t type;
  packet >> type;
  req.type = static_cast<RequestType>(type);

  packet >> req.origin;

  switch (req.type) {
    case RequestType::PLAY:
      packet >> req.play.port;
      break;

    case RequestType::MOVE: {
      uint8_t dir;
      packet >> dir;
      req.move.dir = static_cast<Direction>(dir);
      break;

      case RequestType::END:
        break;
      }
  }

  return packet;
}
