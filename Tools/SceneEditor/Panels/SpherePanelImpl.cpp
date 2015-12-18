#include "BaseComponentPanelImpl.h"
#include "SpherePanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../AssetUtils.h"
#include "../Descriptors/SphereDescriptor.h"

using namespace FireCube;

SpherePanelImpl::SpherePanelImpl(BaseComponentPanelImpl* parent) : SpherePanel(parent), parent(parent)
{
	SphereDescriptor *sphere = static_cast<SphereDescriptor *>(parent->GetComponent());

	radiusTextCtrl->SetLabel(wxString::FromDouble(sphere->GetRadius()));
	ringsTextCtrl->SetLabel(wxString::FromDouble(sphere->GetRings()));
	columnsTextCtrl->SetLabel(wxString::FromDouble(sphere->GetColumns()));
	castShadowCheckBox->SetValue(sphere->GetCastShadow());

	std::stringstream lightMaskStream;
	lightMaskStream << std::hex << sphere->GetLightMask();
	lightMaskTextCtrl->SetLabel(lightMaskStream.str());

	std::stringstream collisionQueryMaskStream;
	collisionQueryMaskStream << std::hex << sphere->GetCollisionQueryMask();
	collisionQueryMaskTextCtrl->SetLabel(collisionQueryMaskStream.str());

	materialFilePicker->SetPath(sphere->GetMaterialFileName());
}

SpherePanelImpl::~SpherePanelImpl()
{

}

void SpherePanelImpl::RadiusChanged(wxCommandEvent& event)
{
	SphereDescriptor *sphere = static_cast<SphereDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	double r;
	event.GetString().ToDouble(&r);
	float oldRadius = sphere->GetRadius();
	float newRadius = r;

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Radius", [sphere, newRadius, engine]()
	{
		sphere->SetRadius(newRadius, engine);
	}, [sphere, oldRadius, engine]()
	{
		sphere->SetRadius(oldRadius, engine);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void SpherePanelImpl::ColumnsChanged(wxCommandEvent& event)
{
	SphereDescriptor *sphere = static_cast<SphereDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	unsigned long columns;
	event.GetString().ToULong(&columns);
	unsigned int oldColumns = sphere->GetColumns();
	unsigned int newColumns = columns;

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Columns", [sphere, newColumns, engine]()
	{
		sphere->SetColumns(newColumns, engine);
	}, [sphere, oldColumns, engine]()
	{
		sphere->SetColumns(oldColumns, engine);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void SpherePanelImpl::RingsChanged(wxCommandEvent& event)
{
	SphereDescriptor *sphere = static_cast<SphereDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	unsigned long rings;
	event.GetString().ToULong(&rings);
	unsigned int oldRings = sphere->GetRings();
	unsigned int newRings = rings;

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Rings", [sphere, newRings, engine]()
	{
		sphere->SetRings(newRings, engine);
	}, [sphere, oldRings, engine]()
	{
		sphere->SetRings(oldRings, engine);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void SpherePanelImpl::CastShadowChanged(wxCommandEvent& event)
{
	SphereDescriptor *sphere = static_cast<SphereDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	bool oldShadow = sphere->GetCastShadow();
	bool newShadow = event.IsChecked();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Cast Shadow", [sphere, newShadow]()
	{
		sphere->SetCastShadow(newShadow);
	}, [sphere, oldShadow]()
	{
		sphere->SetCastShadow(oldShadow);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void SpherePanelImpl::LightMaskChanged(wxCommandEvent& event)
{
	SphereDescriptor *sphere = static_cast<SphereDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	unsigned int newValue = std::stoul(event.GetString().ToStdString(), 0, 16);
	unsigned int oldValue = sphere->GetLightMask();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mask", [sphere, newValue]()
	{
		sphere->SetLightMask(newValue);
	}, [sphere, oldValue]()
	{
		sphere->SetLightMask(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void SpherePanelImpl::CollisionQueryMaskChanged(wxCommandEvent& event)
{
	SphereDescriptor *sphere = static_cast<SphereDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	unsigned int newValue = std::stoul(event.GetString().ToStdString(), 0, 16);
	unsigned int oldValue = sphere->GetCollisionQueryMask();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Mask", [sphere, newValue]()
	{
		sphere->SetCollisionQueryMask(newValue);
	}, [sphere, oldValue]()
	{
		sphere->SetCollisionQueryMask(oldValue);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void SpherePanelImpl::MaterialFileChanged(wxFileDirPickerEvent& event)
{
	SphereDescriptor *sphere = static_cast<SphereDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();

	std::string newValue = AssetUtils::ImportMaterialIfNeeded(event.GetPath().ToStdString());
	std::string oldValue = sphere->GetMaterialFileName();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Material", [sphere, newValue, engine]()
	{
		sphere->SetMaterialFileName(newValue, engine);
	}, [sphere, oldValue, engine]()
	{
		sphere->SetMaterialFileName(oldValue, engine);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}