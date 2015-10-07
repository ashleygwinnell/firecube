#include "BaseComponentPanelImpl.h"
#include "LuaScriptPanelImpl.h"
#include "app.h"
#include "wx/msgdlg.h"
#include "Commands/RemoveComponentCommand.h"
#include "Commands/CustomCommand.h"
#include "Types.h"

using namespace FireCube;

LuaScriptPanelImpl::LuaScriptPanelImpl(BaseComponentPanelImpl* parent) : LuaScriptPanel(parent), parent(parent)
{
	FireCube::LuaScript *luaScript = static_cast<FireCube::LuaScript *>(parent->GetComponent());
	if (luaScript->GetLuaFile())
	{
		wxFileName filename(luaScript->GetLuaFile()->GetFileName());
		scriptFilePicker->SetFileName(filename);
	}
	objectNameTextCtrl->SetLabel(luaScript->GetObjectName());

	parent->removeComponent->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LuaScriptPanelImpl::RemoveComponentClicked), NULL, this);
}

LuaScriptPanelImpl::~LuaScriptPanelImpl()
{

}

void LuaScriptPanelImpl::FileChanged(wxFileDirPickerEvent& event)
{
	LuaScript *luaScript = static_cast<FireCube::LuaScript *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = luaScript->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), luaScript));
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
	std::string oldLuaFileName = luaScript->GetLuaFile() ? luaScript->GetLuaFile()->GetFileName() : "";

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Script", [componentIndex, node, newLuaFileName, engine]()
	{
		LuaScript *luaScript = static_cast<LuaScript *>(node->GetComponents()[componentIndex]);
		luaScript->CreateObject(engine->GetResourceCache()->GetResource<FireCube::LuaFile>(newLuaFileName), luaScript->GetObjectName());		
	}, [componentIndex, node, oldLuaFileName, engine]()
	{
		LuaScript *luaScript = static_cast<LuaScript *>(node->GetComponents()[componentIndex]);
		luaScript->CreateObject(oldLuaFileName.empty() ? nullptr : engine->GetResourceCache()->GetResource<FireCube::LuaFile>(oldLuaFileName), luaScript->GetObjectName());
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}

void LuaScriptPanelImpl::RemoveComponentClicked(wxCommandEvent& event)
{
	LuaScript *luaScript = static_cast<LuaScript *>(parent->GetComponent());
	MyApp *theApp = ((MyApp *)wxTheApp);

	std::string currentScriptFile = luaScript->GetLuaFile() ? luaScript->GetLuaFile()->GetFileName() : "";
	std::string currentObjectName = luaScript->GetObjectName();

	auto removeComponentCommand = new RemoveComponentCommand(theApp->GetEditorState(), "Remove Component", luaScript, [currentScriptFile, currentObjectName](Engine *engine, Node *node) -> Component *
	{
		LuaScript *luaScript = node->CreateComponent<LuaScript>();
		luaScript->SetEnabled(false);
		if (currentScriptFile.empty() == false)
		{
			luaScript->CreateObject(engine->GetResourceCache()->GetResource<LuaFile>(currentScriptFile), currentObjectName);
		}
		else
		{
			luaScript->CreateObject(currentObjectName);
		}
		
		return luaScript;
	});

	theApp->GetEditorState()->ExecuteCommand(removeComponentCommand);
}

void LuaScriptPanelImpl::ObjectNameChanged(wxCommandEvent& event)
{
	LuaScript *luaScript = static_cast<FireCube::LuaScript *>(parent->GetComponent());

	MyApp *theApp = ((MyApp *)wxTheApp);
	auto engine = theApp->fcApp.GetEngine();
	Node *node = luaScript->GetNode();
	unsigned int componentIndex = std::distance(node->GetComponents().begin(), std::find(node->GetComponents().begin(), node->GetComponents().end(), luaScript));
	std::string newObjectName = event.GetString().ToStdString();
	std::string oldObjectName = luaScript->GetObjectName();

	auto command = new CustomCommand(theApp->GetEditorState(), "Change Object", [componentIndex, node, newObjectName, engine]()
	{
		LuaScript *luaScript = static_cast<LuaScript *>(node->GetComponents()[componentIndex]);
		luaScript->CreateObject(newObjectName);
	}, [componentIndex, node, oldObjectName, engine]()
	{
		LuaScript *luaScript = static_cast<LuaScript *>(node->GetComponents()[componentIndex]);
		luaScript->CreateObject(oldObjectName);
	});

	theApp->GetEditorState()->ExecuteCommand(command);
	theApp->GetEditorState()->sceneChanged(theApp->GetEditorState());
}