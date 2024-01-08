#pragma once


class Transform
{
private:
	Transform() {}

	friend class GameEngine;

public:
	void update();

};