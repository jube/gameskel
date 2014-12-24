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
#include <csignal>
#include <cstdio>
#include <cinttypes>

#include <atomic>
#include <thread>

#include <SFML/Network.hpp>

#include <game/base/Log.h>
#include <game/base/Queue.h>
#include <game/graphics/Group.h>

#include "common/CommonGround.h"
#include "common/Config.h"

#include "protocol/Request.h"
#include "protocol/Response.h"

#include "client/ClientBall.h"
#include "client/ClientGround.h"
#include "client/ClientPoints.h"
#include "client/ClientRacket.h"

#include "config.h"

static std::atomic_bool should_continue(true);

static void signal_handler(int sig) {
  assert(sig == SIGINT || sig == SIGTERM);
  should_continue = false;
}

static void receiveLoop(sf::UdpSocket& socket, game::Queue<Response>& queue) {
  sf::Packet packet;
  sf::IpAddress address;
  uint16_t port;

  while (should_continue) {
    socket.receive(packet, address, port);

    Response rep;
    packet >> rep;
    packet.clear();

    if (rep.type == ResponseType::END) {
      break;
    }

    queue.push(rep);
  }
}

static sf::FloatRect computeViewport(unsigned width, unsigned height) {
  float w = static_cast<float>(width);
  float h = static_cast<float>(height);

  float r = w / h * CommonGround::HEIGHT / CommonGround::WIDTH;

  sf::FloatRect vp;

  if (width < 2 * height) {
    vp.left = 0.0f;
    vp.width = 1.0;

    vp.top = (1 - r) / 2;
    vp.height = r;
  } else {
    vp.top = 0.0f;
    vp.height = 1.0f;

    vp.left = (1 - 1 / r) / 2;
    vp.width = 1 / r;
  }

  return vp;
}

uint16_t sendPlay(sf::TcpSocket& server, uint16_t port) {
  Request req;
  req.type = RequestType::PLAY;
  req.origin = 0; // we do not have an id yet
  req.play.port = port;

  sf::Packet packet;
  packet << req;

  server.send(packet);
  packet.clear();

  server.receive(packet);

  Response rep;
  packet >> rep;

  assert(rep.type == ResponseType::PLAYER);
  return rep.player.id;
}

static void endGame(uint16_t port) {
  Response rep;
  rep.type = ResponseType::END;

  sf::Packet packet;
  packet << rep;

  sf::UdpSocket socket;
  socket.send(packet, sf::IpAddress::LocalHost, port);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::printf("Usage: pong_client <hostname>\n");
    return EXIT_FAILURE;
  }

  std::printf("Pong (version " GAME_VERSION ")\n");

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

  // preparing the udp socket

  sf::UdpSocket socket;

  if (socket.bind(sf::Socket::AnyPort) != sf::Socket::Done) {
    game::Log::error(game::Log::NETWORK, "Could not bind a UDP socket\n");
    return EXIT_FAILURE;
  }

  // connecting to the server
  sf::IpAddress host(argv[1]);

  sf::TcpSocket server;
  auto status = server.connect(host, Config::TCP_PORT);

  if (status != sf::Socket::Done) {
    game::Log::error(game::Log::NETWORK, "Could not connect to '%s' on port %" PRIu16 "\n", argv[1], Config::TCP_PORT);
    return EXIT_FAILURE;
  }

  auto id = sendPlay(server, socket.getLocalPort());
  std::printf("You are player #%" PRIu16 "\n", id);

  // prepare the receiving thread

  game::Queue<Response> queue;
  std::thread receiver(receiveLoop, std::ref(socket), std::ref(queue));

  // initialize

  static constexpr unsigned INITIAL_WIDTH = 1024;
  static constexpr unsigned INITIAL_HEIGHT = 768;

  sf::RenderWindow window(sf::VideoMode(INITIAL_WIDTH, INITIAL_HEIGHT), "Pong (version " GAME_VERSION ")");
  window.setKeyRepeatEnabled(false);

  // add entities

  game::Group group;
  ClientGround ground;
  ClientBall ball;
  ClientRacket lft(ClientRacket::Location::LEFT);
  ClientRacket rgt(ClientRacket::Location::RIGHT);
  ClientPoints points;

  group.addEntity(ground).addEntity(ball).addEntity(lft).addEntity(rgt).addEntity(points);

  // main loop

  sf::View view;
  view.setCenter({ 0.0f, 0.0f });
  view.setSize({ 200.0f, 100.0f });

  view.setViewport(computeViewport(INITIAL_WIDTH, INITIAL_HEIGHT));

  sf::Clock clock;

  Request req;
  req.type = RequestType::MOVE;
  req.origin = id;

  while (window.isOpen()) {
    // input
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::KeyPressed) {

        switch (event.key.code) {
          case sf::Keyboard::Escape:
            window.close();
            break;

          case sf::Keyboard::Up:
            req.move.dir = Direction::UP;
            break;

          case sf::Keyboard::Down:
            req.move.dir = Direction::DOWN;
            break;

          default:
            break;
        }

      } else if (event.type == sf::Event::KeyReleased) {

        switch (event.key.code) {
          case sf::Keyboard::Up:
            req.move.dir = Direction::HERE;
            break;

          case sf::Keyboard::Down:
            req.move.dir = Direction::HERE;
            break;

          default:
            break;
        }

      } else if (event.type == sf::Event::Resized) {
        view.setViewport(computeViewport(event.size.width, event.size.height));
      }
    }

    sf::Packet packet;
    packet << req;
    socket.send(packet, host, Config::UDP_PORT);

    Response rep;

    while (queue.poll(rep)) {
      switch (rep.type) {
        case ResponseType::STATE:
          ball.fixState(rep.state.ball_position, rep.state.ball_velocity);
          lft.fixState(rep.state.lft_position, rep.state.lft_velocity);
          rgt.fixState(rep.state.rgt_position, rep.state.rgt_velocity);
          points.fixState(rep.state.lft_points, rep.state.rgt_points);
          break;

        default:
          assert(false);
      }
    }

    // update
    sf::Time elapsed = clock.restart();
    group.update(elapsed.asSeconds());

    // render
    window.clear({ 0x30, 0x30, 0x30 });
    window.setView(view);
    group.render(window);
    window.display();
  }

  endGame(socket.getLocalPort());

  receiver.join();
  socket.unbind();

  return 0;
}
