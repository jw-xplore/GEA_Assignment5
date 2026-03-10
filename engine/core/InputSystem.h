#pragma once
#include <vector>
#include <map>
#include <string>
//#include "render/input/inputserver.h"
#include "render/input/inputserver.h"

//----------------------------------------------
// Input types
//----------------------------------------------

enum EInputDevice
{
	DeviceKeyboard,
	DeviceMouse,
	DeviceGamepad,
};

enum EInputHandlingType
{
	Button,
	Axis,
};

struct InputMappingEntry
{
	std::string name;
	EInputDevice device;
	EInputHandlingType handlingType;
};

struct InputHandlingButton
{
	int button;
};

struct InputHandlingAxis
{
	int positive;
	int negative;
};

class InputEvent
{
public:
	InputMappingEntry inputData;

	virtual bool IsPressed() { return false; }
	virtual float InputAxis() { return 0; }
};

class ButtonInputEvent : public InputEvent
{
public:
	InputHandlingButton input;

	ButtonInputEvent() {}
	ButtonInputEvent(EInputDevice device, int button);

	bool IsPressed() override;
	float InputAxis() override;
};

class AxisInputEvent : public InputEvent
{
public:
	InputHandlingAxis input;

	AxisInputEvent() {}
	AxisInputEvent(EInputDevice device, int positive, int negative);

	bool IsPressed() override;
	float InputAxis() override;
};

//----------------------------------------------
// Input system
//----------------------------------------------
class InputSystem
{
private:
	static InputSystem* instance;

public:
	const const char* filePath = "InputMapping.json";
	std::map<std::string, InputEvent*> actions;

	static InputSystem* GetInstance()
	{
		if (!instance)
			instance = new InputSystem();

		return instance;
	}

	InputSystem();
	~InputSystem();

	void SetDefaultInputMapping();
	void LoadInputMapping();
	void SaveInputMapping();

	void InputMappingUI();
	const char* const* InputMappingUIList(EInputDevice device, int& size);
};