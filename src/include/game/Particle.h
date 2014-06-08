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
#ifndef GAME_PARTICLE_H
#define GAME_PARTICLE_H

#include <game/Entity.h>
#include <game/Random.h>

#include <deque>
#include <functional>
#include <vector>

namespace game {

  struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float angle;
    float rotation;
    sf::Color color;
    float radius;
    float scale;
    unsigned points;
    float remaining_lifetime;
    float total_lifetime;
  };

  using ParticleAffector = std::function<void(Particle&, float)>;

  namespace Affectors {

    ParticleAffector forceAffector(sf::Vector2f force);

    ParticleAffector torqueAffector(float torque);

    ParticleAffector scaleAffector(float scale_change_per_seconds);

    ParticleAffector fadingAffector(float lifetime_ratio);

  }

  class ParticleSystem;

  class ParticleEmitter {
  public:
    ParticleEmitter(Random& random, float emission_rate)
    : m_random(random)
    , m_emission_rate(emission_rate)
    {
    }

    void setPosition(Distribution<sf::Vector2f> position) {
      m_position = position;
    }

    void setVelocity(Distribution<sf::Vector2f> velocity) {
      m_velocity = velocity;
    }

    void setAngle(Distribution<float> angle) {
      m_angle = angle;
    }

    void setRotation(Distribution<float> rotation) {
      m_rotation = rotation;
    }

    void setColor(Distribution<sf::Color> color) {
      m_color = color;
    }

    void setRadius(Distribution<float> radius) {
      m_radius = radius;
    }

    void setPoints(Distribution<unsigned> points) {
      m_points = points;
    }

    void setLifetime(Distribution<float> lifetime) {
      m_lifetime = lifetime;
    }

    void update(ParticleSystem& system, float dt);

  private:
    Random& m_random;
    float m_emission_rate;
    float m_emission_difference = 0.0f;
    Distribution<sf::Vector2f> m_position;
    Distribution<sf::Vector2f> m_velocity;
    Distribution<float> m_angle;
    Distribution<float> m_rotation;
    Distribution<sf::Color> m_color;
    Distribution<float> m_radius;
    Distribution<unsigned> m_points;
    Distribution<float> m_lifetime;
  };

  class ParticleSystem : public Entity {
  public:

    void addEmitter(ParticleEmitter *emitter);
    void addAffector(ParticleAffector affector);

    void emitParticle(const Particle& particle);

    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;

  private:
    void updateParticle(Particle& particle, float dt);

  private:
    std::vector<ParticleEmitter*> m_emitters;
    std::vector<ParticleAffector> m_affectors;
    std::deque<Particle> m_particles;
    float m_tick = 0;
  };

}

#endif // GAME_PARTICLE_H
