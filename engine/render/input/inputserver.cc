//------------------------------------------------------------------------------
//  @file inputserver.cc
//  @copyright (C) 2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "inputserver.h"
#include "GLFW/glfw3.h"
#include <iostream>

namespace Input
{

struct HIDState
{
	Keyboard keyboard;
	Mouse mouse;
	std::vector<Gamepad*> gamepads;
};

static HIDState* hid = nullptr;

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::Create()
{
	if (hid == nullptr)
		hid = new HIDState();

	// Register gamepads
	if (glfwJoystickPresent(GLFW_JOYSTICK_1))
		ConnectJoystick(GLFW_JOYSTICK_1);

	glfwSetJoystickCallback(JoystickCallback);
}

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::BeginFrame()
{
	for (int i = 0; i < Key::Code::NumKeyCodes; i++)
	{
		if (hid->keyboard.released[i])
			hid->keyboard.held[i] = false;
		
		hid->keyboard.pressed[i] = false;
		hid->keyboard.released[i] = false;
	}

	for (int i = 0; i < Mouse::Button::NumMouseButtons; i++)
	{
		if (hid->mouse.released[i])
			hid->mouse.held[i] = false;

		hid->mouse.pressed[i] = false;
		hid->mouse.released[i] = false;
	}

	hid->mouse.delta = glm::vec2(0);
	hid->mouse.previousPosition = hid->mouse.position;

	// Gamepads
	for (Gamepad*& gamepad : hid->gamepads)
	{
		gamepad->HandleInput();
	}
}

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::HandleKeyEvent(int32 key, int32 scancode, int32 action, int32 mods)
{
	if (action == GLFW_PRESS)
	{
		Key::Code code = Key::FromGLFW(key);
		hid->keyboard.pressed[code] = true;
		hid->keyboard.held[code] = true;

		// Special case buttons:
		if (code == Key::Code::LeftControl || code == Key::Code::RightControl)
		{
			hid->keyboard.pressed[Key::Code::Control] = true;
			hid->keyboard.held[Key::Code::Control] = true;
		}
		else if (code == Key::Code::LeftShift || code == Key::Code::RightShift)
		{
			hid->keyboard.pressed[Key::Code::Shift] = true;
			hid->keyboard.held[Key::Code::Shift] = true;
		}
		else if (code == Key::Code::LeftAlt || code == Key::Code::RightAlt)
		{
			hid->keyboard.pressed[Key::Code::Alt] = true;
			hid->keyboard.held[Key::Code::Alt] = true;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		Key::Code code = Key::FromGLFW(key);
		hid->keyboard.released[code] = true;

		// Special case buttons:
		if (code == Key::Code::LeftControl || code == Key::Code::RightControl)
		{
			hid->keyboard.released[Key::Code::Control] = true;
		}
		else if (code == Key::Code::LeftShift || code == Key::Code::RightShift)
		{
			hid->keyboard.released[Key::Code::Shift] = true;
		}
		else if (code == Key::Code::LeftAlt || code == Key::Code::RightAlt)
		{
			hid->keyboard.released[Key::Code::Alt] = true;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::HandleMousePressEvent(int32 button, int32 action, int32 mods)
{
	assert(button < Mouse::Button::NumMouseButtons);
	if (action == GLFW_PRESS)
	{
		hid->mouse.pressed[button] = true;
		hid->mouse.held[button] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		hid->mouse.released[button] = true;
	}
}

//------------------------------------------------------------------------------
/**
*/
void
InputHandler::HandleMouseMoveEvent(double x, double y)
{
	hid->mouse.position = glm::vec2(x, y);
	hid->mouse.delta = hid->mouse.position - hid->mouse.previousPosition;
}

void InputHandler::JoystickCallback(int jid, int event)
{
	std::cout << "Gamepad " << jid << " - event: " << event << "\n";

	switch (event)
	{
	case GLFW_CONNECTED: ConnectJoystick(jid); break;
	case GLFW_DISCONNECTED: DisconnectJoystick(jid); break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void InputHandler::ConnectJoystick(int jid)
{
	Gamepad* gamepad = new Gamepad();
	gamepad->id = jid;

	hid->gamepads.push_back(gamepad);
}

//------------------------------------------------------------------------------
/**
*/
void InputHandler::DisconnectJoystick(int jid)
{
	Gamepad* toErase = nullptr;

	for (Gamepad*& gamepad : hid->gamepads)
	{
		if (gamepad->id == jid)
		{
			toErase = gamepad;
			break;
		}
	}

	if (!toErase)
		return;

	hid->gamepads.erase(std::find(hid->gamepads.begin(), hid->gamepads.end(), toErase));
}

//------------------------------------------------------------------------------
/**
*/
Keyboard*
GetDefaultKeyboard()
{
	assert(hid != nullptr);
	return &hid->keyboard;
}

//------------------------------------------------------------------------------
/**
*/
Mouse*
GetDefaultMouse()
{
	assert(hid != nullptr);
	return &hid->mouse;
}

//------------------------------------------------------------------------------
/**
*/
Gamepad*
GetGamepad(int id)
{
	assert(hid != nullptr);
	assert(id < hid->gamepads.size());
	return hid->gamepads[id];
}

} // namespace Input
