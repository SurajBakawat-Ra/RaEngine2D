#pragma once
#include "Common.h"
#include "Entity.h"
#include "Vec2.h"


class Physics
{
private:
	Physics() {}

	friend class Transform;

public:
	Vec2 GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	Vec2 GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};
