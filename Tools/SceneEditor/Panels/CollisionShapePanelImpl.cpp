#include "BaseComponentPanelImpl.h"
#include "CollisionShapePanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/CollisionShapeDescriptor.h"
#include "EventBindingHelpers.h"

using namespace FireCube;

CollisionShapePanelImpl::CollisionShapePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : CollisionShapePanel(parent), parent(parent), Object(engine)
{	
	MyApp *theApp = ((MyApp *)wxTheApp);
 	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());
 
 	UpdateUI();
 	
	SubscribeToEvent(collisionShape->componentChanged, &CollisionShapePanelImpl::UpdateUI);
	SubscribeToEvent(theApp->GetEditorState(), theApp->GetEditorState()->undoPerformed, &CollisionShapePanelImpl::UndoPerformed);

	auto planeGetter = [](CollisionShapeDescriptor *collisionShape) {
		return collisionShape->GetPlane();
	};

	auto planeSetter = [](CollisionShapeDescriptor *collisionShape, const Plane &newVal) {
		collisionShape->SetPlane(newVal);		
	};

	auto planeEvtHandler = [this](CollisionShapeDescriptor *collisionShape, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);

		Plane curPlane = collisionShape->GetPlane();
		if (evt.GetEventObject() == planeXTextCtrl)
		{
			curPlane.SetNormal(vec3((float)newVal, curPlane.GetNormal().y, curPlane.GetNormal().z));
		}
		else if (evt.GetEventObject() == planeYTextCtrl)
		{
			curPlane.SetNormal(vec3(curPlane.GetNormal().x, (float)newVal, curPlane.GetNormal().z));
		}
		else if (evt.GetEventObject() == planeZTextCtrl)
		{
			curPlane.SetNormal(vec3(curPlane.GetNormal().x, curPlane.GetNormal().y, (float)newVal));
		}
		if (evt.GetEventObject() == planeWTextCtrl)
		{
			curPlane.SetDistance(newVal);
		}

		return curPlane;
	};

	EventBindingHelpers::BindTextCtrl<Plane, CollisionShapeDescriptor>(planeXTextCtrl, collisionShape, engine, theApp->GetEditorState(), "Change Plane", planeGetter, planeSetter, planeEvtHandler, prevCommand, prevPlane);
	EventBindingHelpers::BindTextCtrl<Plane, CollisionShapeDescriptor>(planeYTextCtrl, collisionShape, engine, theApp->GetEditorState(), "Change Plane", planeGetter, planeSetter, planeEvtHandler, prevCommand, prevPlane);
	EventBindingHelpers::BindTextCtrl<Plane, CollisionShapeDescriptor>(planeZTextCtrl, collisionShape, engine, theApp->GetEditorState(), "Change Plane", planeGetter, planeSetter, planeEvtHandler, prevCommand, prevPlane);
	EventBindingHelpers::BindTextCtrl<Plane, CollisionShapeDescriptor>(planeWTextCtrl, collisionShape, engine, theApp->GetEditorState(), "Change Plane", planeGetter, planeSetter, planeEvtHandler, prevCommand, prevPlane);

	auto bboxGetter = [](CollisionShapeDescriptor *collisionShape) {
		return collisionShape->GetBox();
	};

	auto bboxSetter = [](CollisionShapeDescriptor *collisionShape, const BoundingBox &newVal) {
		collisionShape->SetBox(newVal);
	};

	auto bboxEvtHandler = [this](CollisionShapeDescriptor *collisionShape, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);

		BoundingBox curBox = collisionShape->GetBox();
		if (evt.GetEventObject() == bboxMinXTextCtrl)
		{
			curBox.SetMin(vec3((float)newVal, curBox.GetMin().y, curBox.GetMin().z));
		}
		else if (evt.GetEventObject() == bboxMinYTextCtrl)
		{
			curBox.SetMin(vec3(curBox.GetMin().x, (float)newVal, curBox.GetMin().z));
		}
		else if (evt.GetEventObject() == bboxMinZTextCtrl)
		{
			curBox.SetMin(vec3(curBox.GetMin().x, curBox.GetMin().y, (float)newVal));
		}

		else if (evt.GetEventObject() == bboxMaxXTextCtrl)
		{
			curBox.SetMax(vec3((float)newVal, curBox.GetMax().y, curBox.GetMax().z));
		}
		else if (evt.GetEventObject() == bboxMaxYTextCtrl)
		{
			curBox.SetMax(vec3(curBox.GetMax().x, (float)newVal, curBox.GetMax().z));
		}
		else if (evt.GetEventObject() == bboxMaxZTextCtrl)
		{
			curBox.SetMax(vec3(curBox.GetMax().x, curBox.GetMax().y, (float)newVal));
		}

		return curBox;
	};

	EventBindingHelpers::BindTextCtrl<BoundingBox, CollisionShapeDescriptor>(bboxMinXTextCtrl, collisionShape, engine, theApp->GetEditorState(), "Change Box", bboxGetter, bboxSetter, bboxEvtHandler, prevCommand, prevBoundingBox);
	EventBindingHelpers::BindTextCtrl<BoundingBox, CollisionShapeDescriptor>(bboxMinYTextCtrl, collisionShape, engine, theApp->GetEditorState(), "Change Box", bboxGetter, bboxSetter, bboxEvtHandler, prevCommand, prevBoundingBox);
	EventBindingHelpers::BindTextCtrl<BoundingBox, CollisionShapeDescriptor>(bboxMinZTextCtrl, collisionShape, engine, theApp->GetEditorState(), "Change Box", bboxGetter, bboxSetter, bboxEvtHandler, prevCommand, prevBoundingBox);
	EventBindingHelpers::BindTextCtrl<BoundingBox, CollisionShapeDescriptor>(bboxMaxXTextCtrl, collisionShape, engine, theApp->GetEditorState(), "Change Box", bboxGetter, bboxSetter, bboxEvtHandler, prevCommand, prevBoundingBox);
	EventBindingHelpers::BindTextCtrl<BoundingBox, CollisionShapeDescriptor>(bboxMaxYTextCtrl, collisionShape, engine, theApp->GetEditorState(), "Change Box", bboxGetter, bboxSetter, bboxEvtHandler, prevCommand, prevBoundingBox);
	EventBindingHelpers::BindTextCtrl<BoundingBox, CollisionShapeDescriptor>(bboxMaxZTextCtrl, collisionShape, engine, theApp->GetEditorState(), "Change Box", bboxGetter, bboxSetter, bboxEvtHandler, prevCommand, prevBoundingBox);

	EventBindingHelpers::BindTextCtrl<float, CollisionShapeDescriptor>(radiusTextCtrl, collisionShape, engine, theApp->GetEditorState(), "Change Radius", [](CollisionShapeDescriptor *collisionShape) {
		return collisionShape->GetRadius();
	}, [](CollisionShapeDescriptor *collisionShape, const float &radius) {
		collisionShape->SetSphere(radius);
	}, [](CollisionShapeDescriptor *collisionShape, wxCommandEvent &evt) {
		double newVal;
		evt.GetString().ToDouble(&newVal);
		return (float)newVal;
	}, prevCommand, prevRadius);
}

