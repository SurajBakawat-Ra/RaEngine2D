#pragma once

#include "Common.h"
#include "Scene.h"
#include "Scene_Play.h"
#include <deque>
#include <map>
#include <memory>

class Scene_Play : public Scene
{

public:
	std::string m_title;
	std::vector<std::string> m_menuStrings;
	std::vector<std::string> m_levelPaths;
	sf::Text m_menuText;
	size_t m_selectedMenuIndex = 0;

	std::shared_ptr<Entity> m_player;
	std::shared_ptr<Entity> m_playerFeet;

	void init();
	void update() override;
	void sUserInput();
	void sRender() override;
	void onEnd() override;
	void sDoAction(const InputAction& action) override;
	void sMovement();
	bool isPlayerGrounded();

public:
	explicit Scene_Play();
};
