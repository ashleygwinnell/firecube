#pragma once

#include "../Commands/CustomCommand.h"

namespace EventBindingHelpers
{
	template <class T, class Desc>
	void BindTextCtrl(wxTextCtrl *textCtrl, Desc *componentDescriptor, FireCube::Engine *engine, EditorState *editorState, const std::string &description, std::function<T(Desc *)> getValue, std::function<void(Desc *, const T &)> setValue, std::function<T(Desc *, wxCommandEvent &)> getNewValue, Command *&prevCommand, T &value)
	{
		textCtrl->Bind(wxEVT_KILL_FOCUS, [componentDescriptor, getValue, &value](wxFocusEvent &event) {
			value = getValue(componentDescriptor);
			event.Skip();
		});

		textCtrl->Bind(wxEVT_SET_FOCUS, [getValue, &value, &prevCommand, componentDescriptor](wxFocusEvent &event) {
			value = getValue(componentDescriptor);
			prevCommand = nullptr;
			event.Skip();
		});

		textCtrl->Bind(wxEVT_TEXT, [componentDescriptor, getValue, getNewValue, &value, engine, setValue, &prevCommand, editorState, description](wxCommandEvent &evt) {
			auto newVal = getNewValue(componentDescriptor, evt);
			auto command = new CustomCommand(editorState, description, [componentDescriptor, newVal, engine, setValue]()
			{
				setValue(componentDescriptor, newVal);
				componentDescriptor->componentChanged(nullptr);
			}, [componentDescriptor, value, engine, setValue]()
			{
				setValue(componentDescriptor, value);
				componentDescriptor->componentChanged(nullptr);
			});

			editorState->ExecuteCommand(command, prevCommand);
			prevCommand = command;
			editorState->sceneChanged(editorState);
		});
	}

	template <class T, class Cmd>
	void BindTextCtrl(wxTextCtrl *textCtrl, NodeDescriptor *nodeDesc, FireCube::Engine *engine, EditorState *editorState, const std::string &description, std::function<T(NodeDescriptor *)> getValue, std::function<void(NodeDescriptor *, const T &)> setValue, std::function<T(NodeDescriptor *, wxCommandEvent &)> getNewValue, Command *&prevCommand, T &value)
	{
		textCtrl->Bind(wxEVT_KILL_FOCUS, [nodeDesc, getValue, &value](wxFocusEvent &event) {
			value = getValue(nodeDesc);
			event.Skip();
		});

		textCtrl->Bind(wxEVT_SET_FOCUS, [getValue, &value, &prevCommand, nodeDesc](wxFocusEvent &event) {
			value = getValue(nodeDesc);
			prevCommand = nullptr;
			event.Skip();
		});

		textCtrl->Bind(wxEVT_TEXT, [nodeDesc, getValue, getNewValue, &value, engine, setValue, &prevCommand, editorState, description](wxCommandEvent &evt) {
			auto newVal = getNewValue(nodeDesc, evt);			
			auto command = new Cmd(editorState, description, nodeDesc, value, newVal);
			editorState->ExecuteCommand(command, prevCommand);
			prevCommand = command;
			editorState->sceneChanged(editorState);
		});
	}
}