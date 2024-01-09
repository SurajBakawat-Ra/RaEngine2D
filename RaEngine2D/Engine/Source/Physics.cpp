#include "Physics.h"
#include "Vec2.h"

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

bool Physics::CheckCollisionDown(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    Physics p;

    return p.CheckCollisionSide(a, b, false, true, false, false);
}

bool Physics::CheckCollisionSide(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b, bool up = false, bool down = false, bool left = false, bool right = false)
{
    if (a->id() != b->id() && GetOverlap(a, b).x > 0.f && GetOverlap(a, b).y > 0.f)
    {
        bool xOverlap = false;
        bool yOverlap = false;

        if (GetPreviousOverlap(a, b).x > 0.f)
        {
            yOverlap = true;
        }
        else
        {
            xOverlap = true;
        }

        if (yOverlap)
        {
            auto& transform = a->getComponent<CTransform>();

            if (transform.getPos().y < b->getComponent<CTransform>().getPos().y)
            {
                if (down)
                    return true;
            }

            if (transform.getPos().y > b->getComponent<CTransform>().getPos().y)
            {
                if (up)
                    return true;
            }
        }

        if (xOverlap)
        {
            auto& transform = a->getComponent<CTransform>();

            if (transform.getPos().x < b->getComponent<CTransform>().getPos().x)
            {
                if (right)
                    return true;
            }

            if (transform.getPos().x > b->getComponent<CTransform>().getPos().x)
            {
                if (left)
                    return true;
            }
        }
    }

    return false;
}
