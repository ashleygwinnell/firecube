#include "BaseComponentPanelImpl.h"
#include "LuaScriptPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/LuaScriptDescriptor.h"

using namespace FireCube;

LuaScriptPanelImpl::LuaScriptPanelImpl(BaseComponentPanelImpl* parent) : LuaScriptPanel(parent), parent(parent)
{
	LuaScriptDescriptor *luaScript = static_cast<LuaScriptDescriptor *>(parent->GetComponent());
	wxFileName filename(luaScript->GetScriptFilename());
	scriptFilePicker->SetFileName(filename);
	
	objectNameTextCtrl->SetLabel(luaScript->GetObjectName());

	propertyGrid->Freeze();	
	auto &properties = luaScript->GetProperties();
	for (auto &property : properties)
	{
		propertyGrid->Append(new wxStringProperty(property.first, wxPG_LABEL, property.second));
	}
	propertyGrid->Thaw();
	propertyGrid->Connect(wxEVT_PG_LABEL_EDIT_ENDING, wxPropertyGridEventHandler(LuaScriptPanelImpl::PropertyGridLabelChanged), NULL, this);
}

LuaScriptPanelImpl::~LuaScriptPanelImpl()
{

}

void LuaScriptPanelImpl::FileChanged(wxFileDirPickerEvent& event)
{
	LuaScriptDescriptor *luaScript = static_cast<LuaScriptDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);	
	std::string newLuaFileName = event.GetPath().ToStdString();
	if (Filesystem::IsSubPathOf(Filesystem::GetAssetsFolder(), newLuaFileName))
	{
		newLuaFileName = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), newLuaFileName);
	}
	else
	{
		std::string filename = Filesystem::GetLastPathComponent(newLuaFileName);
		std::string targetPath = Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Scripts";
		Filesystem::CreateFolder(targetPath);
		Filesystem::CopyPath(newLuaFileName, targetPath + Filesystem::PATH_SEPARATOR + filename);
		newLuaFileName = "Scripts" + Filesystem::PATH_SEPARATOR + filename;
	}
	std::string oldLuaFileName = luaScript->GetScriptFilename();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Script", [luaScript, newLuaFileName]()
	{
		luaScript->SetScriptFilename(newLuaFileName);		
	}, [luaScript, oldLuaFileName]()
	{		
		luaScript->SetScriptFilename(oldLuaFileName);		
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LuaScriptPanelImpl::ObjectNameChanged(wxCommandEvent& event)
{
	LuaScriptDescriptor *luaScript = static_cast<LuaScriptDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	std::string newObjectName = event.GetString().ToStdString();
	std::string oldObjectName = luaScript->GetObjectName();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Object", [luaScript, newObjectName]()
	{
		luaScript->SetObjectName(newObjectName);
	}, [luaScript, oldObjectName]()
	{
		luaScript->SetObjectName(oldObjectName);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LuaScriptPanelImpl::AddPropertyClicked(wxCommandEvent& event)
{
	LuaScriptDescriptor *luaScript = static_cast<LuaScriptDescriptor *>(parent->GetComponent());
	
	auto propertyName = wxGetTextFromUser("Enter property name", "Add Property");
	if (propertyName.empty() == false)
	{
		propertyGrid->Append(new wxStringProperty(propertyName, wxPG_LABEL));
		luaScript->SetProperty(propertyName.ToStdString(), "");		
	}
}

void LuaScriptPanelImpl::RenamePropertyClicked(wxCommandEvent& event)
{	
	propertyGrid->BeginLabelEdit();
}

void LuaScriptPanelImpl::RemovePropertyClicked(wxCommandEvent& event)
{
	if (propertyGrid->GetSelectedProperty())
	{
		propertyGrid->RemoveProperty(propertyGrid->GetSelectedProperty());
	}
}

void LuaScriptPanelImpl::PropertyGridChanged(wxPropertyGridEvent& event)
{
	LuaScriptDescriptor *luaScript = static_cast<LuaScriptDescriptor *>(parent->GetComponent());
	luaScript->SetProperty(event.GetProperty()->GetLabel().ToStdString(), event.GetPropertyValue().GetString().ToStdString());	
}

void LuaScriptPanelImpl::PropertyGridLabelChanged(wxPropertyGridEvent& event)
{	
	LuaScriptDescriptor *luaScript = static_cast<LuaScriptDescriptor *>(parent->GetComponent());
	
	const wxString &newLabel = propertyGrid->GetLabelEditor()->GetValue();	
	const wxString &oldLabel = event.GetProperty()->GetLabel();
	auto oldProperty = oldLabel.ToStdString();
	auto newProperty = newLabel.ToStdString();
	
	luaScript->SetProperty(newProperty, luaScript->GetProperty(oldProperty));
	luaScript->RemoveProperty(oldProperty);	
	event.GetProperty()->SetLabel(newLabel);
}