CollisionShapePanelImpl::~CollisionShapePanelImpl()
{

}

void CollisionShapePanelImpl::UpdateUI()
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	CollisionShapeType type = collisionShape->GetShapeType();
	switch (type)
	{
	case CollisionShapeType::BOX:
		shapeTypeChoice->SetSelection(0);
		break;
	case CollisionShapeType::PLANE:
		shapeTypeChoice->SetSelection(1);
		break;
	case CollisionShapeType::TRIANGLE_MESH:
		shapeTypeChoice->SetSelection(2);
		break;
	case CollisionShapeType::SPHERE:
		shapeTypeChoice->SetSelection(3);
		break;
	default:
		break;
	}

	UpdateVisibility(type);

	planeXTextCtrl->ChangeValue(wxString::FromDouble(collisionShape->GetPlane().GetNormal().x));
	planeYTextCtrl->ChangeValue(wxString::FromDouble(collisionShape->GetPlane().GetNormal().y));
	planeZTextCtrl->ChangeValue(wxString::FromDouble(collisionShape->GetPlane().GetNormal().z));
	planeWTextCtrl->ChangeValue(wxString::FromDouble(collisionShape->GetPlane().GetDistance()));

	bboxMinXTextCtrl->ChangeValue(wxString::FromDouble(collisionShape->GetBox().GetMin().x));
	bboxMinYTextCtrl->ChangeValue(wxString::FromDouble(collisionShape->GetBox().GetMin().y));
	bboxMinZTextCtrl->ChangeValue(wxString::FromDouble(collisionShape->GetBox().GetMin().z));

	bboxMaxXTextCtrl->ChangeValue(wxString::FromDouble(collisionShape->GetBox().GetMax().x));
	bboxMaxYTextCtrl->ChangeValue(wxString::FromDouble(collisionShape->GetBox().GetMax().y));
	bboxMaxZTextCtrl->ChangeValue(wxString::FromDouble(collisionShape->GetBox().GetMax().z));

	radiusTextCtrl->ChangeValue(wxString::FromDouble(collisionShape->GetRadius()));

	meshFilePicker->SetInitialDirectory(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Models");
	if (collisionShape->GetMeshFilename().empty() == false)
	{
		wxFileName filename(collisionShape->GetMeshFilename());
		meshFilePicker->SetFileName(filename);
	}

	triggerCheckBox->SetValue(collisionShape->IsTrigger());
}

