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
#ifndef GAME_ANIMATION_H
#define GAME_ANIMATION_H

#include <vector>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <game/Entity.h>

namespace game {

  struct AnimationFrame {
    sf::Texture *texture;
    sf::IntRect bounds;
    float duration;
  };

  class Animation {
  public:
    Animation(std::string name)
    : m_name(std::move(name)) {
    }

    const std::string& name() {
      return m_name;
    }

    void addFrame(const AnimationFrame& frame);

    void update(float dt);

    sf::Texture *currentTexture();
    sf::IntRect currentTextureRect();

  private:
    std::string m_name;
    std::size_t m_current_frame = 0;
    float m_current_duration_in_frame = 0.0f;
    std::vector<AnimationFrame> m_frames;
  };

}

#endif // GAME_ANIMATION_H
