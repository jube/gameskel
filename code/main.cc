/*
 * Copyright (c) 2014-2015, Julien Bernard
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
#include <cassert>
#include <cstdio>

#include "game/Action.h"
#include "game/Camera.h"
#include "game/Clock.h"
#include "game/EntityManager.h"
#include "game/Log.h"
#include "game/ResourceManager.h"
#include "game/WindowGeometry.h"
#include "game/WindowSettings.h"

#include "config.h"

static constexpr float AREA_WIDTH = 100.0f;
static constexpr float AREA_HEIGHT = 70.0f;

class Background : public game::Entity {
public:

  virtual void render(sf::RenderWindow& window) override {
    sf::RectangleShape shape({ AREA_WIDTH, AREA_HEIGHT });
    shape.setOrigin(AREA_WIDTH / 2, AREA_HEIGHT / 2);
    shape.setPosition(0.0f, 0.0f);
    shape.setFillColor(sf::Color(0xCC, 0xCC, 0xCC));
    window.draw(shape);
  }

};

class Minimap : public game::Entity {
public:
  Minimap(game::WindowGeometry& geometry)
  : m_geometry(geometry)
  {

  }

  virtual void render(sf::RenderWindow& window) override {
    sf::Vector2f pos = m_geometry.getCornerPosition({ -74.0f, -74.0f });

    sf::RectangleShape shape({ 64, 64 });
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(0xCC, 0x00, 0x00));
    window.draw(shape);
  }

private:
  game::WindowGeometry& m_geometry;
};

int main(int argc, char *argv[]) {
  game::Log::setLevel(game::Log::INFO);

  // initialize

  static constexpr unsigned INITIAL_WIDTH = 1024;
  static constexpr unsigned INITIAL_HEIGHT = 576;

  game::WindowSettings settings(INITIAL_WIDTH, INITIAL_HEIGHT, "Game template (version " GAME_VERSION ")");
  game::WindowGeometry geometry(INITIAL_WIDTH, INITIAL_HEIGHT);

  sf::RenderWindow window;
  settings.applyTo(window);
  window.setKeyRepeatEnabled(false);

  // load resources
  game::ResourceManager resources;
  resources.addSearchDir(GAME_DATADIR);

  // add cameras
  game::CameraManager cameras;

  game::FixedRatioCamera mainCamera(window, AREA_WIDTH, AREA_HEIGHT);
  cameras.addCamera(mainCamera);

  game::HeadsUpCamera hudCamera(window);
  cameras.addCamera(hudCamera);

  // add actions
  game::ActionManager actions;

  game::Action closeWindowAction("Close window");
  closeWindowAction.addCloseControl();
  closeWindowAction.addKeyControl(sf::Keyboard::Escape);
  actions.addAction(closeWindowAction);

  game::Action fullscreenAction("Fullscreen");
  fullscreenAction.addKeyControl(sf::Keyboard::F);
  actions.addAction(fullscreenAction);

  // add entities

  game::EntityManager mainEntities;
  Background bg;
  mainEntities.addEntity(bg);

  game::EntityManager hudEntities;
  Minimap map(geometry);
  hudEntities.addEntity(map);

  // main loop
  game::Clock clock;

  while (window.isOpen()) {
    // input
    sf::Event event;

    while (window.pollEvent(event)) {
      actions.update(event);
      cameras.update(event);
      geometry.update(event);
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      settings.toggleFullscreen();
      settings.applyTo(window);
      auto sz = window.getSize();

      // fake resize event (not sent when going fullscreen before SFML 2.3.1)
      sf::Event event;
      event.type = sf::Event::Resized;
      event.size.width = sz.x;
      event.size.height = sz.y;
      cameras.update(event);
      geometry.update(event);
    }

    // update
    auto elapsed = clock.restart();
    auto dt = elapsed.asSeconds();
    mainEntities.update(dt);
    hudEntities.update(dt);

    // render
    window.clear(sf::Color::White);

    mainCamera.configure(window);
    mainEntities.render(window);

    hudCamera.configure(window);
    hudEntities.render(window);

    window.display();

    actions.reset();
  }

  return 0;
}
