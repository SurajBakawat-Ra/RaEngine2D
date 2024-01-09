#pragma once
#include "Common.h"
#include "Entity.h"
#include "Vec2.h"


class Physics
{
private:
	Physics() {}

	friend class GameEngine;

	void CheckCollisions(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);


public:
	void update();

	static Vec2 GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
	static Vec2 GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

	static std::shared_ptr<Entity> IsCollidingDown(std::shared_ptr<Entity> a);
	static std::shared_ptr<Entity> IsCollidingUp(std::shared_ptr<Entity> a);
	static std::shared_ptr<Entity> IsCollidingLeft(std::shared_ptr<Entity> a);
	static std::shared_ptr<Entity> IsCollidingRight(std::shared_ptr<Entity> a);
};
