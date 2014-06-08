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
#include <game/Random.h>

#include <cassert>
#include <cmath>

namespace game {

  namespace Distributions {

    Distribution<float> constantDistribution(float value) {
      return [=](Random&) {
        return value;
      };
    }

    Distribution<float> uniformDistribution(float min, float max) {
      return std::uniform_real_distribution<float>(min, max);
    }

    Distribution<float> normalDistribution(float mean, float stddev) {
      return std::normal_distribution<float>(mean, stddev);
    }

    Distribution<unsigned> constantDistribution(unsigned value) {
      return [=](Random&) {
        return value;
      };
    }

    Distribution<unsigned> uniformDistribution(unsigned min, unsigned max) {
      return std::uniform_int_distribution<unsigned>(min, max);
    }

    Distribution<sf::Vector2f> constantDistribution(sf::Vector2f value) {
      return [=](Random&) {
        return value;
      };
    }

    Distribution<sf::Vector2f> rectangularDistribution(sf::Vector2f center, sf::Vector2f half_size) {
      assert(half_size.x > 0);
      assert(half_size.y > 0);

      return [=](Random& random) {
        std::uniform_real_distribution<float> dist_x(-half_size.x, half_size.x);
        std::uniform_real_distribution<float> dist_y(-half_size.y, half_size.y);

        return sf::Vector2f(center.x + dist_x(random), center.y + dist_y(random));
      };
    }

    Distribution<sf::Vector2f> circleDistribution(sf::Vector2f center, float radius) {
      assert(radius > 0);

      return [=](Random& random) {
        std::uniform_real_distribution<float> dist_angle(0, 2 * M_PI);
        float angle = dist_angle(random);

        return sf::Vector2f(
          center.x + radius * std::sin(angle),
          center.y + radius * std::cos(angle)
        );
      };
    }

    Distribution<sf::Vector2f> arcDistribution(sf::Vector2f center, float radius, float angle1, float angle2) {
      assert(radius > 0);
      assert(angle1 < angle2);

      return [=](Random& random) {
        std::uniform_real_distribution<float> dist_angle(angle1, angle2);
        float angle = dist_angle(random);

        return sf::Vector2f(
          center.x + radius * std::sin(angle),
          center.y + radius * std::cos(angle)
        );
      };
    }

    Distribution<sf::Vector2f> diskDistribution(sf::Vector2f center, float radius) {
      assert(radius > 0);

      return [=](Random& random) {
        std::uniform_real_distribution<float> dist_angle(0, 2 * M_PI);
        float angle = dist_angle(random);

        std::uniform_real_distribution<float> dist_radius(0, radius);
        float r = dist_radius(random);

        return sf::Vector2f(
          center.x + r * std::sin(angle),
          center.y + r * std::cos(angle)
        );
      };
    }

    Distribution<sf::Color> constantDistribution(sf::Color value) {
      return [=](Random&) {
        return value;
      };
    }

  }

}
