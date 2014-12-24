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
#include <cstdio>

#include <game/base/Log.h>
#include <game/graphics/Group.h>

#include "local/Ball.h"
#include "local/Ground.h"
#include "local/Racket.h"
#include "local/Rules.h"

#include "config.h"

static sf::FloatRect computeViewport(unsigned width, unsigned height) {
  float w = static_cast<float>(width);
  float h = static_cast<float>(height);

  float r = w / h * Ground::HEIGHT / Ground::WIDTH;

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

int main() {
  std::printf("Pong (version " GAME_VERSION ")\n");

  // initialize

  static constexpr unsigned INITIAL_WIDTH = 1024;
  static constexpr unsigned INITIAL_HEIGHT = 768;

  sf::RenderWindow window(sf::VideoMode(INITIAL_WIDTH, INITIAL_HEIGHT), "Pong (version " GAME_VERSION ")");
  window.setKeyRepeatEnabled(false);

  // add entities

  game::Group group;
  Ground ground;
  Racket lft(Racket::Location::LEFT);
  Racket rgt(Racket::Location::RIGHT);
  Ball ball;

  Rules rules(lft, rgt, ball);

  group.addEntity(ground).addEntity(ball).addEntity(lft).addEntity(rgt).addEntity(rules);

  // main loop

  sf::View view;
  view.setCenter({ 0.0f, 0.0f });
  view.setSize({ 200.0f, 100.0f });

  view.setViewport(computeViewport(INITIAL_WIDTH, INITIAL_HEIGHT));

  sf::Clock clock;

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

          case sf::Keyboard::S:
            lft.move(Racket::Move::UP);
            break;

          case sf::Keyboard::X:
            lft.move(Racket::Move::DOWN);
            break;

          case sf::Keyboard::Up:
            rgt.move(Racket::Move::UP);
            break;

          case sf::Keyboard::Down:
            rgt.move(Racket::Move::DOWN);
            break;

          default:
            break;
        }

      } else if (event.type == sf::Event::KeyReleased) {

        switch (event.key.code) {
          case sf::Keyboard::S:
            lft.move(Racket::Move::STOP);
            break;

          case sf::Keyboard::X:
            lft.move(Racket::Move::STOP);
            break;

          case sf::Keyboard::Up:
            rgt.move(Racket::Move::STOP);
            break;

          case sf::Keyboard::Down:
            rgt.move(Racket::Move::STOP);
            break;

          default:
            break;
        }

      } else if (event.type == sf::Event::Resized) {
        view.setViewport(computeViewport(event.size.width, event.size.height));
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

  return 0;
}
