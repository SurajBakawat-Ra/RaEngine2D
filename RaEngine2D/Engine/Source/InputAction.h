#pragma once
#include <string>

enum eInputActionTypes
{
	END = 0,
	START
};

class InputAction
{
	std::string m_name;
	eInputActionTypes m_type;

public:
	InputAction();
	InputAction(std::string name, const eInputActionTypes& type);

	std::string name() const;
	eInputActionTypes type() const;
};
