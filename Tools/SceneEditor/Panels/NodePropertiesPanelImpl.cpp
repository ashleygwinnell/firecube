#include "NodePropertiesPanelImpl.h"
#include "../app.h"
#include "../EditorState.h"
#include "../Commands/TransformCommands.h"
#include "../Commands/RenameNodeCommand.h"
#include "../Descriptors/NodeDescriptor.h"

using namespace FireCube;

NodePropertiesPanelImpl::NodePropertiesPanelImpl(wxWindow* parent) : NodePropertiesPanel(parent), Object(((MyApp*)wxTheApp)->fcApp.GetEngine()), editorState(((MyApp*)wxTheApp)->GetEditorState()),
	commandAdded(false)
{
	SubscribeToEvent(editorState, editorState->nodeChanged, &NodePropertiesPanelImpl::UpdateUI);
	SubscribeToEvent(editorState, editorState->nodeRenamed, &NodePropertiesPanelImpl::NodeRenamed);
	SubscribeToEvent(editorState, editorState->undoPerformed, &NodePropertiesPanelImpl::UndoPerformed);
	UpdateUI();
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

void NodePropertiesPanelImpl::TextSetFocus(wxFocusEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{
		prevTranslation = nodeDesc->GetTranslation();		 
		prevRotation = nodeDesc->GetRotation();
		prevScale = nodeDesc->GetScale();
	}

	event.Skip();
}

void NodePropertiesPanelImpl::TextKillFocus(wxFocusEvent& event)
{
	commandAdded = false;

	event.Skip();
}


void NodePropertiesPanelImpl::PositionXChanged(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{		
		vec3 translation = nodeDesc->GetTranslation();
		double newCoordinate;
		event.GetString().ToDouble(&newCoordinate);
		translation.x = newCoordinate;
		editorState->ExecuteCommand(new SetTranslationCommand(editorState, "Translate", nodeDesc, prevTranslation, translation), commandAdded == false);
		editorState->sceneChanged(editorState);
		commandAdded = true;
	}
}

void NodePropertiesPanelImpl::PositionYChanged(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{		
		vec3 translation = nodeDesc->GetTranslation();
		double newCoordinate;
		event.GetString().ToDouble(&newCoordinate);
		translation.y = newCoordinate;
		editorState->ExecuteCommand(new SetTranslationCommand(editorState, "Translate", nodeDesc, prevTranslation, translation), commandAdded == false);
		editorState->sceneChanged(editorState);
		commandAdded = true;
	}
}

void NodePropertiesPanelImpl::PositionZChanged(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{		
		vec3 translation = nodeDesc->GetTranslation();
		double newCoordinate;
		event.GetString().ToDouble(&newCoordinate);
		translation.z = newCoordinate;
		editorState->ExecuteCommand(new SetTranslationCommand(editorState, "Translate", nodeDesc, prevTranslation, translation), commandAdded == false);
		editorState->sceneChanged(editorState);
		commandAdded = true;
	}
}

void NodePropertiesPanelImpl::RotationXChanged(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{		
		vec3 rotation = nodeDesc->GetRotation();
		double ang;
		event.GetString().ToDouble(&ang);
		rotation.x = ang / 180.0f * PI;
		editorState->ExecuteCommand(new SetRotationCommand(editorState, "Rotate", nodeDesc, prevRotation, rotation), commandAdded == false);
		editorState->sceneChanged(editorState);
		commandAdded = true;
	}
}

void NodePropertiesPanelImpl::RotationYChanged(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{		
		vec3 rotation = nodeDesc->GetRotation();
		double ang;
		event.GetString().ToDouble(&ang);
		rotation.y = ang / 180.0f * PI;
		editorState->ExecuteCommand(new SetRotationCommand(editorState, "Rotate", nodeDesc, prevRotation, rotation), commandAdded == false);
		editorState->sceneChanged(editorState);
		commandAdded = true;
	}
}

void NodePropertiesPanelImpl::RotationZChanged(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{		
		vec3 rotation = nodeDesc->GetRotation();
		double ang;
		event.GetString().ToDouble(&ang);
		rotation.z = ang / 180.0f * PI;
		editorState->ExecuteCommand(new SetRotationCommand(editorState, "Rotate", nodeDesc, prevRotation, rotation), commandAdded == false);
		editorState->sceneChanged(editorState);
		commandAdded = true;
	}
}

void NodePropertiesPanelImpl::ScaleXChanged(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{		
		vec3 scale = nodeDesc->GetScale();
		double newScale;
		event.GetString().ToDouble(&newScale);
		scale.x = newScale;
		editorState->ExecuteCommand(new SetScaleCommand(editorState, "Scale", nodeDesc, prevScale, scale), commandAdded == false);
		editorState->sceneChanged(editorState);
		commandAdded = true;
	}
}

void NodePropertiesPanelImpl::ScaleYChanged(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{		
		vec3 scale = nodeDesc->GetScale();
		double newScale;
		event.GetString().ToDouble(&newScale);
		scale.y = newScale;
		editorState->ExecuteCommand(new SetScaleCommand(editorState, "Scale", nodeDesc, prevScale, scale), commandAdded == false);
		editorState->sceneChanged(editorState);
		commandAdded = true;
	}
}

void NodePropertiesPanelImpl::ScaleZChanged(wxCommandEvent& event)
{
	auto nodeDesc = editorState->GetSelectedNode();
	if (nodeDesc)
	{		
		vec3 scale = nodeDesc->GetScale();
		double newScale;
		event.GetString().ToDouble(&newScale);
		scale.z = newScale;
		editorState->ExecuteCommand(new SetScaleCommand(editorState, "Scale", nodeDesc, prevScale, scale), commandAdded == false);
		editorState->sceneChanged(editorState);
		commandAdded = true;
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
	commandAdded = false;
}
