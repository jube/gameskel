/*
 * Copyright (c) 2014-2015, Julien Bernard
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

#include "game/Action.h"
#include "game/Clock.h"
#include "game/EntityManager.h"
#include "game/Log.h"
#include "game/Resources.h"

#include "config.h"

int main(int argc, char *argv[]) {
  game::Log::setLevel(game::Log::INFO);

  // initialize

  static constexpr unsigned INITIAL_WIDTH = 1024;
  static constexpr unsigned INITIAL_HEIGHT = 768;

  sf::RenderWindow window(sf::VideoMode(INITIAL_WIDTH, INITIAL_HEIGHT), "Game template (version " GAME_VERSION ")");
  window.setKeyRepeatEnabled(false);

  // load resources
  game::ResourceManager resources;
  resources.addSearchDir(GAME_DATADIR);

  // add entities

  game::EntityManager entities;

  // add actions
  game::ActionManager actions;

  auto closeWindowAction = std::make_shared<game::Action>("Close window");
  closeWindowAction->addCloseControl();
  closeWindowAction->addKeyControl(sf::Keyboard::Escape);
  actions.addAction(closeWindowAction);

  // main loop
  game::Clock clock;

  while (window.isOpen()) {
    // input
    sf::Event event;

    while (window.pollEvent(event)) {
      actions.update(event);
    }

    if (closeWindowAction->isActive()) {
      window.close();
    }

    // update
    auto elapsed = clock.restart();
    entities.update(elapsed.asSeconds());

    // render
    window.clear(sf::Color::White);
    entities.render(window);
    window.display();
  }

  return 0;
}
