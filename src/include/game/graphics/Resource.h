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
#ifndef GAME_RESOURCE_H
#define GAME_RESOURCE_H

#include <string>
#include <map>
#include <memory>
#include <vector>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

namespace game {

  template<typename T>
  class ResourceCache {
  public:
    T *findResource(const std::string& key);
    T *loadResource(const std::string& key, const std::string& path);
  private:
    std::map<std::string, std::unique_ptr<T>> m_cache;
  };

  extern template class ResourceCache<sf::Font>;
  extern template class ResourceCache<sf::SoundBuffer>;
  extern template class ResourceCache<sf::Texture>;

  class ResourceManager {
  public:
    void addSearchDir(std::string path);

    sf::Font *getFont(const std::string& path);
    sf::SoundBuffer *getSoundBuffer(const std::string& path);
    sf::Texture *getTexture(const std::string& path);

  private:
    std::vector<std::string> m_searchdirs;

    ResourceCache<sf::Font> m_fonts;
    ResourceCache<sf::SoundBuffer> m_sounds;
    ResourceCache<sf::Texture> m_textures;

  private:
    template<typename T>
    T *getResource(const std::string& path, ResourceCache<T>& cache);
  };


}

#endif // GAME_RESOURCE_H
