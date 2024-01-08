#include "Entity.h"

Entity::Entity(size_t id, std::string& entityTag) : m_tag(entityTag), m_id(id) {}
Entity::Entity() = default;
bool Entity::isActive() const
{
	return m_active;
}

void Entity::destroy()
{
	m_active = false;
}

size_t Entity::id() const
{
	return m_id;
}

std::string Entity::tag() const
{
	return m_tag;
}
