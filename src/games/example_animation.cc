/*
 * Copyright (c) 2014, Julien Bernard
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <cassert>

#include <boost/filesystem.hpp>

#include <SFML/Graphics.hpp>

#include <game/Animation.h>
#include <game/Resource.h>
#include <game/World.h>

#include "config.h"

namespace fs = boost::filesystem;

class Ninja : public game::Entity, public sf::Transformable {
public:
  Ninja(game::ResourceManager& manager)
  : m_texture_walking("walking")
  {
    m_texture_idle = manager.getTexture("ninja/idle.png");

    auto ninja = manager.getTexture("ninja/spritesheet.png");

    for (int i = -4; i <= 3; ++i) {
      int start = (4 - (std::abs(i))) * 88; // each sprite is 88x88
      m_texture_walking.addFrame({ ninja, { start, 0, 88, 88 }, 0.1f });
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

    m_texture_walking.update(dt);
  }

  virtual void render(sf::RenderWindow& window) {
    sf::Sprite sprite;

    if (m_walking) {
      auto texture = m_texture_walking.currentTexture();
      assert(texture);
      sprite.setTexture(*texture);
      sprite.setTextureRect(m_texture_walking.currentTextureRect());
    } else {
      sprite.setTexture(*m_texture_idle);
    }

    sprite.setPosition(getPosition());

    window.draw(sprite);
  }

private:
  bool m_walking = false;
  sf::Texture *m_texture_idle = nullptr;
  game::Animation m_texture_walking;
};

int main(int argc, char *argv[]) {
  // initialize
  game::World world;

  sf::RenderWindow window(sf::VideoMode(300, 200), "Animation (version " GAME_VERSION ")");
  window.setKeyRepeatEnabled(false);

  // load resources
  fs::path bindir_path(argv[0]);
  bindir_path = bindir_path.parent_path();
  fs::path datadir_path = bindir_path / fs::path("../share/" GAME_NAME);

  std::cout << "Path: " << datadir_path << std::endl;

  game::ResourceManager manager;

  manager.addSearchDir(datadir_path.string());
  manager.addSearchDir(GAME_DATADIR);

  // add entities

  Ninja ninja(manager);
  ninja.setPosition(106, 56);

  world.addEntity(&ninja, game::Memory::FROM_STACK);

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
    world.update(elapsed.asSeconds());

    // render
    window.clear(sf::Color::White);
    world.render(window);
    window.display();
  }

  return 0;
}
