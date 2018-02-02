#include "Application/Input.h"

using namespace FireCube;

KeyModifier FireCube::operator | (const KeyModifier &lhs, const KeyModifier &rhs)
{
	KeyModifier ret = static_cast<KeyModifier>(static_cast<int>(lhs) | static_cast<int>(rhs));
	return ret;
}

KeyModifier FireCube::operator & (const KeyModifier &lhs, const KeyModifier &rhs)
{
	KeyModifier ret = static_cast<KeyModifier>(static_cast<int>(lhs) & static_cast<int>(rhs));
	return ret;
}

bool MappedInput::IsStateOn(const std::string &name) const
{
	return states.find(name) != states.end();
}

bool MappedInput::IsActionTriggered(const std::string &name) const
{
	return actions.find(name) != actions.end();
}

float MappedInput::GetValue(const std::string &name) const
{
	std::map<std::string, float>::const_iterator i = values.find(name);
	if (i == values.end())
		return 0.0f;

	return i->second;
}

bool MappedInput::HasValue(const std::string &name) const
{
	std::map<std::string, float>::const_iterator i = values.find(name);
	return i != values.end();
}

void InputManager::AddMapping(Key key, InputMappingType inputMappingType, const std::string &actionName, KeyModifier modifier)
{
	// Add a mapping to the list of mappings of the given key
	InputMapping mapping;
	mapping.inputMappingType = inputMappingType;
	mapping.actionName = actionName;
	mapping.modifier = modifier;
	mappedKeys[key].push_back(mapping);
}

void InputManager::RemoveMapping(Key key, InputMappingType inputMappingType, const std::string &actionName, KeyModifier modifier)
{
	std::map<Key, std::vector<InputMapping>>::iterator i = mappedKeys.find(key);
	if (i == mappedKeys.end())
		return;

	// Search for this specific mapping in the list of mappings of the given key and remove it if found
	std::vector<InputMapping>::iterator j = i->second.begin();
	for (; j != i->second.end(); j++)
	{
		if (j->inputMappingType == inputMappingType && j->actionName == actionName && j->modifier == modifier)
		{
			i->second.erase(j);
			break;
		}
	}
}

void InputManager::AddMapping(AnalogInput analogInput, const std::string &name)
{
	mappedAnalogs[analogInput].push_back(name);
}

void InputManager::RemoveMapping(AnalogInput analogInput, const std::string &name)
{
	std::map<AnalogInput, std::vector<std::string>>::iterator i = mappedAnalogs.find(analogInput);
	if (i == mappedAnalogs.end())
		return;

	// Search for the mapping name in the list of mappings for the given analog input and remove it if found
	std::vector<std::string>::iterator j = i->second.begin();
	for (; j != i->second.end(); j++)
	{
		if (*j == name)
		{
			i->second.erase(j);
			break;
		}
	}
}

