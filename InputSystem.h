#pragma once
#include "InputListener.h"
#include "Point.h"
#include <unordered_set>

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void setCursorPosition(const Point& pos);
	void showCursor(bool show);

	static InputSystem* get();

private:
	std::unordered_set<InputListener*> m_set_listeners;
	unsigned char m_key_states[256] = { };
	unsigned char m_old_key_states[256] = { };
	Point m_old_mouse_pos;
	bool m_first_time = true;
};
