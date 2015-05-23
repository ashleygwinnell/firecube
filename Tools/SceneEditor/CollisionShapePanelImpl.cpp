#include "BaseComponentPanelImpl.h"
#include "CollisionShapePanelImpl.h"
#include "app.h"
#include "wx/msgdlg.h"
#include "Commands/RemoveComponentCommand.h"
#include "Commands/CustomCommand.h"
#include "Types.h"

using namespace FireCube;

CollisionShapePanelImpl::CollisionShapePanelImpl(BaseComponentPanelImpl* parent) : CollisionShapePanel(parent), parent(parent)
{	
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	UpdateUI();
	UpdateVisibility(collisionShape->GetShapeType());

	parent->removeComponent->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(CollisionShapePanelImpl::RemoveComponentClicked), NULL, this);
}

CollisionShapePanelImpl::~CollisionShapePanelImpl()
{

}

void CollisionShapePanelImpl::UpdateUI()
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

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

	if (collisionShape->GetMesh())
	{
		wxFileName filename(collisionShape->GetMesh()->GetFileName());
		meshFilePicker->SetFileName(filename);
	}
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

void CollisionShapePanelImpl::RemoveComponentClicked(wxCommandEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());
	MyApp *theApp = ((MyApp *)wxTheApp);
	Engine *engine = theApp->fcApp.GetEngine();

	bool isTrigger = collisionShape->IsTrigger();

	switch (collisionShape->GetShapeType())
	{
	case CollisionShapeType::BOX:
	{
		BoundingBox bbox = collisionShape->GetBox();
		auto removeComponentCommand = new RemoveComponentCommand(theApp->GetEditorState(), "Remove Component", collisionShape, [isTrigger, bbox](Engine *engine, Node *node) -> Component *
		{
			CollisionShape *collisionShape = node->CreateComponent<CollisionShape>();
			collisionShape->SetIsTrigger(isTrigger);
			collisionShape->SetBox(bbox);

			return collisionShape;
		});

		theApp->GetEditorState()->ExecuteCommand(removeComponentCommand);
		break;
	}
	case CollisionShapeType::PLANE:
	{
		Plane plane = collisionShape->GetPlane();
		auto removeComponentCommand = new RemoveComponentCommand(theApp->GetEditorState(), "Remove Component", collisionShape, [isTrigger, plane](Engine *engine, Node *node) -> Component *
		{
			CollisionShape *collisionShape = node->CreateComponent<CollisionShape>();
			collisionShape->SetIsTrigger(isTrigger);
			collisionShape->SetPlane(plane);

			return collisionShape;
		});

		theApp->GetEditorState()->ExecuteCommand(removeComponentCommand);
		break;
	}
	case CollisionShapeType::TRIANGLE_MESH:
	{
		Mesh *mesh = collisionShape->GetMesh();
		auto removeComponentCommand = new RemoveComponentCommand(theApp->GetEditorState(), "Remove Component", collisionShape, [isTrigger, mesh](Engine *engine, Node *node) -> Component *
		{
			CollisionShape *collisionShape = node->CreateComponent<CollisionShape>();
			collisionShape->SetIsTrigger(isTrigger);
			collisionShape->SetMesh(mesh);

			return collisionShape;
		});

		theApp->GetEditorState()->ExecuteCommand(removeComponentCommand);
		break;
	}
	default:
		break;
	}		
}

