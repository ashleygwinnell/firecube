#pragma once

#include "FireCube.h"
#include "AuxRenderWindow.h"
#include "imguifilesystem.h"

class EditorState;
class NodeDescriptor;

enum class PropertyType2
{
	FLOAT, VEC2, VEC3, VEC4, COLOR
};

class MaterialEditorWindow : FireCube::Object
{
	FIRECUBE_OBJECT(MaterialEditorWindow);
public:
	MaterialEditorWindow(FireCube::Engine *engine);
	void Render();
	void SetScene(NodeDescriptor *rootDesc, EditorState *editorState);
	bool IsOpen() const;	
	void UpdatePreview(FireCube::Material *material);
	bool *GetIsOpenPtr();
private:
	void MaterialPicked(FireCube::Material *material);

	NodeDescriptor *rootDesc;
	EditorState *editorState;
	std::string currentFileName;
	FireCube::SharedPtr<FireCube::Material> material;	
	std::map<FireCube::StringHash, std::pair<std::string, PropertyType2>> standardParametersProperties;
	ImGuiFs::Dialog openDialog;	
	ImGuiFs::Dialog saveDialog;
	AuxRenderWindow auxRenderWindow;
	bool isOpen;
};