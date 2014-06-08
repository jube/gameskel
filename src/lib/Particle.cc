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

#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>

namespace game {

  inline
  bool operator<(const Particle& lhs, const Particle& rhs) {
    return lhs.remaining_lifetime > rhs.remaining_lifetime;
  }

  namespace Affectors {

    ParticleAffector forceAffector(sf::Vector2f force) {
      return [=](Particle& particle, float dt) {
        particle.velocity += dt * force;
      };
    }

    ParticleAffector torqueAffector(float torque) {
      return [=](Particle& particle, float dt) {
        particle.rotation += dt * torque;
      };
    }

    ParticleAffector scaleAffector(float scale_change_per_seconds) {
      return [=](Particle& particle, float dt) {
        particle.scale += dt * scale_change_per_seconds;

        if (particle.scale < 0) {
          particle.scale = 0;
        }
      };
    }

    ParticleAffector fadingAffector(float lifetime_ratio) {
      return [=](Particle& particle, float dt) {
        float ratio = particle.remaining_lifetime / particle.total_lifetime;

        if (ratio < lifetime_ratio) {
          particle.color.a = static_cast<sf::Uint8>(255 * ratio / lifetime_ratio);
        }
      };
    }

  }

  void ParticleEmitter::update(ParticleSystem& system, float dt) {
    float exact_count = dt * m_emission_rate + m_emission_difference;
    unsigned count = static_cast<unsigned>(exact_count);

    m_emission_difference = exact_count - count;

    for (unsigned i = 0; i < count; ++i) {
      Particle p;

      p.position = m_position ? m_position(m_random) : sf::Vector2f(0.0f, 0.0f);
      p.velocity = m_velocity ? m_velocity(m_random) : sf::Vector2f(0.0f, 0.0f);
      p.angle = m_angle ? m_angle(m_random) : 0.0f;
      p.rotation = m_rotation ? m_rotation(m_random) : 0.0f;
      p.color = m_color ? m_color(m_random) : sf::Color::Black;
      p.radius = m_radius ? m_radius(m_random) : 1.0f;
      p.scale = 1.0f;
      p.points = m_points ? m_points(m_random) : 30;
      p.remaining_lifetime = p.total_lifetime = m_lifetime ? m_lifetime(m_random) : 1.0;

      system.emitParticle(p);
    }
  }


  void ParticleSystem::addEmitter(ParticleEmitter *emitter) {
    m_emitters.push_back(emitter);
  }

  void ParticleSystem::addAffector(ParticleAffector affector) {
    m_affectors.push_back(affector);
  }

  void ParticleSystem::emitParticle(const Particle& particle) {
    m_particles.push_back(particle);
    std::push_heap(m_particles.begin(), m_particles.end());
  }

  void ParticleSystem::update(float dt) {
    for (auto& p : m_particles) {
      p.remaining_lifetime -= dt;
    }

    while (m_particles.front().remaining_lifetime < 0) {
      std::pop_heap(m_particles.begin(), m_particles.end());
      m_particles.pop_back();
    }

    assert(std::is_heap(m_particles.begin(), m_particles.end()));

    for (auto emitter : m_emitters) {
      emitter->update(*this, dt);
    }

    for (auto& p : m_particles) {
      for (auto affector : m_affectors) {
        affector(p, dt);
      }

      updateParticle(p, dt);
    }

    m_tick -= dt;

    if (m_tick < 0) {
      m_tick += 1;
      std::cerr << m_particles.size() << '\n';
    }
  }

  void ParticleSystem::render(sf::RenderWindow& window)  {
    for (auto& p : m_particles) {
      if (p.color.a == 0) {
        continue;
      }

      sf::CircleShape shape(p.radius, p.points);
      shape.setOrigin(p.radius, p.radius);

      shape.setFillColor(p.color);
      shape.setPosition(p.position);
      shape.setRotation(p.angle);
      shape.setScale(p.scale, p.scale);
      window.draw(shape);
    }
  }

  void ParticleSystem::updateParticle(Particle& particle, float dt) {
    particle.position += dt * particle.velocity;
    particle.angle += dt * particle.rotation;
  }

}
