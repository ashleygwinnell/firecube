#include "NodePropertiesPanelImpl.h"
#include "../app.h"
#include "../EditorState.h"
#include "../Commands/TransformCommands.h"
#include "../Commands/RenameNodeCommand.h"
#include "../Descriptors/NodeDescriptor.h"
#include "../Panels/EventBindingHelpers.h"

using namespace FireCube;

NodePropertiesPanelImpl::NodePropertiesPanelImpl(wxWindow* parent) : NodePropertiesPanel(parent), Object(((MyApp*)wxTheApp)->fcApp.GetEngine()), editorState(((MyApp*)wxTheApp)->GetEditorState()), 
	prevCommand(nullptr)
{
	SubscribeToEvent(editorState, editorState->nodeChanged, &NodePropertiesPanelImpl::UpdateUI);
	SubscribeToEvent(editorState, editorState->nodeRenamed, &NodePropertiesPanelImpl::NodeRenamed);
	SubscribeToEvent(editorState, editorState->undoPerformed, &NodePropertiesPanelImpl::UndoPerformed);
	UpdateUI();

	auto nodeDesc = editorState->GetSelectedNode();

	auto positionGetter = [](NodeDescriptor *nodeDesc) {
		return nodeDesc->GetTranslation();
	};

	auto positionSetter = [](NodeDescriptor *nodeDesc, const vec3 &v) {
		nodeDesc->SetTranslation(v);
	};

	auto positionEvtHandler = [this](NodeDescriptor *nodeDesc, wxCommandEvent &evt) {
		vec3 translation = nodeDesc->GetTranslation();
		double newVal;
		evt.GetString().ToDouble(&newVal);

		if (evt.GetEventObject() == positionXTextCtrl)
		{
			translation.x = newVal;
		}
		else if (evt.GetEventObject() == positionYTextCtrl)
		{
			translation.y = newVal;
		}
		else if (evt.GetEventObject() == positionZTextCtrl)
		{
			translation.z = newVal;
		}

		return translation;
	};

	EventBindingHelpers::BindTextCtrl<vec3, SetTranslationCommand>(positionXTextCtrl, nodeDesc, engine, editorState, "Translate", positionGetter, positionSetter, positionEvtHandler, prevCommand, prevTranslation);
	EventBindingHelpers::BindTextCtrl<vec3, SetTranslationCommand>(positionYTextCtrl, nodeDesc, engine, editorState, "Translate", positionGetter, positionSetter, positionEvtHandler, prevCommand, prevTranslation);
	EventBindingHelpers::BindTextCtrl<vec3, SetTranslationCommand>(positionZTextCtrl, nodeDesc, engine, editorState, "Translate", positionGetter, positionSetter, positionEvtHandler, prevCommand, prevTranslation);

	auto rotationGetter = [](NodeDescriptor *nodeDesc) {
		return nodeDesc->GetRotation();
	};

	auto rotationSetter = [](NodeDescriptor *nodeDesc, const vec3 &v) {
		nodeDesc->SetRotation(v);
	};

	auto rotationEvtHandler = [this](NodeDescriptor *nodeDesc, wxCommandEvent &evt) {
		vec3 rotation = nodeDesc->GetRotation();
		double newVal;
		evt.GetString().ToDouble(&newVal);
		newVal = newVal / 180.0f * PI;

		if (evt.GetEventObject() == rotationXTextCtrl)
		{
			rotation.x = newVal;
		}
		else if (evt.GetEventObject() == rotationYTextCtrl)
		{
			rotation.y = newVal;
		}
		else if (evt.GetEventObject() == rotationZTextCtrl)
		{
			rotation.z = newVal;
		}

		return rotation;
	};

	EventBindingHelpers::BindTextCtrl<vec3, SetRotationCommand>(rotationXTextCtrl, nodeDesc, engine, editorState, "Rotate", rotationGetter, rotationSetter, rotationEvtHandler, prevCommand, prevRotation);
	EventBindingHelpers::BindTextCtrl<vec3, SetRotationCommand>(rotationYTextCtrl, nodeDesc, engine, editorState, "Rotate", positionGetter, positionSetter, rotationEvtHandler, prevCommand, prevRotation);
	EventBindingHelpers::BindTextCtrl<vec3, SetRotationCommand>(rotationZTextCtrl, nodeDesc, engine, editorState, "Rotate", positionGetter, positionSetter, rotationEvtHandler, prevCommand, prevRotation);

	auto scaleGetter = [](NodeDescriptor *nodeDesc) {
		return nodeDesc->GetScale();
	};

	auto scaleSetter = [](NodeDescriptor *nodeDesc, const vec3 &v) {
		nodeDesc->SetScale(v);
	};

	auto scaleEvtHandler = [this](NodeDescriptor *nodeDesc, wxCommandEvent &evt) {
		vec3 scale = nodeDesc->GetScale();
		double newVal;
		evt.GetString().ToDouble(&newVal);

		if (evt.GetEventObject() == scaleXTextCtrl)
		{
			scale.x = newVal;
		}
		else if (evt.GetEventObject() == scaleYTextCtrl)
		{
			scale.y = newVal;
		}
		else if (evt.GetEventObject() == scaleZTextCtrl)
		{
			scale.z = newVal;
		}

		return scale;
	};

	EventBindingHelpers::BindTextCtrl<vec3, SetScaleCommand>(scaleXTextCtrl, nodeDesc, engine, editorState, "Scale", scaleGetter, scaleSetter, scaleEvtHandler, prevCommand, prevScale);
	EventBindingHelpers::BindTextCtrl<vec3, SetScaleCommand>(scaleYTextCtrl, nodeDesc, engine, editorState, "Scale", scaleGetter, scaleSetter, scaleEvtHandler, prevCommand, prevScale);
	EventBindingHelpers::BindTextCtrl<vec3, SetScaleCommand>(scaleZTextCtrl, nodeDesc, engine, editorState, "Scale", scaleGetter, scaleSetter, scaleEvtHandler, prevCommand, prevScale);
}

