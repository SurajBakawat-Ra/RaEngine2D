#pragma once

#include "Assets.h"

#include "Common.h"
#include "Scene.h"
#include <memory>


class Scene;

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{
private:
    static GameEngine* m_Instance;

    GameEngine()
    {

    }

protected:
    sf::RenderWindow m_window;
    Assets m_assets;
    std::string m_currentScene;
    SceneMap m_sceneMap;
    size_t m_simulationSpeed = 1;
    bool m_running = true;
    size_t m_currentFrames = 0;

    void init(const std::string& path);
    void update();

    void sUserInput();
    void sRender();

    std::shared_ptr<Scene> currentScene();

public:
    GameEngine(const GameEngine& obj) = delete;

    static GameEngine* Instance();

    void changeScene(std::string scenes,
        const std::shared_ptr<Scene>& scene,
        bool endCurrentScene = false);

    void quit();
    void run();

    sf::RenderWindow& window();
    const Assets& getAssets() const;
    bool isRunning();
    std::shared_ptr<Scene> getScene(std::string);
    EntityManager entityManager;

    static Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);
    void removeAllEntities();
};
