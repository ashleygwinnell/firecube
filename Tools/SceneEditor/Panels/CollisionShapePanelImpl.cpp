#include "BaseComponentPanelImpl.h"
#include "CollisionShapePanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/CollisionShapeDescriptor.h"

using namespace FireCube;

CollisionShapePanelImpl::CollisionShapePanelImpl(BaseComponentPanelImpl* parent) : CollisionShapePanel(parent), parent(parent)
{	
 	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());
 
 	UpdateUI();
 	UpdateVisibility(collisionShape->GetShapeType());
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
	default:
		break;
	}

	planeXTextCtrl->SetLabelText(wxString::FromDouble(collisionShape->GetPlane().GetNormal().x));
	planeYTextCtrl->SetLabelText(wxString::FromDouble(collisionShape->GetPlane().GetNormal().y));
	planeZTextCtrl->SetLabelText(wxString::FromDouble(collisionShape->GetPlane().GetNormal().z));
	planeWTextCtrl->SetLabelText(wxString::FromDouble(collisionShape->GetPlane().GetDistance()));

	bboxMinXTextCtrl->SetLabelText(wxString::FromDouble(collisionShape->GetBox().GetMin().x));
	bboxMinYTextCtrl->SetLabelText(wxString::FromDouble(collisionShape->GetBox().GetMin().y));
	bboxMinZTextCtrl->SetLabelText(wxString::FromDouble(collisionShape->GetBox().GetMin().z));

	bboxMaxXTextCtrl->SetLabelText(wxString::FromDouble(collisionShape->GetBox().GetMax().x));
	bboxMaxYTextCtrl->SetLabelText(wxString::FromDouble(collisionShape->GetBox().GetMax().y));
	bboxMaxZTextCtrl->SetLabelText(wxString::FromDouble(collisionShape->GetBox().GetMax().z));

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
	meshFilePicker->Hide();
	planePanel->Hide();
	boxPanel->Hide();

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
	default:
		break;
	}

	UpdateVisibility(newType);

	CollisionShapeType oldType = collisionShape->GetShapeType();
	std::string oldMesh = collisionShape->GetMeshFilename();
	Plane oldPlane = collisionShape->GetPlane();
	BoundingBox oldBoundingBox = collisionShape->GetBox();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Shape Type", [collisionShape, newType, oldPlane, oldBoundingBox, oldMesh, engine]()
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
		default:
			break;
		}
	}, [collisionShape, oldType, oldPlane, oldBoundingBox, oldMesh, engine]()
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
		default:
			break;
		}
	});		

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::PlaneXChanged(wxCommandEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());
	
	MyApp *theApp = ((MyApp *)wxTheApp);	
	double x, y, z, w;		
	event.GetString().ToDouble(&x);
	planeYTextCtrl->GetValue().ToDouble(&y);
	planeZTextCtrl->GetValue().ToDouble(&z);
	planeWTextCtrl->GetValue().ToDouble(&w);		
	Plane oldPlane = collisionShape->GetPlane();
	Plane newPlane(vec3(x, y, z).Normalized(), w);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Plane", [collisionShape, newPlane]()
	{		
		collisionShape->SetPlane(newPlane);
	}, [collisionShape, oldPlane]()
	{		
		collisionShape->SetPlane(oldPlane);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::PlaneYChanged(wxCommandEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double x, y, z, w;
	event.GetString().ToDouble(&y);
	planeXTextCtrl->GetValue().ToDouble(&x);
	planeZTextCtrl->GetValue().ToDouble(&z);
	planeWTextCtrl->GetValue().ToDouble(&w);	
	Plane oldPlane = collisionShape->GetPlane();
	Plane newPlane(vec3(x, y, z).Normalized(), w);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Plane", [collisionShape, newPlane]()
	{		
		collisionShape->SetPlane(newPlane);
	}, [collisionShape, oldPlane]()
	{		
		collisionShape->SetPlane(oldPlane);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::PlaneZChanged(wxCommandEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double x, y, z, w;
	event.GetString().ToDouble(&z);
	planeXTextCtrl->GetValue().ToDouble(&x);
	planeYTextCtrl->GetValue().ToDouble(&y);	
	planeWTextCtrl->GetValue().ToDouble(&w);
	Plane oldPlane = collisionShape->GetPlane();
	Plane newPlane(vec3(x, y, z).Normalized(), w);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Plane", [collisionShape, newPlane]()
	{		
		collisionShape->SetPlane(newPlane);
	}, [collisionShape, oldPlane]()
	{		
		collisionShape->SetPlane(oldPlane);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::PlaneWChanged(wxCommandEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double x, y, z, w;
	event.GetString().ToDouble(&w);
	planeXTextCtrl->GetValue().ToDouble(&x);
	planeYTextCtrl->GetValue().ToDouble(&y);
	planeZTextCtrl->GetValue().ToDouble(&z);	
	Plane oldPlane = collisionShape->GetPlane();
	Plane newPlane(vec3(x, y, z).Normalized(), w);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Plane", [collisionShape, newPlane]()
	{		
		collisionShape->SetPlane(newPlane);
	}, [collisionShape, oldPlane]()
	{		
		collisionShape->SetPlane(oldPlane);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::BBoxMinXChanged(wxCommandEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double minX, minY, minZ, maxX, maxY, maxZ;
	event.GetString().ToDouble(&minX);
	bboxMinYTextCtrl->GetValue().ToDouble(&minY);
	bboxMinZTextCtrl->GetValue().ToDouble(&minZ);
	bboxMaxXTextCtrl->GetValue().ToDouble(&maxX);
	bboxMaxYTextCtrl->GetValue().ToDouble(&maxY);
	bboxMaxZTextCtrl->GetValue().ToDouble(&maxZ);
	BoundingBox oldBox = collisionShape->GetBox();
	BoundingBox newBox(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [collisionShape, newBox]()
	{		
		collisionShape->SetBox(newBox);
	}, [collisionShape, oldBox]()
	{		
		collisionShape->SetBox(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::BBoxMinYChanged(wxCommandEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double minX, minY, minZ, maxX, maxY, maxZ;
	event.GetString().ToDouble(&minY);
	bboxMinXTextCtrl->GetValue().ToDouble(&minX);
	bboxMinZTextCtrl->GetValue().ToDouble(&minZ);
	bboxMaxXTextCtrl->GetValue().ToDouble(&maxX);
	bboxMaxYTextCtrl->GetValue().ToDouble(&maxY);
	bboxMaxZTextCtrl->GetValue().ToDouble(&maxZ);
	BoundingBox oldBox = collisionShape->GetBox();
	BoundingBox newBox(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [collisionShape, newBox]()
	{
		collisionShape->SetBox(newBox);
	}, [collisionShape, oldBox]()
	{
		collisionShape->SetBox(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::BBoxMinZChanged(wxCommandEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double minX, minY, minZ, maxX, maxY, maxZ;
	event.GetString().ToDouble(&minZ);
	bboxMinXTextCtrl->GetValue().ToDouble(&minX);
	bboxMinYTextCtrl->GetValue().ToDouble(&minY);
	bboxMaxXTextCtrl->GetValue().ToDouble(&maxX);
	bboxMaxYTextCtrl->GetValue().ToDouble(&maxY);
	bboxMaxZTextCtrl->GetValue().ToDouble(&maxZ);
	BoundingBox oldBox = collisionShape->GetBox();
	BoundingBox newBox(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [collisionShape, newBox]()
	{
		collisionShape->SetBox(newBox);
	}, [collisionShape, oldBox]()
	{
		collisionShape->SetBox(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::BBoxMaxXChanged(wxCommandEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double minX, minY, minZ, maxX, maxY, maxZ;
	event.GetString().ToDouble(&maxX);
	bboxMinXTextCtrl->GetValue().ToDouble(&minX);
	bboxMinYTextCtrl->GetValue().ToDouble(&minY);
	bboxMinZTextCtrl->GetValue().ToDouble(&minZ);	
	bboxMaxYTextCtrl->GetValue().ToDouble(&maxY);
	bboxMaxZTextCtrl->GetValue().ToDouble(&maxZ);
	BoundingBox oldBox = collisionShape->GetBox();
	BoundingBox newBox(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [collisionShape, newBox]()
	{
		collisionShape->SetBox(newBox);
	}, [collisionShape, oldBox]()
	{
		collisionShape->SetBox(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::BBoxMaxYChanged(wxCommandEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double minX, minY, minZ, maxX, maxY, maxZ;
	event.GetString().ToDouble(&maxY);
	bboxMinXTextCtrl->GetValue().ToDouble(&minX);
	bboxMinYTextCtrl->GetValue().ToDouble(&minY);
	bboxMinZTextCtrl->GetValue().ToDouble(&minZ);
	bboxMaxXTextCtrl->GetValue().ToDouble(&maxX);
	bboxMaxZTextCtrl->GetValue().ToDouble(&maxZ);
	BoundingBox oldBox = collisionShape->GetBox();
	BoundingBox newBox(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [collisionShape, newBox]()
	{
		collisionShape->SetBox(newBox);
	}, [collisionShape, oldBox]()
	{
		collisionShape->SetBox(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::BBoxMaxZChanged(wxCommandEvent& event)
{
	CollisionShapeDescriptor *collisionShape = static_cast<CollisionShapeDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	double minX, minY, minZ, maxX, maxY, maxZ;
	event.GetString().ToDouble(&maxZ);
	bboxMinXTextCtrl->GetValue().ToDouble(&minX);
	bboxMinYTextCtrl->GetValue().ToDouble(&minY);
	bboxMinZTextCtrl->GetValue().ToDouble(&minZ);
	bboxMaxXTextCtrl->GetValue().ToDouble(&maxX);
	bboxMaxYTextCtrl->GetValue().ToDouble(&maxY);	
	BoundingBox oldBox = collisionShape->GetBox();
	BoundingBox newBox(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [collisionShape, newBox]()
	{
		collisionShape->SetBox(newBox);
	}, [collisionShape, oldBox]()
	{
		collisionShape->SetBox(oldBox);
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
	}, [collisionShape, oldTrigger]()
	{		
		collisionShape->SetIsTrigger(oldTrigger);
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
	}, [collisionShape, oldMeshFileName, engine]()
	{		
		collisionShape->SetMesh(oldMeshFileName, engine);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}