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
#include <game/graphics/Animation.h>

#include <iostream>

namespace game {

  void Animation::addFrame(const Frame& frame) {
    if (m_frames.empty()) {
      m_current_duration_in_frame = frame.duration;
      m_current_frame = 0;
    }

    m_frames.push_back(frame);
  }

  void Animation::update(float dt) {
    if (m_frames.empty()) {
      return;
    }

    m_current_duration_in_frame -= dt;

    while (m_current_duration_in_frame < 0) {
      m_current_frame = (m_current_frame + 1) % m_frames.size();
      m_current_duration_in_frame += m_frames[m_current_frame].duration;
    }
  }

  sf::Texture *Animation::currentTexture() {
    if (m_frames.empty()) {
      std::cerr << "Error! The animation does not have any frame: " << name() << std::endl;
      return nullptr;
    }

    return m_frames[m_current_frame].texture;
  }

  sf::IntRect Animation::currentTextureRect() {
    if (m_frames.empty()) {
      std::cerr << "Error! The animation does not have any frame: " << name() << std::endl;
      return sf::IntRect();
    }

    return m_frames[m_current_frame].bounds;
  }

}
