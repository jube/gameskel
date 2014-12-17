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
#include <game/graphics/Group.h>

#include "config.h"

int main() {
  // initialize
  game::Group group;
  sf::RenderWindow window(sf::VideoMode(1024, 768), "Foo (version " GAME_VERSION ")");
  window.setKeyRepeatEnabled(false);

  // load resources


  // add entities


  // main loop
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

          default:
            break;
        }

      }
    }

    // update
    sf::Time elapsed = clock.restart();
    group.update(elapsed.asSeconds());

    // render
    window.clear(sf::Color::White);
    group.render(window);
    window.display();
  }

  return 0;
}
