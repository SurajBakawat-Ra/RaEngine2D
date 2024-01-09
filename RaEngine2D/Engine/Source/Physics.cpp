#include "Physics.h"
#include "Vec2.h"
#include "GameEngine.h"
#include <cmath>

void Physics::update()
{
    for (auto& e : GameEngine::Instance()->entityManager.getEntities())
    {
        for (auto& e2 : GameEngine::Instance()->entityManager.getEntities())
        {
            if(e->id() != e2->id())
                CheckCollisions(e, e2);
        }
    }
}

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	// TODO: return the overlap rectangle size of the bounding boxes of entity a
	// and b
	Vec2 aPos = { a->getComponent<CTransform>().getPos() };
	Vec2 bPos = { b->getComponent<CTransform>().getPos() };
	Vec2 aSize = a->getComponent<CBoundingBox>().size;
	Vec2 bSize = b->getComponent<CBoundingBox>().size;

	Vec2 delta = { abs(aPos.x - bPos.x), abs(aPos.y - bPos.y) };
	float overlapX = (aSize.x / 2 + bSize.x / 2) - delta.x;
	float overlapY = (aSize.y / 2 + bSize.y / 2) - delta.y;

	return { overlapX, overlapY };
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	// TODO: return the previous overlap rectangle size of the bounding boxes of
	// entity a and b
	//       previous overlap the entity's previous position
	Vec2 aPos = { a->getComponent<CTransform>().getPrevPos() };
	Vec2 bPos = { b->getComponent<CTransform>().getPrevPos() };
	Vec2 aSize = a->getComponent<CBoundingBox>().size;
	Vec2 bSize = b->getComponent<CBoundingBox>().size;

	Vec2 delta = { abs(aPos.x - bPos.x), abs(aPos.y - bPos.y) };
	float overlapX = (aSize.x / 2 + bSize.x / 2) - delta.x;
	float overlapY = (aSize.y / 2 + bSize.y / 2) - delta.y;

	return { overlapX, overlapY };
}

std::shared_ptr<Entity> Physics::IsCollidingDown(std::shared_ptr<Entity> a)
{
    if (std::get<0>(a->collisionInfo) != nullptr)
    {
		if (std::get<2>(a->collisionInfo))
			return std::get<0>(a->collisionInfo);
    }
    return nullptr;
}

std::shared_ptr<Entity> Physics::IsCollidingUp(std::shared_ptr<Entity> a)
{
    if (std::get<0>(a->collisionInfo) != nullptr)
    {
        if (std::get<1>(a->collisionInfo))
            return std::get<0>(a->collisionInfo);
    }
    return nullptr;
}

std::shared_ptr<Entity> Physics::IsCollidingLeft(std::shared_ptr<Entity> a)
{
    if (std::get<0>(a->collisionInfo) != nullptr)
    {
        if (std::get<3>(a->collisionInfo))
            return std::get<0>(a->collisionInfo);
    }
    return nullptr;
}

std::shared_ptr<Entity> Physics::IsCollidingRight(std::shared_ptr<Entity> a)
{
    if (std::get<0>(a->collisionInfo) != nullptr)
    {
        if (std::get<4>(a->collisionInfo))
            return std::get<0>(a->collisionInfo);
    }
    return nullptr;
}

void Physics::CheckCollisions(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    if (a->id() != b->id() && GetOverlap(a, b).x > 0.f && GetOverlap(a, b).y > 0.f)
    {
        bool xOverlap = false;
        bool yOverlap = false;

        if (GetPreviousOverlap(a, b).x > 0.f)
        {
            yOverlap = true;
        }
        if (GetPreviousOverlap(a, b).y > 0.f)
        {
            xOverlap = true;
        }

        bool up = false;
        bool down = false;
        bool left = false;
        bool right = false;

        if (yOverlap)
        {
            auto& transform = a->getComponent<CTransform>();

            if (transform.getPos().y < b->getComponent<CTransform>().getPos().y)
            {
                if (a->tag() == "PLAYER")
                std::cout << "down" << std::endl;
                down = true;
            }

            if (transform.getPos().y > b->getComponent<CTransform>().getPos().y)
            {
                if(a->tag() == "PLAYER")
                std::cout << "up" << std::endl;
                up = true;
            }
        }

        if (xOverlap)
        {
            auto& transform = a->getComponent<CTransform>();

            if (transform.getPos().x < b->getComponent<CTransform>().getPos().x)
            {
                if (a->tag() == "PLAYER")
                std::cout << "right" << std::endl;
                right = true;
            }

            if (transform.getPos().x > b->getComponent<CTransform>().getPos().x)
            {
                if (a->tag() == "PLAYER")
				std::cout << "left" << std::endl; 
                left = true;
            }
        }

        a->collisionInfo = std::make_tuple<>(b, up, down, left, right);
    }
}

