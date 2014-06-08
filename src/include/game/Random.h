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
#ifndef GAME_RANDOM_H
#define GAME_RANDOM_H

#include <random>
#include <functional>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace game {

  typedef std::mt19937 Random;

  template<typename T>
  using Distribution = std::function<T(Random&)>;

  namespace Distributions {

    Distribution<float> constantDistribution(float value);

    Distribution<float> uniformDistribution(float min, float max);

    Distribution<float> normalDistribution(float mean, float stddev);

    Distribution<unsigned> constantDistribution(unsigned value);

    Distribution<unsigned> uniformDistribution(unsigned min, unsigned max);

    Distribution<sf::Vector2f> constantDistribution(sf::Vector2f value);

    Distribution<sf::Vector2f> rectangularDistribution(sf::Vector2f center, sf::Vector2f half_size);

    Distribution<sf::Vector2f> circleDistribution(sf::Vector2f center, float radius);

    Distribution<sf::Vector2f> arcDistribution(sf::Vector2f center, float radius, float angle1, float angle2);

    Distribution<sf::Vector2f> diskDistribution(sf::Vector2f center, float radius);

    Distribution<sf::Color> constantDistribution(sf::Color value);

  }

}

#endif // GAME_RANDOM_H
