#include "Transform.h"
#include "Physics.h"
#include "GameEngine.h"

void Transform::update()
{
    for (auto& e : GameEngine::Instance()->entityManager.getEntities())
    {
        if (e->hasComponent<CRigidbody>())
        {
            auto& rigidbody = e->getComponent<CRigidbody>();
            rigidbody.rigidbody.update();
        }
    }


    for (auto& e : GameEngine::Instance()->entityManager.getEntities())
    {
        auto& transform = e->getComponent<CTransform>();
        if (e->hasComponent<CAnimation>())
        {
            auto& animation = e->getComponent<CAnimation>().animation;

            if (transform.velocity != Vec2(0.f, 0.f))
            {
                transform.SetPosition(Vec2(transform.getPos().x + transform.velocity.x, transform.getPos().y + transform.velocity.y));
            }

            animation.getSprite().setRotation(transform.angle);
            animation.getSprite().setPosition(transform.getPos().x, transform.getPos().y);
            animation.getSprite().setScale(transform.scale.x, transform.scale.y);
        }
    }

    for (auto& e : GameEngine::Instance()->entityManager.getEntities())
    {
        if (!e->hasComponent<CRigidbody>())
            continue;

        auto& transform = e->getComponent<CTransform>();

        if (Physics::IsCollidingLeft(e) || Physics::IsCollidingRight(e))
        {
            transform.velocity.x = 0.f;
            transform.SetPosition(Vec2(transform.getPos().x + Physics::GetOverlap(e, std::get<0>(e->collisionInfo)).x, transform.getPos().y));
        }
        if (Physics::IsCollidingDown(e) || Physics::IsCollidingUp(e))
        {
            transform.velocity.y = 0.f;
            transform.SetPosition(Vec2(transform.getPos().x, transform.getPos().y - Physics::GetOverlap(e, std::get<0>(e->collisionInfo)).y));
        }
    }
}