void InputManager::SetRawKeyState(Key key, bool pressed, bool previouslyPressed, KeyModifier modifier)
{
	if (key == Key::LEFT_CTRL || key == Key::RIGHT_CTRL || key == Key::LEFT_ALT || key == Key::RIGHT_ALT || key == Key::LEFT_SHIFT || key == Key::RIGHT_SHIFT)
	{
		KeyModifier modifier = KeyModifier::NONE;							
		if (key == Key::LEFT_CTRL)
			modifier = KeyModifier::LEFT_CTRL;
		else if (key == Key::RIGHT_CTRL)
			modifier = KeyModifier::RIGHT_CTRL;
		else if (key == Key::LEFT_ALT)
			modifier = KeyModifier::LEFT_ALT;
		else if (key == Key::RIGHT_ALT)
			modifier = KeyModifier::RIGHT_ALT;
		else if (key == Key::LEFT_SHIFT)
			modifier = KeyModifier::LEFT_SHIFT;
		else if (key == Key::RIGHT_SHIFT)
			modifier = KeyModifier::RIGHT_SHIFT;

		// Iterate over all pressed keys
		for (auto i = pressedKeys.begin(); i != pressedKeys.end(); ++i)
		{
			std::map<Key, std::vector<InputMapping>>::iterator inputMapping = mappedKeys.find(*i);			
			if (inputMapping != mappedKeys.end())
			{
				for (unsigned int j = 0; j < inputMapping->second.size(); j++)
				{
					InputMapping &keyMapping = inputMapping->second[j];
					// If a modifier key which is required to an active state is released remove this state
					if (!pressed)
					{			
						if (keyMapping.inputMappingType == InputMappingType::STATE && ((keyMapping.modifier == KeyModifier::NONE && modifier != KeyModifier::NONE) || ((keyMapping.modifier & modifier) != KeyModifier::NONE)))
							mappedInput.states.erase(keyMapping.actionName);
					}
					else
					{
						// If a modifier key which is required to an active state is pressed while the main key is already down, add this state
						if (keyMapping.inputMappingType == InputMappingType::STATE && (keyMapping.modifier != KeyModifier::NONE) && (keyMapping.modifier != KeyModifier::ANY) && ((keyMapping.modifier & modifier) != KeyModifier::NONE))
							mappedInput.states.insert(keyMapping.actionName);
					}
				}
			}
		}
	}
	
	if (key != Key::NO_KEY)
	{
		if (!pressed && previouslyPressed)
		{
			releasedKeys.push_back(key);
		}

		if (pressed && !previouslyPressed)
		{
			pressedKeys.push_back(key);
		}
		else if (!pressed && previouslyPressed)
		{
			std::vector<Key>::iterator i = find(pressedKeys.begin(), pressedKeys.end(), key);
			if (i != pressedKeys.end())
			{
				pressedKeys.erase(i);
			}
		}
	}

	std::map<Key, std::vector<InputMapping>>::iterator i = mappedKeys.find(key);
	if (i == mappedKeys.end())
	{
		return;
	}
	
	for (unsigned int j = 0; j < i->second.size(); j++)
	{
		InputMapping &keyMapping = i->second[j];
		if (keyMapping.inputMappingType == InputMappingType::ACTION && ((keyMapping.modifier == KeyModifier::ANY) || (keyMapping.modifier == KeyModifier::NONE && modifier == KeyModifier::NONE) || ((keyMapping.modifier & modifier) != KeyModifier::NONE)) && !pressed && previouslyPressed)
			mappedInput.actions.insert(keyMapping.actionName);
		else if (keyMapping.inputMappingType == InputMappingType::STATE && ((keyMapping.modifier == KeyModifier::ANY) || (keyMapping.modifier == KeyModifier::NONE && modifier == KeyModifier::NONE) || ((keyMapping.modifier & modifier) != KeyModifier::NONE)) && pressed)
			mappedInput.states.insert(keyMapping.actionName);
		else if (keyMapping.inputMappingType == InputMappingType::STATE && !pressed)
			mappedInput.states.erase(keyMapping.actionName);
	}	
}

void InputManager::SetRawAnalogValue(AnalogInput analogInput, float value)
{
	std::map<AnalogInput, std::vector<std::string>>::iterator i = mappedAnalogs.find(analogInput);
	if (i == mappedAnalogs.end())
		return;

	for (unsigned int j = 0; j < i->second.size(); j++)
	{
		mappedInput.values[i->second[j]] = value;
	}	
}

const MappedInput &InputManager::GetMappedInput() const
{
	return mappedInput;
}

void InputManager::ResetInputState()
{
	mappedInput.actions.clear();
	mappedInput.values.clear();
	releasedKeys.clear();
	
	// Trigger no key input (mainly used to detect mouse movement)
	SetRawKeyState(Key::NO_KEY, true, false);
}

bool InputManager::IsKeyPressed(Key key, KeyModifier modifier) const
{
	bool ret = std::find(releasedKeys.begin(), releasedKeys.end(), key) != releasedKeys.end();
	return ret && (modifier == KeyModifier::ANY ||
			((modifier & KeyModifier::LEFT_ALT) != KeyModifier::NONE) && (std::find(pressedKeys.begin(), pressedKeys.end(), Key::LEFT_ALT) != pressedKeys.end()) ||
			((modifier & KeyModifier::RIGHT_ALT) != KeyModifier::NONE) && (std::find(pressedKeys.begin(), pressedKeys.end(), Key::RIGHT_ALT) != pressedKeys.end()) ||
			((modifier & KeyModifier::LEFT_CTRL) != KeyModifier::NONE) && (std::find(pressedKeys.begin(), pressedKeys.end(), Key::LEFT_CTRL) != pressedKeys.end()) ||
			((modifier & KeyModifier::RIGHT_CTRL) != KeyModifier::NONE) && (std::find(pressedKeys.begin(), pressedKeys.end(), Key::RIGHT_CTRL) != pressedKeys.end()) ||
			((modifier & KeyModifier::LEFT_SHIFT) != KeyModifier::NONE) && (std::find(pressedKeys.begin(), pressedKeys.end(), Key::LEFT_SHIFT) != pressedKeys.end()) ||
			((modifier & KeyModifier::RIGHT_SHIFT) != KeyModifier::NONE) && (std::find(pressedKeys.begin(), pressedKeys.end(), Key::RIGHT_SHIFT) != pressedKeys.end()));
}
