#pragma once

class Render
{
private:
	Render() {}
	friend class GameEngine;

public:
	void update();
};