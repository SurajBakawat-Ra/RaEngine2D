#include "InputAction.h"

InputAction::InputAction() = default;

InputAction::InputAction(std::string name, const eInputActionTypes& type) : m_name(name), m_type(type) {}

std::string InputAction::name() const { return m_name; }

eInputActionTypes InputAction::type() const { return m_type; }
