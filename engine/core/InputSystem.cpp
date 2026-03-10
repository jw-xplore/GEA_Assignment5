#include "config.h"
#include "InputSystem.h"
#include "render/input/inputserver.h"
#include "render/input/keyboard.h"
#include "render/input/mouse.h"
#include "render/input/gamepad.h"
#include "render/input/key.h"
#include "render/input/mouse.h"
#include "render/json.hpp"
#include <fstream>
#include <exception>
#include <string>
#include "imgui.h"

//----------------------------------------------
// Input types
//----------------------------------------------

ButtonInputEvent::ButtonInputEvent(EInputDevice device, int button)
{
	inputData.device = device;
	inputData.handlingType = EInputHandlingType::Button;
	input.button = button;
}

bool ButtonInputEvent::IsPressed()
{
	switch (inputData.device)
	{
	case EInputDevice::DeviceKeyboard: return Input::GetDefaultKeyboard()->held[input.button];
	case EInputDevice::DeviceMouse: return Input::GetDefaultMouse()->held[input.button];
	//case EInputDevice::DeviceGamepad: Input::GetGamepad() ... - NOTE: Not implemented in engine
	}

	return false;
}

float ButtonInputEvent::InputAxis()
{
	switch (inputData.device)
	{
	case EInputDevice::DeviceKeyboard: return Input::GetDefaultKeyboard()->held[input.button];
	case EInputDevice::DeviceMouse: return Input::GetDefaultMouse()->held[input.button];
	}

	return 0;
}

// Axis input
AxisInputEvent::AxisInputEvent(EInputDevice device, int positive, int negative)
{
	inputData.device = device;
	inputData.handlingType = EInputHandlingType::Axis;
	input.positive = positive;
	input.negative = negative;
}

bool AxisInputEvent::IsPressed()
{
	switch (inputData.device)
	{
	case DeviceKeyboard: return Input::GetDefaultKeyboard()->held[input.positive] || Input::GetDefaultKeyboard()->held[input.negative];
	case DeviceMouse: return Input::GetDefaultMouse()->held[input.positive] || Input::GetDefaultMouse()->held[input.negative];
	}

	return true;
}

float AxisInputEvent::InputAxis()
{
	switch (inputData.device)
	{
	case DeviceKeyboard:
	{
		if (Input::GetDefaultKeyboard()->held[input.positive])
			return 1;
		else if (Input::GetDefaultKeyboard()->held[input.negative])
			return -1;
		
		return 0;
	}
	case DeviceMouse:
	{
		if (Input::GetDefaultMouse()->held[input.positive])
			return 1;
		else if (Input::GetDefaultMouse()->held[input.negative])
			return -1;

		return 0;
	}
	// end
	}

	return 0;
}

//----------------------------------------------
// Input system
//----------------------------------------------

InputSystem* InputSystem::instance = new InputSystem();

InputSystem::InputSystem()
{
	LoadInputMapping();
}

InputSystem::~InputSystem()
{

}

void InputSystem::SetDefaultInputMapping()
{
	actions.clear();

	actions = {
		{ "Forward", new ButtonInputEvent(EInputDevice::DeviceKeyboard, Input::Key::W) },
		{ "Boost", new ButtonInputEvent(EInputDevice::DeviceKeyboard, Input::Key::Shift) },
		{ "Yaw", new AxisInputEvent(EInputDevice::DeviceKeyboard, Input::Key::Left, Input::Key::Right) }, // Around Y axis
		{ "Roll", new AxisInputEvent(EInputDevice::DeviceKeyboard, Input::Key::D, Input::Key::A) },
		{ "Pitch", new AxisInputEvent(EInputDevice::DeviceKeyboard, Input::Key::Up, Input::Key::Down) },
		{ "Shoot", new ButtonInputEvent(EInputDevice::DeviceKeyboard, Input::Key::Space) }
	};
}

