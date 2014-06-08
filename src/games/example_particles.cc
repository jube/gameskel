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
#include <game/Particle.h>
#include <game/Resource.h>
#include <game/World.h>

#include "config.h"

int main() {
  // initialize
  game::World world;
  sf::RenderWindow window(sf::VideoMode(1024, 768), "Particles (version " GAME_VERSION ")");
  window.setKeyRepeatEnabled(false);

  // load resources

  game::ResourceManager manager;

  manager.addSearchDir(GAME_DATADIR);

  // add entities

  game::Random random;

  game::ParticleEmitter emitter(random, 30);
  emitter.setPosition(game::Distributions::constantDistribution({ 512.0f, 700.0f }));
  emitter.setVelocity(game::Distributions::arcDistribution({ 0.0f, 0.0f }, 200.0f, M_PI - 0.3, M_PI + 0.3));

  emitter.setRotation(game::Distributions::uniformDistribution(-180.0f, 180.0f));

  emitter.setColor(game::Distributions::constantDistribution(sf::Color::Red));
  emitter.setRadius(game::Distributions::uniformDistribution(8.0f, 10.0f));
  emitter.setPoints(game::Distributions::uniformDistribution(3u, 6u));

  emitter.setLifetime(game::Distributions::uniformDistribution(4.0f, 10.0f));

  game::ParticleSystem system;
  system.addEmitter(&emitter);
  system.addAffector(game::Affectors::forceAffector({ 0, 40 }));
  system.addAffector(game::Affectors::scaleAffector(0.3));
  system.addAffector(game::Affectors::fadingAffector(1.0));

  world.addEntity(&system, game::Memory::FROM_STACK);

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
    world.update(elapsed.asSeconds());

    // render
    window.clear(sf::Color::White);
    world.render(window);
    window.display();
  }

  return 0;
}
