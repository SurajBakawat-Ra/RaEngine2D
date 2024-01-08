#pragma once

#include "Common.h"
#include "Entity.h"
typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager
{
private:

	EntityVec m_entities;
	EntityVec m_entitiesToAdd;
	std::map<std::string, EntityVec> m_entityMap;
	size_t    m_totalEntities{ 0 };

	// helper function to avoid repeated code
	void removeDeadEntities(EntityVec& vec);

	EntityManager();

	friend class GameEngine;

public:


	void update();

	std::shared_ptr<Entity> addEntity(std::string entities);

	EntityVec& getEntities();
	EntityVec& getEntities(std::string type);
};