void CollisionShapePanelImpl::ShapeTypeChanged(wxCommandEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));

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
	Mesh *oldMesh = collisionShape->GetMesh();
	Plane oldPlane = collisionShape->GetPlane();
	BoundingBox oldBoundingBox = collisionShape->GetBox();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Shape Type", [componentIndex, node, newType, oldPlane, oldBoundingBox, oldMesh]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		switch (newType)
		{
		case CollisionShapeType::TRIANGLE_MESH:
			collisionShape->SetMesh(oldMesh);
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
	}, [componentIndex, node, oldType, oldPlane, oldBoundingBox, oldMesh]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		switch (oldType)
		{
		case CollisionShapeType::TRIANGLE_MESH:
			collisionShape->SetMesh(oldMesh);
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
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());
	
	MyApp *theApp = ((MyApp *)wxTheApp);	
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));
	double x, y, z, w;		
	event.GetString().ToDouble(&x);
	planeYTextCtrl->GetValue().ToDouble(&y);
	planeZTextCtrl->GetValue().ToDouble(&z);
	planeWTextCtrl->GetValue().ToDouble(&w);		
	Plane oldPlane = collisionShape->GetPlane();
	Plane newPlane(vec3(x, y, z).Normalized(), w);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Plane", [componentIndex, node, newPlane]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetPlane(newPlane);
	}, [componentIndex, node, oldPlane]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetPlane(oldPlane);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::PlaneYChanged(wxCommandEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));
	double x, y, z, w;
	event.GetString().ToDouble(&y);
	planeXTextCtrl->GetValue().ToDouble(&x);
	planeZTextCtrl->GetValue().ToDouble(&z);
	planeWTextCtrl->GetValue().ToDouble(&w);	
	Plane oldPlane = collisionShape->GetPlane();
	Plane newPlane(vec3(x, y, z).Normalized(), w);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Plane", [componentIndex, node, newPlane]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetPlane(newPlane);
	}, [componentIndex, node, oldPlane]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetPlane(oldPlane);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::PlaneZChanged(wxCommandEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));
	double x, y, z, w;
	event.GetString().ToDouble(&z);
	planeXTextCtrl->GetValue().ToDouble(&x);
	planeYTextCtrl->GetValue().ToDouble(&y);	
	planeWTextCtrl->GetValue().ToDouble(&w);
	Plane oldPlane = collisionShape->GetPlane();
	Plane newPlane(vec3(x, y, z).Normalized(), w);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Plane", [componentIndex, node, newPlane]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetPlane(newPlane);
	}, [componentIndex, node, oldPlane]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetPlane(oldPlane);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::PlaneWChanged(wxCommandEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));
	double x, y, z, w;
	event.GetString().ToDouble(&w);
	planeXTextCtrl->GetValue().ToDouble(&x);
	planeYTextCtrl->GetValue().ToDouble(&y);
	planeZTextCtrl->GetValue().ToDouble(&z);	
	Plane oldPlane = collisionShape->GetPlane();
	Plane newPlane(vec3(x, y, z).Normalized(), w);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Plane", [componentIndex, node, newPlane]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetPlane(newPlane);
	}, [componentIndex, node, oldPlane]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetPlane(oldPlane);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::BBoxMinXChanged(wxCommandEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));
	double minX, minY, minZ, maxX, maxY, maxZ;
	event.GetString().ToDouble(&minX);
	bboxMinYTextCtrl->GetValue().ToDouble(&minY);
	bboxMinZTextCtrl->GetValue().ToDouble(&minZ);
	bboxMaxXTextCtrl->GetValue().ToDouble(&maxX);
	bboxMaxYTextCtrl->GetValue().ToDouble(&maxY);
	bboxMaxZTextCtrl->GetValue().ToDouble(&maxZ);
	BoundingBox oldBox = collisionShape->GetBox();
	BoundingBox newBox(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [componentIndex, node, newBox]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetBox(newBox);
	}, [componentIndex, node, oldBox]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetBox(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::BBoxMinYChanged(wxCommandEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));
	double minX, minY, minZ, maxX, maxY, maxZ;
	event.GetString().ToDouble(&minY);
	bboxMinXTextCtrl->GetValue().ToDouble(&minX);
	bboxMinZTextCtrl->GetValue().ToDouble(&minZ);
	bboxMaxXTextCtrl->GetValue().ToDouble(&maxX);
	bboxMaxYTextCtrl->GetValue().ToDouble(&maxY);
	bboxMaxZTextCtrl->GetValue().ToDouble(&maxZ);
	BoundingBox oldBox = collisionShape->GetBox();
	BoundingBox newBox(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [componentIndex, node, newBox]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetBox(newBox);
	}, [componentIndex, node, oldBox]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetBox(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::BBoxMinZChanged(wxCommandEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));
	double minX, minY, minZ, maxX, maxY, maxZ;
	event.GetString().ToDouble(&minZ);
	bboxMinXTextCtrl->GetValue().ToDouble(&minX);
	bboxMinYTextCtrl->GetValue().ToDouble(&minY);
	bboxMaxXTextCtrl->GetValue().ToDouble(&maxX);
	bboxMaxYTextCtrl->GetValue().ToDouble(&maxY);
	bboxMaxZTextCtrl->GetValue().ToDouble(&maxZ);
	BoundingBox oldBox = collisionShape->GetBox();
	BoundingBox newBox(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [componentIndex, node, newBox]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetBox(newBox);
	}, [componentIndex, node, oldBox]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetBox(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::BBoxMaxXChanged(wxCommandEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));
	double minX, minY, minZ, maxX, maxY, maxZ;
	event.GetString().ToDouble(&maxX);
	bboxMinXTextCtrl->GetValue().ToDouble(&minX);
	bboxMinYTextCtrl->GetValue().ToDouble(&minY);
	bboxMinZTextCtrl->GetValue().ToDouble(&minZ);	
	bboxMaxYTextCtrl->GetValue().ToDouble(&maxY);
	bboxMaxZTextCtrl->GetValue().ToDouble(&maxZ);
	BoundingBox oldBox = collisionShape->GetBox();
	BoundingBox newBox(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [componentIndex, node, newBox]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetBox(newBox);
	}, [componentIndex, node, oldBox]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetBox(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::BBoxMaxYChanged(wxCommandEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));
	double minX, minY, minZ, maxX, maxY, maxZ;
	event.GetString().ToDouble(&maxY);
	bboxMinXTextCtrl->GetValue().ToDouble(&minX);
	bboxMinYTextCtrl->GetValue().ToDouble(&minY);
	bboxMinZTextCtrl->GetValue().ToDouble(&minZ);
	bboxMaxXTextCtrl->GetValue().ToDouble(&maxX);
	bboxMaxZTextCtrl->GetValue().ToDouble(&maxZ);
	BoundingBox oldBox = collisionShape->GetBox();
	BoundingBox newBox(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [componentIndex, node, newBox]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetBox(newBox);
	}, [componentIndex, node, oldBox]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetBox(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::BBoxMaxZChanged(wxCommandEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));
	double minX, minY, minZ, maxX, maxY, maxZ;
	event.GetString().ToDouble(&maxZ);
	bboxMinXTextCtrl->GetValue().ToDouble(&minX);
	bboxMinYTextCtrl->GetValue().ToDouble(&minY);
	bboxMinZTextCtrl->GetValue().ToDouble(&minZ);
	bboxMaxXTextCtrl->GetValue().ToDouble(&maxX);
	bboxMaxYTextCtrl->GetValue().ToDouble(&maxY);	
	BoundingBox oldBox = collisionShape->GetBox();
	BoundingBox newBox(vec3(minX, minY, minZ), vec3(maxX, maxY, maxZ));

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Box", [componentIndex, node, newBox]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetBox(newBox);
	}, [componentIndex, node, oldBox]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetBox(oldBox);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::TriggerChanged(wxCommandEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));
	bool oldTrigger = collisionShape->IsTrigger();
	bool newTrigger = event.IsChecked();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Trigger", [componentIndex, node, newTrigger]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetIsTrigger(newTrigger);
	}, [componentIndex, node, oldTrigger]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetIsTrigger(oldTrigger);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CollisionShapePanelImpl::MeshFileChanged(wxFileDirPickerEvent& event)
{
	CollisionShape *collisionShape = static_cast<CollisionShape *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = collisionShape->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), collisionShape));

	std::string newMeshFileName = event.GetPath().ToStdString();
	std::string oldMeshFileName = collisionShape->GetMesh() ? collisionShape->GetMesh()->GetFileName() : "";

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mesh", [componentIndex, node, newMeshFileName, engine]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetMesh(engine->GetResourceCache()->GetResource<Mesh>(newMeshFileName));
	}, [componentIndex, node, oldMeshFileName, engine]()
	{
		CollisionShape *collisionShape = static_cast<CollisionShape *>(node->GetComponents()[componentIndex]);
		collisionShape->SetMesh(oldMeshFileName.empty() ? nullptr : engine->GetResourceCache()->GetResource<Mesh>(oldMeshFileName));
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}