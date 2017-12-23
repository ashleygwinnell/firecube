#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <set>
#include <vector>
#include <string>
#include <map>

#include "Utils/utils.h"

#undef DELETE

namespace FireCube
{

/**
* The type of a mapping.
* <br>An action happens upon a key press, while a state is on while the key is down.
*/
enum class InputMappingType
{
	ACTION, STATE
};

/**
* List of keys for mapping.
*/
enum class Key
{
	A, B, C, D, E, F, G, H, I,
	J, K, L, M, N, O, P, Q, R,
	S, T, U, V, W, X, Y, Z,
	NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,
	ESCAPE, LEFT, RIGHT, UP, DOWN,
	LEFT_SHIFT, RIGHT_SHIFT, LEFT_CTRL, RIGHT_CTRL, LEFT_ALT, RIGHT_ALT,
	DELETE, TAB, SPACE, RETURN,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	MOUSE_LEFT_BUTTON, MOUSE_MIDDLE_BUTTON, MOUSE_RIGHT_BUTTON,
	NO_KEY
};

/**
* Key modifiers: Shift, Control, Alt.
*/
enum class KeyModifier
{
	NONE = 0,
	LEFT_SHIFT = 1,
	RIGHT_SHIFT = 2,
	SHIFT = 3,
	LEFT_CTRL = 4,
	RIGHT_CTRL = 8,
	CTRL = 12,
	LEFT_ALT = 16,
	RIGHT_ALT = 32,
	ALT = 48,
	ANY = 64
};

KeyModifier operator | (const KeyModifier &lhs, const KeyModifier &rhs);
KeyModifier operator & (const KeyModifier &lhs, const KeyModifier &rhs);

/**
* Analog mappings.
*/
enum class AnalogInput
{
	MOUSE_AXIS_X_RELATIVE, MOUSE_AXIS_Y_RELATIVE, MOUSE_AXIS_X_ABSOLUTE, MOUSE_AXIS_Y_ABSOLUTE, MOUSE_WHEEL_Y_RELATIVE
};

/**
* This class represents the current mapped inputs states.
*/
class FIRECUBE_API MappedInput
{
	friend class InputManager;
public:

	/**
	* Checks whether a given state is on.
	* @param name The name of the state.
	* @return True if the given state is on.
	*/
	bool IsStateOn(const std::string &name) const;

	/**
	* Checks whether a given action was triggered.
	* @param name The name of the action.
	* @return True if the given action was triggered.
	*/
	bool IsActionTriggered(const std::string &name) const;

	/**
	* Returns the value of a mapped analog input.
	* @param name The name of the mapped analog input.
	* @return The value of a mapped analog input.
	*/
	float GetValue(const std::string &name) const;

	/**
	* Checks whether a given analog input has a value.
	* @param name The name of the mapped analog input.
	* @return True if the given analog input currently has a value.
	*/
	bool HasValue(const std::string &name) const;
private:
	std::set<std::string> states;
	std::set<std::string> actions;
	std::map<std::string, float> values;
};

/**
* This class is responsible to handle all input.
*/
class FIRECUBE_API InputManager
{
	friend class Application;
public:	
	/**
	* Adds a key mapping.
	* @param key The key which is associated with this mapping.
	* @param inputMappingType The type of the mapping(action or state).
	* @param actionName the name of the mapping.
	* @param modifier A modifier for the mapping.
	*/
	void AddMapping(Key key, InputMappingType inputMappingType, const std::string &actionName, KeyModifier modifier = KeyModifier::ANY);

	/**
	* Adds an analog mapping.
	* @param analogInput The type of the analog input.
	* @param name The name of the mapping.
	*/
	void AddMapping(AnalogInput analogInput, const std::string &name);

	/**
	* Removes a key mapping.
	* @param key The key which is associated with this mapping.
	* @param inputMappingType The type of the mapping(action or state).
	* @param actionName the name of the mapping.
	* @param modifier A modifier for the mapping.
	*/
	void RemoveMapping(Key key, InputMappingType inputMappingType, const std::string &actionName, KeyModifier modifier = KeyModifier::ANY);

	/**
	* Removes an analog mapping.
	* @param analogInput The type of the analog input.
	* @param name The name of the mapping.
	*/
	void RemoveMapping(AnalogInput analogInput, const std::string &name);			
	
private:
	class InputMapping
	{
	public:
		InputMappingType inputMappingType;
		std::string actionName;
		KeyModifier modifier;
	};

	/**
	* This function is used internally to provide key state from the underlying input system(OS).
	* @param key The given key.
	* @param pressed Specifies whether the key is pressed.
	* @param previouslyPressed Specifies whether the key was previously pressed.
	* @param modifier Specifies the key modifiers.
	*/
	void SetRawKeyState(Key key, bool pressed, bool previouslyPressed, KeyModifier modifier = KeyModifier::ANY);

	/**
	* This function is used internally to provide analog input from the underlying input system(OS).
	* @param analogInput The given analog input.
	* @param value The value of this analog input.
	*/
	void SetRawAnalogValue(AnalogInput analogInput, float value);

	void ResetInputState();
	const MappedInput &GetMappedInput() const;

	std::map<Key, std::vector<InputMapping>> mappedKeys;
	std::map<AnalogInput, std::vector<std::string>> mappedAnalogs;	
	std::vector<Key> pressedKeys;
	MappedInput mappedInput;
};

}

#pragma warning(pop)
