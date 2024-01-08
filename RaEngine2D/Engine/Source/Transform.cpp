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

    Physics physics;

    for (auto& e : GameEngine::Instance()->entityManager.getEntities())
    {
        for (auto& e2 : GameEngine::Instance()->entityManager.getEntities())
        {
            if (e->id() != e2->id() && (physics.GetOverlap(e, e2).x > 0.f && physics.GetOverlap(e, e2).y > 0.f))
            {
                //std::string s = std::to_string(e->id()) + " " + std::to_string(e2->id()) + " " + std::to_string(e->getComponent<CTransform>().getPos().x) + " " + std::to_string(e->getComponent<CTransform>().getPos().y)
    //                + " " + std::to_string(e2->getComponent<CTransform>().getPos().x) + " " + std::to_string(e2->getComponent<CTransform>().getPos().y) + " " +
    //                std::to_string(physics.GetOverlap(e, e2).x) + " " + std::to_string(physics.GetOverlap(e, e2).y);
                //std::cout << s;

                bool xOverlap = false;
                bool yOverlap = false;

                if (physics.GetPreviousOverlap(e, e2).x > 0.f)
                {
                    yOverlap = true;
                }
                else
                {
                    xOverlap = true;
                }

                if (yOverlap)
                {
                    auto& transform = e->getComponent<CTransform>();
                    transform.SetPosition(transform.getPrevPos());
                    transform.velocity.y = 0.f;

                }

                if (xOverlap)
                {
                    auto& transform = e->getComponent<CTransform>();
                    transform.SetPosition(transform.getPrevPos());
                    transform.velocity.x = 0.f;
                }
            }
        }
    }

    if (true)//gravity
    {
        for (auto& e : GameEngine::Instance()->entityManager.getEntities())
        {
            auto& transform = e->getComponent<CTransform>();
            if (e->hasComponent<CAnimation>()) {
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
    }
}
