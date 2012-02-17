#ifndef INPUT_H
#define INPUT_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

/**
* The type of a mapping.
* <br>An action happens upon a key press, while a state is on while the key is down.
*/
enum InputMappingType
{
	ACTION, STATE
};

/**
* List of keys for mapping.
*/
enum Key
{
	KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I,
	KEY_J, KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R,
	KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
	KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
	KEY_ESCAPE, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN,
	KEY_LEFT_SHIFT, KEY_RIGHT_SHIFT, KEY_LEFT_CTRL, KEY_RIGHT_CTRL, KEY_LEFT_ALT, KEY_RIGHT_ALT,
	KEY_TAB,
	KEY_MOUSE_LEFT_BUTTON, KEY_MOUSE_MIDDLE_BUTTON, KEY_MOUSE_RIGHT_BUTTON
};

/**
* Key modifiers: Shift, Control, Alt.
*/
enum KeyModifier
{
	MODIFIER_NONE = 0,
	MODIFIER_LEFT_SHIFT = 1,
	MODIFIER_RIGHT_SHIFT = 2,
	MODIFIER_SHIFT = 3,
	MODIFIER_LEFT_CTRL = 4,
	MODIFIER_RIGHT_CTRL = 8,
	MODIFIER_CTRL = 12,
	MODIFIER_LEFT_ALT = 16,
	MODIFIER_RIGHT_ALT = 32,
	MODIFIER_ALT = 48,
	MODIFIER_ANY = 64
};

/**
* Analog mappings.
*/
enum AnalogInput
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
* Interface class to handle input.
*/
class FIRECUBE_API InputListener
{
public:
	/**
	* Override this function to handle input.
	* @param time The time passed since last frame.
	* @param input The current input state.
	*/
	virtual void HandleInput(float time, const MappedInput &input) = 0;
};

/**
* This class is responsible to handle all input.
*/
class FIRECUBE_API InputManager
{
	friend class Application;
public:
	/**
	* Adds an input listener.
	* @param inputListener the InputListener to add.
	*/
	void AddInputListener(InputListener *inputListener);

	/**
	* Removes an input listener.
	* @param inputListener the InputListener to remove.
	*/
	void RemoveInputListener(InputListener *inputListener);

	/**
	* Adds a key mapping.
	* @param key The key which is associated with this mapping.
	* @param inputMappingType The type of the mapping(action or state).
	* @param actionName the name of the mapping.
	* @param modifier A modifier for the mapping.
	*/
	void AddMapping(Key key, InputMappingType inputMappingType, const std::string &actionName, KeyModifier modifier = MODIFIER_ANY);

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
	void RemoveMapping(Key key, InputMappingType inputMappingType, const std::string &actionName, KeyModifier modifier = MODIFIER_ANY);

	/**
	* Removes an analog mapping.
	* @param analogInput The type of the analog input.
	* @param name The name of the mapping.
	*/
	void RemoveMapping(AnalogInput analogInput, const std::string &name);

	/**
	* This function is used internally to provide key state from the underlying input system(OS).
	* @param key The given key.
	* @param pressed Specifies whether the key is pressed.
	* @param previouslyPressed Specifies whether the key was previously pressed.
	* @param modifier Specifies the key modifiers.
	*/
	void SetRawKeyState(Key key, bool pressed, bool previouslyPressed, KeyModifier modifier = MODIFIER_ANY);

	/**
	* This function is used internally to provide analog input from the underlying input system(OS).
	* @param analogInput The given analog input.
	* @param value The value of this analog input.
	*/
	void SetRawAnalogValue(AnalogInput analogInput, float value);

	/**
	* This function is used internally to dispatch the current input state to all input listeners.
	* @param time The time passed since last frame.
	*/
	void DispatchInput(float time);
private:
	class InputMapping
	{
	public:
		InputMappingType inputMappingType;
		std::string actionName;
		KeyModifier modifier;
	};
	std::map<Key, std::vector<InputMapping>> mappedKeys;
	std::map<AnalogInput, std::vector<std::string>> mappedAnalogs;
	std::vector<InputListener *> inputListeners;
	MappedInput mappedInput;
};

}

#pragma warning(pop)
#endif