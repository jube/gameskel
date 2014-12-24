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
#include <cassert>
#include <cinttypes>
#include <csignal>
#include <cstdio>
#include <cstdlib>

#include <array>
#include <atomic>
#include <thread>

#include <SFML/Network.hpp>

#include <game/base/Clock.h>
#include <game/base/Queue.h>
#include <game/base/Log.h>
#include <game/model/World.h>

#include "common/Config.h"

#include "protocol/Request.h"
#include "protocol/Response.h"

#include "server/Player.h"
#include "server/ServerBall.h"
#include "server/ServerRacket.h"
#include "server/ServerRules.h"

#include "config.h"

static std::atomic_bool should_continue(true);

static void signal_handler(int sig) {
  assert(sig == SIGINT || sig == SIGTERM);
  should_continue = false;
}

static void receiveLoop(sf::UdpSocket& socket, game::Queue<Request>& queue) {
  sf::Packet packet;
  sf::IpAddress address;
  uint16_t port;

  while (should_continue) {
    socket.receive(packet, address, port);

    Request req;
    packet >> req;
    packet.clear();

    if (req.type == RequestType::END) {
      break;
    }

    assert(req.origin == 1 || req.origin == 2);

    queue.push(req);
  }
}

Player acceptPlayer(sf::TcpListener& listener, sf::TcpSocket& client, uint16_t id) {
  Player player;
  player.id = id;

  if (listener.accept(client) !=  sf::Socket::Done) {
    game::Log::error(game::Log::NETWORK, "Could accept a client on port %" PRIu16 "\n", Config::TCP_PORT);
    std::exit(EXIT_FAILURE);
  }

  player.remote.address = client.getRemoteAddress();

  game::Log::info(game::Log::NETWORK, "Player #%" PRIu16 " connecting on port %" PRIu16 "\n", id, Config::TCP_PORT);

  sf::Packet packet;
  client.receive(packet);

  Request req;
  packet >> req;

  assert(req.type == RequestType::PLAY);
  assert(req.origin == 0);

  player.remote.port = req.play.port;

  game::Log::info(game::Log::NETWORK, "Player #%" PRIu16 " joinable on port %" PRIu16 "\n", id, player.remote.port);

  Response rep;
  rep.type = ResponseType::PLAYER;
  rep.player.id = id;

  packet.clear();
  packet << rep;

  client.send(packet);

  return player;
}

static void endGame(uint16_t port) {
  Request req;
  req.type = RequestType::END;

  sf::Packet packet;
  packet << req;

  sf::UdpSocket socket;
  socket.send(packet, sf::IpAddress::LocalHost, port);
}

int main() {
  std::printf("Pong Server (version " GAME_VERSION ")\n");

  game::Log::setLevel(game::Log::INFO);
  game::Log::info(game::Log::GENERAL, "Starting the server...\n");
  game::Log::info(game::Log::GENERAL, "You can stop it with CTRL+C or with the kill command.\n");

  // set signal handlers

  auto previous_sigint_handler = std::signal(SIGINT, signal_handler);

  if (previous_sigint_handler == SIG_ERR) {
    game::Log::error(game::Log::GENERAL, "Could not set the signal handler for SIGINT\n");
    return EXIT_FAILURE;
  }

  auto previous_sigterm_handler = std::signal(SIGTERM, signal_handler);

  if (previous_sigterm_handler == SIG_ERR) {
    game::Log::error(game::Log::GENERAL, "Could not set the signal handler for SIGTERM\n");
    return EXIT_FAILURE;
  }

  // prepare the udp socket

  sf::UdpSocket socket;

  if (socket.bind(Config::UDP_PORT) != sf::Socket::Done) {
    game::Log::error(game::Log::NETWORK, "Could not bind the UDP server on port %" PRIu16 "\n", Config::UDP_PORT);
    return EXIT_FAILURE;
  }


  // waiting for players

  sf::TcpListener listener;

  if (listener.listen(Config::TCP_PORT) != sf::Socket::Done) {
    game::Log::error(game::Log::NETWORK, "Could not bind the TCP server on port %" PRIu16 "\n", Config::TCP_PORT);
    return EXIT_FAILURE;
  }

  game::Log::info(game::Log::NETWORK, "Bound the server on port %" PRIu16 "\n", Config::TCP_PORT);

  std::array<Player, 2> players;

  sf::TcpSocket client1;
  players[0] = acceptPlayer(listener, client1, 1);
  client1.disconnect();

  sf::TcpSocket client2;
  players[1] = acceptPlayer(listener, client2, 2);
  client2.disconnect();

  // prepare the receiving thread

  game::Queue<Request> queue;
  std::thread receiver(receiveLoop, std::ref(socket), std::ref(queue));

  // add entities
  game::World world;
  ServerBall ball;
  ServerRacket lft(ServerRacket::Location::LEFT);
  ServerRacket rgt(ServerRacket::Location::RIGHT);
  ServerRules rules(lft, rgt, ball);

  world.addModel(ball).addModel(lft).addModel(rgt).addModel(rules);


  // main loop
  game::Clock clock;

  queue.clear();

  while (should_continue) {
    // update
    game::Time elapsed = clock.restart();
    world.update(elapsed.asSeconds());

    Request req;

    while (queue.poll(req)) {
      switch (req.type) {
        case RequestType::MOVE: {
          ServerRacket::Move move = ServerRacket::Move::STOP;

          switch (req.move.dir) {
            case Direction::UP:
              move = ServerRacket::Move::UP;
              break;

            case Direction::DOWN:
              move = ServerRacket::Move::DOWN;
              break;

            case Direction::HERE:
              move = ServerRacket::Move::STOP;
              break;
          }

          if (req.origin == 1) {
            lft.setMove(move);
          } else {
            assert(req.origin == 2);
            rgt.setMove(move);
          }

          break;
        }

        default:
          assert(false);
          break;
      }
    }

    // send new state
    Response rep;
    rep.type = ResponseType::STATE;
    rep.state.ball_position = ball.getPosition();
    rep.state.ball_velocity = ball.getVelocity();
    rep.state.lft_position = lft.getPosition();
    rep.state.lft_velocity = lft.getVelocity();
    rep.state.lft_points = rules.getLPoints();
    rep.state.rgt_position = rgt.getPosition();
    rep.state.rgt_velocity = rgt.getVelocity();
    rep.state.rgt_points = rules.getRPoints();

    sf::Packet packet;
    packet << rep;

    for (auto& player : players) {
      socket.send(packet, player.remote.address, player.remote.port);
    }

    //     std::printf("FPS: %f\n", 1 / elapsed.asSeconds());

    queue.waitIfEmpty(0.01f - elapsed.asSeconds());
  }

  game::Log::info(game::Log::GENERAL, "Stopping the server...\n");

  endGame(socket.getLocalPort());

  receiver.join();
  socket.unbind();

  return EXIT_SUCCESS;
}