void InputSystem::LoadInputMapping()
{
	// Read json
	std::ifstream file(this->filePath);
	if (!file.is_open())
	{
		// Fill with default setting
		SetDefaultInputMapping();
		SaveInputMapping();
		return;
	}

	// Parse data
	nlohmann::ordered_json jsonRes = nlohmann::ordered_json::parse(file);
	file.close();

	//tartingPopulation = jsonRes["worldData"]["population"];
	//ironOreAmount = jsonRes["worldData"]["ironOreAmount"];

	actions.clear();

	nlohmann::json inputMapping = jsonRes["inputMapping"];
	for (auto& action : inputMapping.items())
	{
		std::string name = action.value()["name"];

		InputEvent* event;

		// Setup handling
		EInputHandlingType type = action.value()["handlingType"];
		EInputDevice device = action.value()["device"];
		nlohmann::json handling = action.value()["handling"];

		switch (type)
		{
		case EInputHandlingType::Button: event = new ButtonInputEvent(device, handling["button"]); break;
		case EInputHandlingType::Axis: event = new AxisInputEvent(device, handling["positive"], handling["negative"]); break;

		// End
		}

		// Fill data
		event->inputData.name = name;
		actions[name] = event;
	}

}

void InputSystem::SaveInputMapping()
{
	nlohmann::json j;

	// Prepare list
	nlohmann::json list = nlohmann::json::array();

	for (auto const& action : actions)
	{
		InputMappingEntry mapping = action.second->inputData;

		nlohmann::json actionEntry = nlohmann::json::object();
		actionEntry.push_back({ "name", action.first });
		actionEntry.push_back({ "device", mapping.device });
		actionEntry.push_back({ "handlingType", mapping.handlingType });

		// Handling type serialization
		nlohmann::json handling = nlohmann::json::object();

		switch (mapping.handlingType)
		{
		case EInputHandlingType::Button:
		{
			ButtonInputEvent* event = dynamic_cast<ButtonInputEvent*>(action.second);
			handling.push_back({ "button", event->input.button });
			break;
		}

		case EInputHandlingType::Axis:
		{
			AxisInputEvent* event = dynamic_cast<AxisInputEvent*>(action.second);
			handling.push_back({ "positive", event->input.positive });
			handling.push_back({ "negative", event->input.negative });
			break;
		}

		// End
		}

		actionEntry["handling"] = handling;

		// Push to list
		list.push_back(actionEntry);
	}

	j["inputMapping"] = list;

	// Save
	std::ofstream file;
	file.open(filePath);
	file << std::setw(4) << j;
	file.close();
}

void InputSystem::InputMappingUI()
{
	// Keys
	for (auto& action : actions)
	{
		// Device option
		std::string actionDeviceName = action.first + " DEVICE";
		int* device = (int*)(&action.second->inputData.device);
		ImGui::Combo(actionDeviceName.c_str(), device, DeviceNames, EInputDevice::EInputDeviceCount);

		// Display proper handling type
		EInputHandlingType handlingType = action.second->inputData.handlingType;

		switch (handlingType)
		{
		case EInputHandlingType::Button:
		{
			ButtonInputEvent * event = dynamic_cast<ButtonInputEvent*>(action.second);
			int* button = &event->input.button;

			const char* name = action.first.c_str();
			int size = 0;
			const char* const* list = InputMappingUIList(event->inputData.device, size);

			ImGui::Combo(name, button, list, size);
			break;
		}

		case EInputHandlingType::Axis:
		{
			AxisInputEvent* event = dynamic_cast<AxisInputEvent*>(action.second);

			int size = 0;
			const char* const* list = InputMappingUIList(event->inputData.device, size);

			// Positive
			int* pos = &event->input.positive;
			std::string name = action.first + " POSITIVE";
			ImGui::Combo(name.c_str(), pos, list, size);

			// Negative
			int* neg = &event->input.negative;
			name = action.first + " NEGATIVE";
			ImGui::Combo(name.c_str(), neg, list, size);
			break;
		}

		// End
		}
	}

	// Save
	if (ImGui::Button("Save changes"))
	{
		SaveInputMapping();
	}

	// Load
	if (ImGui::Button("Load mapping"))
	{
		LoadInputMapping();
	}

	// Default
	if (ImGui::Button("Default mapping"))
	{
		SetDefaultInputMapping();
	}
}

const char* const* InputSystem::InputMappingUIList(EInputDevice device, int& size)
{
	switch (device)
	{
	case EInputDevice::DeviceKeyboard: size = Input::Key::NumKeyCodes; return Input::Key::CodeNames;
	case EInputDevice::DeviceMouse: size = Input::Mouse::NumMouseButtons; return Input::Mouse::mouseButtonNames;
		// Gamepad
	}

	return nullptr;
}