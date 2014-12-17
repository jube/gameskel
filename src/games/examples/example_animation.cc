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

#include <SFML/Graphics.hpp>

#include <game/graphics/Animation.h>
#include <game/graphics/Resource.h>
#include <game/graphics/Group.h>

#include "config.h"

class Ninja : public game::Entity, public sf::Transformable {
public:
  Ninja(game::ResourceManager& manager)
    : m_animation("walking") {
    m_texture_idle = manager.getTexture("ninja/idle.png");

    auto ninja = manager.getTexture("ninja/spritesheet.png");

    for (int i = -4; i <= 3; ++i) {
      int start = (4 - (std::abs(i))) * 88; // each sprite is 88x88
      m_animation.addFrame({ ninja, { start, 0, 88, 88 }, 0.1f });
    }
  }

  void walk() {
    m_walking = true;
  }

  void stop() {
    m_walking = false;
  }

  virtual void update(float dt) {
    if (!m_walking) {
      return;
    }

    m_animation.update(dt);
  }

  virtual void render(sf::RenderWindow& window) {
    sf::Sprite sprite;

    if (m_walking) {
      auto texture = m_animation.currentTexture();
      assert(texture);
      sprite.setTexture(*texture);
      sprite.setTextureRect(m_animation.currentTextureRect());
    } else {
      sprite.setTexture(*m_texture_idle);
    }

    sprite.setPosition(getPosition());

    window.draw(sprite);
  }

private:
  bool m_walking = false;
  sf::Texture *m_texture_idle = nullptr;
  game::Animation m_animation;
};

int main(int argc, char *argv[]) {
  // initialize

  sf::RenderWindow window(sf::VideoMode(300, 200), "Animation (version " GAME_VERSION ")");
  window.setKeyRepeatEnabled(false);

  // load resources

  game::ResourceManager manager;
  manager.addSearchDir(GAME_DATADIR);

  // add entities

  game::Group group;
  Ninja ninja(manager);
  ninja.setPosition(106, 56);

  group.addEntity(ninja);

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

          case sf::Keyboard::Space:
            ninja.walk();
            break;

          default:
            break;
        }
      } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
          case sf::Keyboard::Space:
            ninja.stop();
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
