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
#include <game/graphics/Resource.h>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace game {

  template<typename T>
  T *ResourceCache<T>::findResource(const std::string& key) {
    auto it = m_cache.find(key);

    if (it != m_cache.end()) {
      return it->second.get();
    }

    return nullptr;
  }

  template<typename T>
  T *ResourceCache<T>::loadResource(const std::string& key, const std::string& path) {
    std::unique_ptr<T> obj(new T);

    bool loaded = obj->loadFromFile(path);
    assert(loaded);

    auto inserted = m_cache.emplace(key, std::move(obj));
    assert(inserted.second);

    return inserted.first->second.get();
  }

  template class ResourceCache<sf::Font>;
  template class ResourceCache<sf::SoundBuffer>;
  template class ResourceCache<sf::Texture>;


  sf::Font *ResourceManager::getFont(const std::string& path) {
    return getResource(path, m_fonts);
  }

  sf::SoundBuffer *ResourceManager::getSoundBuffer(const std::string& path) {
    return getResource(path, m_sounds);
  }

  sf::Texture *ResourceManager::getTexture(const std::string& path) {
    return getResource(path, m_textures);
  }

  template<typename T>
  T *ResourceManager::getResource(const std::string& path, ResourceCache<T>& cache) {
    auto res = cache.findResource(path);

    if (res != nullptr) {
      return res;
    }

    auto absolute_path = getAbsolutePath(path);

    if (absolute_path.empty()) {
      std::cerr << "Error! Could not find the following file: " << path << std::endl;
      return nullptr;
    }

    return cache.loadResource(path, absolute_path);
  }

}
