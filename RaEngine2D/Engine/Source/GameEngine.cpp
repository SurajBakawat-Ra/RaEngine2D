#include "GameEngine.h"
#include "Assets.h"
#include "Scene_Menu.h"
#include "Scene_Play.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowHandle.hpp>
#include <iostream>

GameEngine* GameEngine::m_Instance = nullptr;

void GameEngine::update() 
{
    sUserInput();
    m_sceneMap.at(m_currentScene)->update();
    m_currentFrames++;
}

void GameEngine::init(const std::string &path) {
  m_assets.loadFromFile(path);
  m_window.create(sf::VideoMode(1280, 770), "Definitely Not Mario");
  //, sf::Style::Fullscreen);
  m_window.setFramerateLimit(60);
  // m_window.setPosition(sf::Vector2i(350, 200));
  changeScene("MENU", std::make_shared<Scene_Menu>());
}

std::shared_ptr<Scene> GameEngine::currentScene() {
  return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning() { return m_running && window().isOpen(); }

sf::RenderWindow &GameEngine::window() { return m_window; }

void GameEngine::run() {
  while (isRunning())
    update();
}

void GameEngine::sUserInput() 
{
  sf::Event event{};
  while (m_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      quit();
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::X) {
        sf::Texture texture;
        texture.create(m_window.getSize().x, m_window.getSize().y);
        texture.update(m_window);
        if (texture.copyToImage().saveToFile("bin/screenshots/test.png"))
          std::cout << "screenshot saved to "
                    << "bin/screenshots/test.png" << std::endl;
      }
    }
    if (event.type == sf::Event::KeyPressed ||
        event.type == sf::Event::KeyReleased) {
      // if the current scene does not have an action associated with this key,
      // skip the event
      if (currentScene()->getActionMap().find(event.key.code) ==
          currentScene()->getActionMap().end())
        continue;
      // determinate start or end action whether it was pres or release
      eInputActionTypes actionType = (event.type == sf::Event::KeyPressed)
                                    ? eInputActionTypes::START
                                    : eInputActionTypes::END;
      // look up the action and send the action to the scene
      currentScene()->sDoAction(InputAction(
          currentScene()->getActionMap().at(event.key.code), actionType));
    }
  }
}


GameEngine* GameEngine::Instance()
{
    if (m_Instance == nullptr)
    {
        m_Instance = new GameEngine();

        m_Instance->init("bin/assets.txt");

        return m_Instance;
    }
    else
    {
        return m_Instance;
    }
}

void GameEngine::changeScene(std::string scenes,
                             const std::shared_ptr<Scene> &scene,
                             bool endCurrentScene) {
  // review this one
  m_currentScene = scenes;
  if (scene)
    m_sceneMap[scenes] = scene;
}

const Assets &GameEngine::getAssets() const { return m_assets; }

void GameEngine::quit() { m_running = false; }

std::shared_ptr<Scene> GameEngine::getScene(std::string scenes) {
  return m_sceneMap[scenes];
}

Vec2 GameEngine::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    //      TODO: This function takes in a grid (x,y) position and an Entity
    //            Return a Vec2 indicating where the CENTER position of the Entity
    //            should be You must use the Entity's Animation size to position
    //            it correctly The size of the grid width and height is stored in
    //            m_gridSize.x and m_gridSize.y The up-left corner of the
    //            Animation should align with the bottom left of the grid cell
    float x, y;
    x = gridX + entity->getComponent<CAnimation>().animation.getSize().x / 2;
    y = gridY - entity->getComponent<CAnimation>().animation.getSize().y / 2;
    return Vec2(x, y);
}

void GameEngine::removeAllEntities()
{
    for (auto& e : GameEngine::Instance()->entityManager.getEntities())
    {
        e->destroy();
    }
}
