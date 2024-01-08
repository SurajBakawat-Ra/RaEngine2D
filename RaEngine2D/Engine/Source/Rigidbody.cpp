#include "Rigidbody.h"
#include "GameEngine.h"

void Rigidbody::update()
{
    ProcessGravity();
}

void Rigidbody::ProcessGravity()
{
    for (auto& e : GameEngine::Instance()->entityManager.getEntities())
    {
        if (e->hasComponent<CRigidbody>())
        {
            auto& rigidbody = e->getComponent<CRigidbody>();
            auto& transform = e->getComponent<CTransform>();

			transform.velocity.y += rigidbody.gravity;
            

			if (transform.velocity.y > rigidbody.maxVelocity)
			{
				transform.velocity.y = rigidbody.maxVelocity;
			}
        }
    }
}
