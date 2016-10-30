#include <fstream>
#include <regex>
#include "BaseComponentPanelImpl.h"
#include "LuaScriptPanelImpl.h"
#include "../app.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"
#include "../Commands/CustomCommand.h"
#include "../Types.h"
#include "../Descriptors/LuaScriptDescriptor.h"

using namespace FireCube;

LuaScriptPanelImpl::LuaScriptPanelImpl(BaseComponentPanelImpl* parent, FireCube::Engine *engine) : LuaScriptPanel(parent), parent(parent), Object(engine)
{
	LuaScriptDescriptor *luaScript = static_cast<LuaScriptDescriptor *>(parent->GetComponent());
	
	scriptFilePicker->SetInitialDirectory(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + "Scripts");
	
	UpdateUI();
	
	propertyGrid->Connect(wxEVT_PG_LABEL_EDIT_ENDING, wxPropertyGridEventHandler(LuaScriptPanelImpl::PropertyGridLabelChanged), NULL, this);

	SubscribeToEvent(luaScript->componentChanged, &LuaScriptPanelImpl::UpdateUI);
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
	std::string oldObjectName = luaScript->GetObjectName();

	std::string newObjectName = GetObjectNameFromScript(Filesystem::GetAssetsFolder() + Filesystem::PATH_SEPARATOR + newLuaFileName);	

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Script", [luaScript, newLuaFileName, newObjectName]()
	{
		luaScript->SetScriptFilename(newLuaFileName);
		if (newObjectName.empty() == false)
		{
			luaScript->SetObjectName(newObjectName);
		}
		luaScript->componentChanged(nullptr);
	}, [luaScript, oldLuaFileName, oldObjectName]()
	{		
		luaScript->SetScriptFilename(oldLuaFileName);		
		luaScript->SetObjectName(oldObjectName);
		luaScript->componentChanged(nullptr);
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
		luaScript->componentChanged(nullptr);
	}, [luaScript, oldObjectName]()
	{
		luaScript->SetObjectName(oldObjectName);
		luaScript->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LuaScriptPanelImpl::AddPropertyClicked(wxCommandEvent& event)
{
	LuaScriptDescriptor *luaScript = static_cast<LuaScriptDescriptor *>(parent->GetComponent());		

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto propertyName = wxGetTextFromUser("Enter property name", "Add Property");
	
	if (propertyName.empty() == false)
	{
		auto command = new CustomCommand(theApp->GetEditorState(), "Add Property", [luaScript, propertyName]()
		{
			luaScript->SetProperty(propertyName.ToStdString(), "");
			luaScript->componentChanged(nullptr);
		}, [luaScript, propertyName]()
		{
			luaScript->RemoveProperty(propertyName.ToStdString());			
			luaScript->componentChanged(nullptr);
		});

		theApp->GetEditorState()->ExecuteCommand(command);
		theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
	}	
}

void LuaScriptPanelImpl::RenamePropertyClicked(wxCommandEvent& event)
{	
	propertyGrid->BeginLabelEdit();
}

void LuaScriptPanelImpl::RemovePropertyClicked(wxCommandEvent& event)
{
	LuaScriptDescriptor *luaScript = static_cast<LuaScriptDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);	

	if (propertyGrid->GetSelectedProperty())
	{
		std::string propertyName = propertyGrid->GetSelectedProperty()->GetLabel();
		std::string propertyValue = propertyGrid->GetSelectedProperty()->GetValue().GetString().ToStdString();

		auto command = new CustomCommand(theApp->GetEditorState(), "Remove Property", [luaScript, propertyName]()
		{
			luaScript->RemoveProperty(propertyName);
			luaScript->componentChanged(nullptr);
		}, [luaScript, propertyName, propertyValue]()
		{
			luaScript->SetProperty(propertyName, propertyValue);
			luaScript->componentChanged(nullptr);
		});

		theApp->GetEditorState()->ExecuteCommand(command);
		theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
	}
}

void LuaScriptPanelImpl::PropertyGridChanged(wxPropertyGridEvent& event)
{
	LuaScriptDescriptor *luaScript = static_cast<LuaScriptDescriptor *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);

	std::string propertyName = event.GetProperty()->GetLabel().ToStdString();
	std::string newPropertyValue = event.GetPropertyValue().GetString().ToStdString();
	std::string oldPropertyValue = luaScript->GetProperty(propertyName);

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Value", [luaScript, propertyName, newPropertyValue]()
	{
		luaScript->SetProperty(propertyName, newPropertyValue);
		luaScript->componentChanged(nullptr);
	}, [luaScript, propertyName, oldPropertyValue]()
	{
		luaScript->SetProperty(propertyName, oldPropertyValue);
		luaScript->componentChanged(nullptr);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LuaScriptPanelImpl::PropertyGridLabelChanged(wxPropertyGridEvent& event)
{	
	LuaScriptDescriptor *luaScript = static_cast<LuaScriptDescriptor *>(parent->GetComponent());
	
	MyApp *theApp = ((MyApp *)wxTheApp);
	const wxString &newLabel = propertyGrid->GetLabelEditor()->GetValue();	
	const wxString &oldLabel = event.GetProperty()->GetLabel();
	auto oldPropertyName = oldLabel.ToStdString();
	auto newPropertyName = newLabel.ToStdString();
	auto propertyValue = luaScript->GetProperty(oldPropertyName);
			
	CallAfter([theApp, luaScript, newPropertyName, oldPropertyName, propertyValue]() {
		auto command = new CustomCommand(theApp->GetEditorState(), "Rename Property", [luaScript, newPropertyName, oldPropertyName, propertyValue]()
		{
			luaScript->SetProperty(newPropertyName, propertyValue);
			luaScript->RemoveProperty(oldPropertyName);
			luaScript->componentChanged(nullptr);
		}, [luaScript, newPropertyName, oldPropertyName, propertyValue]()
		{
			luaScript->SetProperty(oldPropertyName, propertyValue);
			luaScript->RemoveProperty(newPropertyName);
			luaScript->componentChanged(nullptr);
		});

		theApp->GetEditorState()->ExecuteCommand(command);
		theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
	});	
}

std::string LuaScriptPanelImpl::GetObjectNameFromScript(const std::string &filename)
{
	std::ifstream file(filename);
	const std::string code((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	
	std::regex rgx("\\s*(\\w+)\\s*=\\s*Script");
	std::smatch match;

	if (std::regex_search(code.begin(), code.end(), match, rgx))
	{
		return match[1];
	}

	return "";
}

void LuaScriptPanelImpl::UpdateUI()
{
	LuaScriptDescriptor *luaScript = static_cast<LuaScriptDescriptor *>(parent->GetComponent());	
	objectNameTextCtrl->ChangeValue(luaScript->GetObjectName());
	wxFileName filename(luaScript->GetScriptFilename());
	scriptFilePicker->SetFileName(filename);		
		
	propertyGrid->Freeze();
	propertyGrid->Clear();
	
	auto &properties = luaScript->GetProperties();
	for (auto &property : properties)
	{
		propertyGrid->Append(new wxStringProperty(property.first, wxPG_LABEL, property.second));
	}	

	propertyGrid->Thaw();
}
