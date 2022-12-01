#include "InputSystem.h"
#include <Windows.h>

InputSystem::InputSystem()
{
	m_set_listeners = {};
}

InputSystem::~InputSystem()
{

}

void InputSystem::update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	if (m_first_time)
	{
		m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);
		m_first_time = false;
	}

	if (current_mouse_pos.x != m_old_mouse_pos.m_x ||
		current_mouse_pos.y != m_old_mouse_pos.m_y)
	{
		// THERE IS MOUSE MOVEMENT
		std::unordered_set<InputListener*>::iterator it =
			m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->onMouseMove(Point(current_mouse_pos.x - m_old_mouse_pos.m_x,
				current_mouse_pos.y - m_old_mouse_pos.m_y));
			++it;
		}
	}
	m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);

	if (::GetKeyboardState(m_key_states))
	{
		for (unsigned int i = 0; i < 255; i++)
		{
			// KEY IS DOWN
			if (m_key_states[i] & 0x80)
			{
				std::unordered_set<InputListener*>::iterator it =
					m_set_listeners.begin();

				while (it != m_set_listeners.end())
				{
					if (i == VK_LBUTTON)
					{
						if (m_key_states[i] != m_old_key_states[i])
							(*it)->onLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else if (i == VK_RBUTTON)
					{
						if (m_key_states[i] != m_old_key_states[i])
							(*it)->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
					}
					else
						(*it)->onKeyDown(i);

					++it;
				}
			}
			else // KEY IS UP
			{
				// If current key state doesn't match old key state, a keyup event occured.
				if (m_key_states[i] != m_old_key_states[i])
				{
					std::unordered_set<InputListener*>::iterator it =
						m_set_listeners.begin();

					while (it != m_set_listeners.end())
					{
						if (i == VK_LBUTTON)
							(*it)->onLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						else if (i == VK_RBUTTON)
							(*it)->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						else
							(*it)->onKeyUp(i);

						++it;
					}
				}
			}
		}
		// Store current key states in old key state array.
		::memcpy(m_old_key_states, m_key_states, sizeof(unsigned char) * 256);
	}
}

void InputSystem::addListener(InputListener* listener)
{
	//m_map_listeners.insert(
	//	std::make_pair<InputListener*, InputListener*>
	//	(std::forward<InputListener*>(listener), std::forward<InputListener*>(listener)));
	m_set_listeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
	//std::map<InputListener*, InputListener*>::iterator it =
	//	m_map_listeners.find(listener);

	//if (it != m_map_listeners.end())
	//{
	//	m_map_listeners.erase(it);
	//}
	m_set_listeners.erase(listener);
}

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}