void CollisionShapePanelImpl::UpdateVisibility(CollisionShapeType type)
{
	parent->GetParent()->Freeze();
	
	meshStaticText->Hide();
	planeStaticText->Hide();
	boxStaticText->Hide();
	sphereStaticText->Hide();
	meshFilePicker->Hide();
	planePanel->Hide();
	boxPanel->Hide();
	radiusTextCtrl->Hide();

	switch (type)
	{
	case CollisionShapeType::TRIANGLE_MESH:
		meshStaticText->Show();
		meshFilePicker->Show();
		break;
	case CollisionShapeType::PLANE:
		planeStaticText->Show();
		planePanel->Show();
		break;
	case CollisionShapeType::BOX:
		boxStaticText->Show();
		boxPanel->Show();
		break;
	case CollisionShapeType::SPHERE:
		sphereStaticText->Show();
		radiusTextCtrl->Show();
		break;
	default:
		break;
	}

	this->Fit();

	parent->Fit();

	parent->GetParent()->FitInside();
	parent->GetParent()->Layout();
	parent->GetParent()->Thaw();
}

void CollisionShapePanelImpl::ShapeTypeChanged(wxCommandEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	CollisionShapeType newType;

	switch (event.GetSelection())
	{
	case 0:
		newType = CollisionShapeType::BOX;
		break;
	case 1:
		newType = CollisionShapeType::PLANE;
		break;
	case 2:
		newType = CollisionShapeType::TRIANGLE_MESH;
		break;
	case 3:
		newType = CollisionShapeType::SPHERE;
		break;
	default:
		break;
	}	

	CollisionShapeType oldType = collisionShape->GetShapeType();
	std::string oldMesh = collisionShape->GetMeshFilename();
	Plane oldPlane = collisionShape->GetPlane();
	BoundingBox oldBoundingBox = collisionShape->GetBox();
	float oldRadius = collisionShape->GetRadius();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Shape Type", [collisionShape, newType, oldPlane, oldBoundingBox, oldMesh, oldRadius, engine]()
	{		
		switch (newType)
		{
		case CollisionShapeType::TRIANGLE_MESH:
			collisionShape->SetMesh(oldMesh, engine);
			break;
		case CollisionShapeType::PLANE:
			collisionShape->SetPlane(oldPlane);
			break;
		case CollisionShapeType::BOX:
			collisionShape->SetBox(oldBoundingBox);
			break;
		case CollisionShapeType::SPHERE:
			collisionShape->SetSphere(oldRadius);
			break;
		default:
			break;
		}
		collisionShape->componentChanged(nullptr);
	}, [collisionShape, oldType, oldPlane, oldBoundingBox, oldMesh, oldRadius, engine]()
	{		
		switch (oldType)
		{
		case CollisionShapeType::TRIANGLE_MESH:
			collisionShape->SetMesh(oldMesh, engine);
			break;
		case CollisionShapeType::PLANE:
			collisionShape->SetPlane(oldPlane);
			break;
		case CollisionShapeType::BOX:
			collisionShape->SetBox(oldBoundingBox);
			break;
		case CollisionShapeType::SPHERE:
			collisionShape->SetSphere(oldRadius);
			break;
		default:
			break;
		}
		collisionShape->componentChanged(nullptr);
	});		

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::TriggerChanged(wxCommandEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	bool oldTrigger = collisionShape->IsTrigger();
	bool newTrigger = event.IsChecked();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Trigger", [collisionShape, newTrigger]()
	{	
		collisionShape->SetIsTrigger(newTrigger);
		collisionShape->componentChanged(nullptr);
	}, [collisionShape, oldTrigger]()
	{		
		collisionShape->SetIsTrigger(oldTrigger);
		collisionShape->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::MeshFileChanged(wxFileDirPickerEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	
	std::string newMeshFileName = event.GetPath().ToStdString();
	std::string oldMeshFileName = collisionShape->GetMeshFilename();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mesh", [collisionShape, newMeshFileName, engine]()
	{		
		collisionShape->SetMesh(newMeshFileName, engine);
		collisionShape->componentChanged(nullptr);
	}, [collisionShape, oldMeshFileName, engine]()
	{		
		collisionShape->SetMesh(oldMeshFileName, engine);
		collisionShape->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::UndoPerformed(Command *command)
{
	prevCommand = nullptr;
}