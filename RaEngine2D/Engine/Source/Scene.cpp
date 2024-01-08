#include "Scene.h"

Scene::Scene() {}

void Scene::registerAction(int inputKey, std::string actionName)
{
	m_actionMap[inputKey] = actionName;
}

int Scene::width() { return GameEngine::Instance()->window().getSize().x; }

int Scene::height() { return GameEngine::Instance()->window().getSize().y; }

void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{
	sf::VertexArray line(sf::LinesStrip, 2);

	line[0].position = sf::Vector2f(p1.x, p1.y);
	line[1].position = sf::Vector2f(p2.x, p2.y);

	GameEngine::Instance()->window().draw(line);
}

const ActionMap& Scene::getActionMap() const { return m_actionMap; }

bool Scene::hasEnded() const { return m_hasEnded; }

size_t Scene::currentFrame() const { return m_currentFrame; }

void Scene::setPaused(bool paused) { m_paused = paused; }

void Scene::simulate(const size_t frames) {}
