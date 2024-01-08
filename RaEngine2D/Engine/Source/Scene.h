#pragma once

#include "InputAction.h"
#include "EntityManager.h"
#include "GameEngine.h"

#include <memory>

typedef std::map<int, std::string> ActionMap;

class GameEngine;

class Scene
{
protected:
	ActionMap m_actionMap;
	bool m_paused = false;
	bool m_hasEnded = false;
	size_t m_currentFrame = 0;

	virtual void onEnd() = 0;
	void setPaused(bool paused);

public:
	Scene();

	virtual void update() = 0;
	virtual void sDoAction(const InputAction& action) = 0;
	virtual void sRender() = 0;

	void simulate(const size_t frames);
	void registerAction(int inputKey, std::string actionName);

	int width();
	int height();
	size_t currentFrame() const;

	bool hasEnded() const;
	const ActionMap& getActionMap() const;
	void drawLine(const Vec2& p1, const Vec2& p2);
};