NodePropertiesPanelImpl::~NodePropertiesPanelImpl()
{

}

void NodePropertiesPanelImpl::UpdateUI()
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{
		auto node = nodeDesc->GetNode();
		nameTextCtrl->SetLabelText(node->GetName());

		positionXTextCtrl->ChangeValue(wxString::FromDouble(node->GetTranslation().x));
		positionYTextCtrl->ChangeValue(wxString::FromDouble(node->GetTranslation().y));
		positionZTextCtrl->ChangeValue(wxString::FromDouble(node->GetTranslation().z));

		vec3 rotationAngles = node->GetRotation().GetMatrix().ExtractEulerAngles();
		rotationXTextCtrl->ChangeValue(wxString::FromDouble(rotationAngles.x / PI * 180.0f));
		rotationYTextCtrl->ChangeValue(wxString::FromDouble(rotationAngles.y / PI * 180.0f));
		rotationZTextCtrl->ChangeValue(wxString::FromDouble(rotationAngles.z / PI * 180.0f));

		scaleXTextCtrl->ChangeValue(wxString::FromDouble(node->GetScale().x));
		scaleYTextCtrl->ChangeValue(wxString::FromDouble(node->GetScale().y));
		scaleZTextCtrl->ChangeValue(wxString::FromDouble(node->GetScale().z));
	}
}

void NodePropertiesPanelImpl::NameChanged(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{		
		auto command = new RenameNodeCommand(editorState, "Rename", nodeDesc, event.GetString().ToStdString());
		editorState->ExecuteCommand(command);
	}
}

void NodePropertiesPanelImpl::NodeRenamed(NodeDescriptor *node)
{
	nameTextCtrl->ChangeValue(node->GetNode()->GetName());
}

void NodePropertiesPanelImpl::UndoPerformed(Command *command)
{
	prevCommand = nullptr;
}
