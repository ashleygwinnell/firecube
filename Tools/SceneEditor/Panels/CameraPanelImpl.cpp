#include "BaseComponentPanelImpl.h"
#include "CameraPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/CameraDescriptor.h"

using namespace FireCube;

CameraPanelImpl::CameraPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : CameraPanel(parent), parent(parent), Object(engine)
{
	CameraDescriptor *camera = static_cast<CameraDescriptor *>(parent->GetComponent());

	UpdateUI();

	SubscribeToEvent(camera->componentChanged, &CameraPanelImpl::UpdateUI);
}

CameraPanelImpl::~CameraPanelImpl()
{

}

void CameraPanelImpl::UpdateUI()
{
	CameraDescriptor *camera = static_cast<CameraDescriptor *>(parent->GetComponent());

	UpdatePanelsVisibility(camera->GetOrthographic());

	leftTextCtrl->ChangeValue(wxString::FromDouble(camera->GetLeftPlane()));
	rightTextCtrl->ChangeValue(wxString::FromDouble(camera->GetRightPlane()));
	topTextCtrl->ChangeValue(wxString::FromDouble(camera->GetTopPlane()));
	bottomTextCtrl->ChangeValue(wxString::FromDouble(camera->GetBottomPlane()));
	fovTextCtrl->ChangeValue(wxString::FromDouble(camera->GetFOV()));	
	zNearTextCtrl->ChangeValue(wxString::FromDouble(camera->GetNearPlane()));
	zFarTextCtrl->ChangeValue(wxString::FromDouble(camera->GetFarPlane()));
	projectionTypeChoice->SetSelection(camera->GetOrthographic() ? 1 : 0);
}

void CameraPanelImpl::UpdatePanelsVisibility(bool orthographic)
{
	parent->GetParent()->Freeze();
	if (orthographic)
	{
		leftStaticText->Show();
		leftTextCtrl->Show();
		rightStaticText->Show();
		rightTextCtrl->Show();
		topStaticText->Show();
		topTextCtrl->Show();
		bottomStaticText->Show();
		bottomTextCtrl->Show();
		fovStaticText->Hide();
		fovTextCtrl->Hide();
	}
	else
	{
		leftStaticText->Hide();
		leftTextCtrl->Hide();
		rightStaticText->Hide();
		rightTextCtrl->Hide();
		topStaticText->Hide();
		topTextCtrl->Hide();
		bottomStaticText->Hide();
		bottomTextCtrl->Hide();
		fovStaticText->Show();
		fovTextCtrl->Show();
	}

	this->Fit();

	parent->Fit();

	parent->GetParent()->FitInside();
	parent->GetParent()->Layout();
	parent->GetParent()->Thaw();
}

void CameraPanelImpl::ProjectionTypeChanged(wxCommandEvent& event)
{
	CameraDescriptor *camera = static_cast<CameraDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	auto oldOrthographic = camera->GetOrthographic();
	bool newOrthographic;

	switch (event.GetSelection())
	{
	case 0:
		newOrthographic = false;
		break;
	case 1:
		newOrthographic = true;
		break;	
	default:
		break;
	}

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Projection Type", [camera, newOrthographic]()
	{
		camera->SetOrthographic(newOrthographic);
		camera->componentChanged(nullptr);
	}, [camera, oldOrthographic]()
	{
		camera->SetOrthographic(oldOrthographic);
		camera->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CameraPanelImpl::ZNearChanged(wxCommandEvent& event)
{
	CameraDescriptor *camera = static_cast<CameraDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	double newNearPlane;
	event.GetString().ToDouble(&newNearPlane);
	float oldNearPlane = camera->GetNearPlane();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Near Plane", [camera, newNearPlane]()
	{
		camera->SetNearPlane(newNearPlane);
		camera->componentChanged(nullptr);
	}, [camera, oldNearPlane]()
	{
		camera->SetNearPlane(oldNearPlane);
		camera->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CameraPanelImpl::ZFarChanged(wxCommandEvent& event)
{
	CameraDescriptor *camera = static_cast<CameraDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	double newFarPlane;
	event.GetString().ToDouble(&newFarPlane);
	float oldFarPlane = camera->GetFarPlane();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Far Plane", [camera, newFarPlane]()
	{
		camera->SetFarPlane(newFarPlane);
		camera->componentChanged(nullptr);
	}, [camera, oldFarPlane]()
	{
		camera->SetFarPlane(oldFarPlane);
		camera->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CameraPanelImpl::LeftChanged(wxCommandEvent& event)
{
	CameraDescriptor *camera = static_cast<CameraDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	double newLeftPlane;
	event.GetString().ToDouble(&newLeftPlane);
	float oldLeftPlane = camera->GetLeftPlane();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Left Plane", [camera, newLeftPlane]()
	{
		camera->SetLeftPlane(newLeftPlane);
		camera->componentChanged(nullptr);
	}, [camera, oldLeftPlane]()
	{
		camera->SetLeftPlane(oldLeftPlane);
		camera->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CameraPanelImpl::RightChanged(wxCommandEvent& event)
{
	CameraDescriptor *camera = static_cast<CameraDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	double newRightPlane;
	event.GetString().ToDouble(&newRightPlane);
	float oldRightPlane = camera->GetRightPlane();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Right Plane", [camera, newRightPlane]()
	{
		camera->SetRightPlane(newRightPlane);
		camera->componentChanged(nullptr);
	}, [camera, oldRightPlane]()
	{
		camera->SetRightPlane(oldRightPlane);
		camera->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CameraPanelImpl::TopChanged(wxCommandEvent& event)
{
	CameraDescriptor *camera = static_cast<CameraDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	double newTopPlane;
	event.GetString().ToDouble(&newTopPlane);
	float oldTopPlane = camera->GetTopPlane();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Top Plane", [camera, newTopPlane]()
	{
		camera->SetTopPlane(newTopPlane);
		camera->componentChanged(nullptr);
	}, [camera, oldTopPlane]()
	{
		camera->SetTopPlane(oldTopPlane);
		camera->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CameraPanelImpl::BottomChanged(wxCommandEvent& event)
{
	CameraDescriptor *camera = static_cast<CameraDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	double newBottomPlane;
	event.GetString().ToDouble(&newBottomPlane);
	float oldBottomPlane = camera->GetBottomPlane();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Bottom Plane", [camera, newBottomPlane]()
	{
		camera->SetBottomPlane(newBottomPlane);
		camera->componentChanged(nullptr);
	}, [camera, oldBottomPlane]()
	{
		camera->SetBottomPlane(oldBottomPlane);
		camera->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void CameraPanelImpl::FovChanged(wxCommandEvent& event)
{
	CameraDescriptor *camera = static_cast<CameraDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	double newFov;
	event.GetString().ToDouble(&newFov);
	float oldFov = camera->GetFOV();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Fov", [camera, newFov]()
	{
		camera->SetFOV(newFov);
		camera->componentChanged(nullptr);
	}, [camera, oldFov]()
	{
		camera->SetFOV(oldFov);
		camera->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}
