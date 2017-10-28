#include "BaseComponentPanelImpl.h"
#include "CollisionShapePanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/CollisionShapeDescriptor.h"
#include "EventBindingHelpers.h"

using namespace FireCube;

CollisionShapePanelImpl::CollisionShapePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : CollisionShapePanel(parent), parent(parent), 
	PanelCommon(engine, static_cast<CollisionShapeDescriptor *>(parent->GetComponent()))
{
	UpdateUI();

	BindTextCtrl(planeXTextCtrl, planeYTextCtrl, planeZTextCtrl, planeWTextCtrl, "Change Plane", &CollisionShapeDescriptor::GetPlane, &CollisionShapeDescriptor::SetPlane);
	BindTextCtrl(bboxMinXTextCtrl, bboxMinYTextCtrl, bboxMinZTextCtrl, bboxMaxXTextCtrl, bboxMaxYTextCtrl, bboxMaxZTextCtrl, "Change Box", &CollisionShapeDescriptor::GetBox, 
		&CollisionShapeDescriptor::SetBox);
	BindTextCtrl(radiusTextCtrl, "Change Radius", &CollisionShapeDescriptor::GetRadius, &CollisionShapeDescriptor::SetSphere);
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

	ChangeValue(planeXTextCtrl, wxString::FromDouble(collisionShape->GetPlane().GetNormal().x));
	ChangeValue(planeYTextCtrl, wxString::FromDouble(collisionShape->GetPlane().GetNormal().y));
	ChangeValue(planeZTextCtrl, wxString::FromDouble(collisionShape->GetPlane().GetNormal().z));
	ChangeValue(planeWTextCtrl, wxString::FromDouble(collisionShape->GetPlane().GetDistance()));

	ChangeValue(bboxMinXTextCtrl, wxString::FromDouble(collisionShape->GetBox().GetMin().x));
	ChangeValue(bboxMinYTextCtrl, wxString::FromDouble(collisionShape->GetBox().GetMin().y));
	ChangeValue(bboxMinZTextCtrl, wxString::FromDouble(collisionShape->GetBox().GetMin().z));

	ChangeValue(bboxMaxXTextCtrl, wxString::FromDouble(collisionShape->GetBox().GetMax().x));
	ChangeValue(bboxMaxYTextCtrl, wxString::FromDouble(collisionShape->GetBox().GetMax().y));
	ChangeValue(bboxMaxZTextCtrl, wxString::FromDouble(collisionShape->GetBox().GetMax().z));

	ChangeValue(radiusTextCtrl, wxString::FromDouble(collisionShape->GetRadius()));

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