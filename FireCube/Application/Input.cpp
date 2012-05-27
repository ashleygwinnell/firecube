#include <string>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

#include "Utils/utils.h"
#include "Application/Input.h"
using namespace FireCube;

bool MappedInput::IsStateOn(const string &name) const
{
	return states.find(name) != states.end();
}

bool MappedInput::IsActionTriggered(const string &name) const
{
	return actions.find(name) != actions.end();
}

float MappedInput::GetValue(const string &name) const
{
	map<string, float>::const_iterator i = values.find(name);
	if (i == values.end())
		return 0.0f;

	return i->second;
}

bool MappedInput::HasValue(const std::string &name) const
{
	map<string, float>::const_iterator i = values.find(name);
	return i != values.end();
}

void InputManager::AddInputListener(InputListener *inputListener)
{
	inputListeners.push_back(inputListener);
}

void InputManager::RemoveInputListener(InputListener *inputListener)
{
	vector<InputListener *>::iterator i = std::find(inputListeners.begin(), inputListeners.end(), inputListener);
	if (i != inputListeners.end())
		inputListeners.erase(i);
}

void InputManager::DispatchInput(float time)
{
	for (unsigned int i = 0; i < inputListeners.size(); i++)
		inputListeners[i]->HandleInput(time, mappedInput);

	mappedInput.actions.clear();
	mappedInput.values.clear();
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
	map<Key, vector<InputMapping>>::iterator i = mappedKeys.find(key);
	if (i == mappedKeys.end())
		return;

	// Search for this specific mapping in the list of mappings of the given key and remove it if found
	vector<InputMapping>::iterator j = i->second.begin();
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
	map<AnalogInput, vector<string>>::iterator i = mappedAnalogs.find(analogInput);
	if (i == mappedAnalogs.end())
		return;

	// Search for the mapping name in the list of mappings for the given analog input and remove it if found
	vector<string>::iterator j = i->second.begin();
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
	if (key == KEY_LEFT_CTRL || key == KEY_RIGHT_CTRL || key == KEY_LEFT_ALT || key == KEY_RIGHT_ALT || key == KEY_LEFT_SHIFT || key == KEY_RIGHT_SHIFT)
	{
		KeyModifier modifier = MODIFIER_NONE;							
		if (key == KEY_LEFT_CTRL)
			modifier = MODIFIER_LEFT_CTRL;
		else if (key == KEY_RIGHT_CTRL)
			modifier = MODIFIER_RIGHT_CTRL;
		else if (key == KEY_LEFT_ALT)
			modifier = MODIFIER_LEFT_ALT;
		else if (key == KEY_RIGHT_ALT)
			modifier = MODIFIER_RIGHT_ALT;
		else if (key == KEY_LEFT_SHIFT)
			modifier = MODIFIER_LEFT_SHIFT;
		else if (key == KEY_RIGHT_SHIFT)
			modifier = MODIFIER_RIGHT_SHIFT;

		// Iterate over all pressed keys
		for (auto i = pressedKeys.begin(); i != pressedKeys.end(); ++i)
		{
			map<Key, vector<InputMapping>>::iterator inputMapping = mappedKeys.find(*i);			
			if (inputMapping != mappedKeys.end())
			{
				for (unsigned int j = 0; j < inputMapping->second.size(); j++)
				{
					InputMapping &keyMapping = inputMapping->second[j];
					// If a modifier key which is required to an active state is released remove this state
					if (!pressed)
					{			
						if (keyMapping.inputMappingType == STATE && ((keyMapping.modifier == MODIFIER_NONE && modifier != MODIFIER_NONE) || ((keyMapping.modifier & modifier) != 0)))
							mappedInput.states.erase(keyMapping.actionName);
					}
					else
					{
						// If a modifier key which is required to an active state is pressed while the main key is already down, add this state
						if (keyMapping.inputMappingType == STATE && (keyMapping.modifier != MODIFIER_NONE) && (keyMapping.modifier != MODIFIER_ANY) && ((keyMapping.modifier & modifier) != 0))
							mappedInput.states.insert(keyMapping.actionName);
					}
				}
			}
		}
	}

	if (pressed && !previouslyPressed)
		pressedKeys.push_back(key);
	else if (!pressed && previouslyPressed)
	{
		vector<Key>::iterator i = find(pressedKeys.begin(), pressedKeys.end(), key);
		if (i != pressedKeys.end())
			pressedKeys.erase(i);
	}
		

	map<Key, vector<InputMapping>>::iterator i = mappedKeys.find(key);
	if (i == mappedKeys.end())
		return;
	
	for (unsigned int j = 0; j < i->second.size(); j++)
	{
		InputMapping &keyMapping = i->second[j];
		if (keyMapping.inputMappingType == ACTION && ((keyMapping.modifier == MODIFIER_ANY) || (keyMapping.modifier == MODIFIER_NONE && modifier == MODIFIER_NONE) || ((keyMapping.modifier & modifier) != 0))  && !pressed && previouslyPressed)
			mappedInput.actions.insert(keyMapping.actionName);
		else if (keyMapping.inputMappingType == STATE && ((keyMapping.modifier == MODIFIER_ANY) || (keyMapping.modifier == MODIFIER_NONE && modifier == MODIFIER_NONE) || ((keyMapping.modifier & modifier) != 0)) && pressed)
			mappedInput.states.insert(keyMapping.actionName);
		else if (keyMapping.inputMappingType == STATE && !pressed)
			mappedInput.states.erase(keyMapping.actionName);
	}	
}

void InputManager::SetRawAnalogValue(AnalogInput analogInput, float value)
{
	map<AnalogInput, vector<string>>::iterator i = mappedAnalogs.find(analogInput);
	if (i == mappedAnalogs.end())
		return;

	for (unsigned int j = 0; j < i->second.size(); j++)
	{
		mappedInput.values[i->second[j]] = value;
	}	
}

