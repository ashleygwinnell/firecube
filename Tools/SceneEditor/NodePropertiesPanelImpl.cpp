#include "NodePropertiesPanelImpl.h"
#include "app.h"
#include "EditorState.h"
#include "Commands/TransformCommands.h"
#include "Commands/RenameNodeCommand.h"

using namespace FireCube;

NodePropertiesPanelImpl::NodePropertiesPanelImpl(wxWindow* parent) : NodePropertiesPanel(parent), Object(((MyApp*)wxTheApp)->fcApp.GetEngine()), editorState(((MyApp*)wxTheApp)->GetEditorState())
{
	SubscribeToEvent(editorState, editorState->nodeChanged, &NodePropertiesPanelImpl::UpdateUI);
	SubscribeToEvent(editorState, editorState->nodeRenamed, &NodePropertiesPanelImpl::NodeRenamed);
	UpdateUI();
}

NodePropertiesPanelImpl::~NodePropertiesPanelImpl()
{

}

void NodePropertiesPanelImpl::UpdateUI()
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		nameTextCtrl->SetLabelText(node->GetName());

		positionXTextCtrl->SetLabelText(wxString::FromDouble(node->GetTranslation().x));
		positionYTextCtrl->SetLabelText(wxString::FromDouble(node->GetTranslation().y));
		positionZTextCtrl->SetLabelText(wxString::FromDouble(node->GetTranslation().z));

		vec3 rotationAngles = node->GetRotation().ExtractEulerAngles();
		rotationXTextCtrl->SetLabelText(wxString::FromDouble(rotationAngles.x / PI * 180.0f));
		rotationYTextCtrl->SetLabelText(wxString::FromDouble(rotationAngles.y / PI * 180.0f));
		rotationZTextCtrl->SetLabelText(wxString::FromDouble(rotationAngles.z / PI * 180.0f));

		scaleXTextCtrl->SetLabelText(wxString::FromDouble(node->GetScale().x));
		scaleYTextCtrl->SetLabelText(wxString::FromDouble(node->GetScale().y));
		scaleZTextCtrl->SetLabelText(wxString::FromDouble(node->GetScale().z));
	}
}

void NodePropertiesPanelImpl::PositionXChanged(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		vec3 translation = node->GetTranslation();
		double newCoordinate;
		event.GetString().ToDouble(&newCoordinate);
		translation.x = newCoordinate;
		editorState->ExecuteCommand(new SetTranslationCommand(editorState, node, node->GetTranslation(), translation));
		editorState->sceneChanged(editorState);
	}
}

void NodePropertiesPanelImpl::PositionYChanged(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		vec3 translation = node->GetTranslation();
		double newCoordinate;
		event.GetString().ToDouble(&newCoordinate);
		translation.y = newCoordinate;
		editorState->ExecuteCommand(new SetTranslationCommand(editorState, node, node->GetTranslation(), translation));
		editorState->sceneChanged(editorState);
	}
}

void NodePropertiesPanelImpl::PositionZChanged(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		vec3 translation = node->GetTranslation();
		double newCoordinate;
		event.GetString().ToDouble(&newCoordinate);
		translation.z = newCoordinate;
		editorState->ExecuteCommand(new SetTranslationCommand(editorState, node, node->GetTranslation(), translation));
		editorState->sceneChanged(editorState);
	}
}

void NodePropertiesPanelImpl::RotationXChanged(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		vec3 rotation = node->GetRotation().ExtractEulerAngles();
		double ang;
		event.GetString().ToDouble(&ang);
		rotation.x = ang / 180.0f * PI;
		mat4 rotationMatrix = mat4::IDENTITY;
		rotationMatrix.RotateX(rotation.x);
		rotationMatrix.RotateY(rotation.y);
		rotationMatrix.RotateZ(rotation.z);
		editorState->ExecuteCommand(new SetRotationCommand(editorState, node, node->GetRotation(), rotationMatrix));
		editorState->sceneChanged(editorState);
	}
}

void NodePropertiesPanelImpl::RotationYChanged(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		vec3 rotation = node->GetRotation().ExtractEulerAngles();
		double ang;
		event.GetString().ToDouble(&ang);
		rotation.y = ang / 180.0f * PI;
		mat4 rotationMatrix = mat4::IDENTITY;
		rotationMatrix.RotateX(rotation.x);
		rotationMatrix.RotateY(rotation.y);
		rotationMatrix.RotateZ(rotation.z);
		editorState->ExecuteCommand(new SetRotationCommand(editorState, node, node->GetRotation(), rotationMatrix));
		editorState->sceneChanged(editorState);
	}
}

void NodePropertiesPanelImpl::RotationZChanged(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		vec3 rotation = node->GetRotation().ExtractEulerAngles();
		double ang;
		event.GetString().ToDouble(&ang);
		rotation.z = ang / 180.0f * PI;
		mat4 rotationMatrix = mat4::IDENTITY;
		rotationMatrix.RotateX(rotation.x);
		rotationMatrix.RotateY(rotation.y);
		rotationMatrix.RotateZ(rotation.z);
		editorState->ExecuteCommand(new SetRotationCommand(editorState, node, node->GetRotation(), rotationMatrix));
		editorState->sceneChanged(editorState);
	}
}

void NodePropertiesPanelImpl::ScaleXChanged(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		vec3 scale = node->GetScale();
		double newScale;
		event.GetString().ToDouble(&newScale);
		scale.x = newScale;
		editorState->ExecuteCommand(new SetScaleCommand(editorState, node, node->GetScale(), scale));
		editorState->sceneChanged(editorState);
	}
}

void NodePropertiesPanelImpl::ScaleYChanged(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		vec3 scale = node->GetScale();
		double newScale;
		event.GetString().ToDouble(&newScale);
		scale.y = newScale;
		editorState->ExecuteCommand(new SetScaleCommand(editorState, node, node->GetScale(), scale));
		editorState->sceneChanged(editorState);
	}
}

void NodePropertiesPanelImpl::ScaleZChanged(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		vec3 scale = node->GetScale();
		double newScale;
		event.GetString().ToDouble(&newScale);
		scale.z = newScale;
		editorState->ExecuteCommand(new SetScaleCommand(editorState, node, node->GetScale(), scale));
		editorState->sceneChanged(editorState);
	}
}

void NodePropertiesPanelImpl::NameChanged(wxCommandEvent& event)
{
	auto node = editorState->GetSelectedNode();
	if (node)
	{
		auto command = new RenameNodeCommand(editorState, node, event.GetString().ToStdString());
		editorState->ExecuteCommand(command);
	}
}

void NodePropertiesPanelImpl::NodeRenamed(FireCube::Node *node)
{
	nameTextCtrl->ChangeValue(node->GetName());
}