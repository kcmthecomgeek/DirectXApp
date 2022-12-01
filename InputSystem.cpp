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