#include "BaseComponentPanelImpl.h"
#include "PlanePanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../AssetUtils.h"
#include "../Descriptors/PlaneDescriptor.h"

using namespace FireCube;

PlanePanelImpl::PlanePanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : PlanePanel(parent), parent(parent), Object(engine)
{
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());
	
	materialFilePicker->SetInitialDirectory(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Materials");

	UpdateUI();

	SubscribeToEvent(plane->componentChanged, &PlanePanelImpl::UpdateUI);
}

PlanePanelImpl::~PlanePanelImpl()
{

}

void PlanePanelImpl::WidthChanged(wxCommandEvent& event)
{
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	double x;
	event.GetString().ToDouble(&x);
	vec2 oldSize = plane->GetSize();
	vec2 newSize(x, oldSize.y);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Width", [plane, newSize, engine]()
	{
		plane->SetSize(newSize, engine);
		plane->componentChanged(nullptr);
	}, [plane, oldSize, engine]()
	{
		plane->SetSize(oldSize, engine);
		plane->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void PlanePanelImpl::DepthChanged(wxCommandEvent& event)
{
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	double y;
	event.GetString().ToDouble(&y);
	vec2 oldSize = plane->GetSize();
	vec2 newSize(oldSize.x, y);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Depth", [plane, newSize, engine]()
	{
		plane->SetSize(newSize, engine);
		plane->componentChanged(nullptr);
	}, [plane, oldSize, engine]()
	{
		plane->SetSize(oldSize, engine);
		plane->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void PlanePanelImpl::CastShadowChanged(wxCommandEvent& event)
{
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	bool oldShadow = plane->GetCastShadow();
	bool newShadow = event.IsChecked();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Cast Shadow", [plane, newShadow]()
	{
		plane->SetCastShadow(newShadow);
		plane->componentChanged(nullptr);
	}, [plane, oldShadow]()
	{
		plane->SetCastShadow(oldShadow);
		plane->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void PlanePanelImpl::LightMaskChanged(wxCommandEvent& event)
{
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	unsigned int newValue = std::stoul(event.GetString().ToStdString(), 0, 16);
	unsigned int oldValue = plane->GetLightMask();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mask", [plane, newValue]()
	{
		plane->SetLightMask(newValue);
		plane->componentChanged(nullptr);
	}, [plane, oldValue]()
	{
		plane->SetLightMask(oldValue);
		plane->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void PlanePanelImpl::CollisionQueryMaskChanged(wxCommandEvent& event)
{
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	unsigned int newValue = std::stoul(event.GetString().ToStdString(), 0, 16);
	unsigned int oldValue = plane->GetCollisionQueryMask();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mask", [plane, newValue]()
	{
		plane->SetCollisionQueryMask(newValue);
		plane->componentChanged(nullptr);
	}, [plane, oldValue]()
	{
		plane->SetCollisionQueryMask(oldValue);
		plane->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void PlanePanelImpl::MaterialFileChanged(wxFileDirPickerEvent& event)
{
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	std::string newValue = AssetUtils::ImportMaterialIfNeeded(event.GetPath().ToStdString());
	std::string oldValue = plane->GetMaterialFileName();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Material", [plane, newValue, engine]()
	{
		plane->SetMaterialFileName(newValue, engine);
		plane->componentChanged(nullptr);
	}, [plane, oldValue, engine]()
	{
		plane->SetMaterialFileName(oldValue, engine);
		plane->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void PlanePanelImpl::UpdateUI()
{
	PlaneDescriptor *plane = static_cast<PlaneDescriptor *>(parent->GetComponent());

	widthTextCtrl->ChangeValue(wxString::FromDouble(plane->GetSize().x));
	depthTextCtrl->ChangeValue(wxString::FromDouble(plane->GetSize().y));
	castShadowCheckBox->SetValue(plane->GetCastShadow());

	std::stringstream lightMaskStream;
	lightMaskStream << std::hex << plane->GetLightMask();
	lightMaskTextCtrl->ChangeValue(lightMaskStream.str());

	std::stringstream collisionQueryMaskStream;
	collisionQueryMaskStream << std::hex << plane->GetCollisionQueryMask();
	collisionQueryMaskTextCtrl->ChangeValue(collisionQueryMaskStream.str());

	materialFilePicker->SetPath(plane->GetMaterialFileName());
}
