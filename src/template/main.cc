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
#include "game/Resources.h"
#include "game/WindowSettings.h"

#include "config.h"

static constexpr float SIZE = 100.0f;

class Background : public game::Entity {
public:

  virtual void render(sf::RenderWindow& window) override {
    sf::RectangleShape shape({ SIZE, SIZE / 2 });
    shape.setOrigin(SIZE / 2, SIZE / 4);
    shape.setPosition(0.0f, 0.0f);
    shape.setFillColor(sf::Color(0xCC, 0xCC, 0xCC));
    window.draw(shape);
  }

};

class Minimap : public game::Entity {
public:
  Minimap(game::HeadsUpCamera& camera)
  : m_camera(camera)
  {

  }

  virtual void render(sf::RenderWindow& window) override {
    sf::Vector2f pos = m_camera.transform({ -74.0f, -74.0f });

    sf::RectangleShape shape({ 64, 64 });
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(0xCC, 0x00, 0x00));
    window.draw(shape);
  }

private:
  game::HeadsUpCamera& m_camera;
};

int main(int argc, char *argv[]) {
  game::Log::setLevel(game::Log::INFO);

  // initialize

  static constexpr unsigned INITIAL_WIDTH = 1024;
  static constexpr unsigned INITIAL_HEIGHT = 576;

  game::WindowSettings settings(INITIAL_WIDTH, INITIAL_HEIGHT, "Game template (version " GAME_VERSION ")");

  sf::RenderWindow window;
  settings.applyTo(window);
  window.setKeyRepeatEnabled(false);

  // load resources
  game::ResourceManager resources;
  resources.addSearchDir(GAME_DATADIR);

  // add cameras
  game::CameraManager cameras;

  game::FixedRatioCamera mainCamera(2.0f, SIZE);
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
  Minimap map(hudCamera);
  hudEntities.addEntity(map);

  // main loop
  game::Clock clock;

  while (window.isOpen()) {
    // input
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Resized) {
        std::printf("Resized: %ux%u\n", event.size.width, event.size.height);
      }

      actions.update(event);
      cameras.update(event);
    }

    if (closeWindowAction.isActive()) {
      window.close();
    }

    if (fullscreenAction.isActive()) {
      settings.toggleFullscreen();
      settings.applyTo(window);
      auto sz = window.getSize();

      // fake resize event (not sent when going fullscreen)
      sf::Event event;
      event.type = sf::Event::Resized;
      event.size.width = sz.x;
      event.size.height = sz.y;
      cameras.update(event);
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
