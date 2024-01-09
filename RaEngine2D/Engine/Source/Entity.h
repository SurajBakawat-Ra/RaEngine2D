#pragma once

#include "Components.h"
#include <string>
#include <tuple>

class EntityManager;

typedef std::tuple<
    CTransform,
    CLifeSpan,
    CInput,
    CBoundingBox,
    CAnimation,
    CState,
    CRigidbody>
    ComponentsTuple;

typedef std::tuple<std::shared_ptr<Entity>, bool, bool, bool, bool> CollisionInfo;

class Entity
{
    friend class EntityManager;

    bool m_active = true;
    size_t m_id{ 0 };
    std::string m_tag;

    ComponentsTuple m_components = std::make_tuple(CTransform(),
        CLifeSpan(5),
        CInput(),
        CBoundingBox(),
        CAnimation(),
        CState(),
        CRigidbody(Rigidbody(), .1f));

    // constructor is private se we can never create
    // entities outside the EntityManager which had friend access
    Entity(size_t id, std::string& entityTag);
    Entity();

public:
    void destroy();
    size_t id() const;
    bool isActive() const;
    std::string tag() const;
	CollisionInfo collisionInfo = std::make_tuple(nullptr, false, false, false, false);

    template<typename T>
    bool hasComponent() 
    {
        return getComponent<T>().has;
    }

    template<typename T, typename... TArgs>
    T& addComponents(TArgs &&...mArgs) 
    {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template<typename T>
    T& getComponent() { return std::get<T>(m_components); }

    template<typename T>
    void removeComponent() { getComponent<T>() = T(); }
};
