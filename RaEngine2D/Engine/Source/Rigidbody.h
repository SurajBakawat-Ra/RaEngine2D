#pragma once


class Rigidbody
{
public:
	void update();

private:
	void ProcessGravity();
	void ProcessVelocity();
	void ProcessRotation();
};