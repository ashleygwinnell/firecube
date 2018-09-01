#pragma once

#include "FireCube.h"
#include "AuxRenderWindow.h"

class EditorState;
class NodeDescriptor;

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
	void Show();
private:
	void MaterialPicked(FireCube::Material *material);

	NodeDescriptor *rootDesc;
	EditorState *editorState;
	std::string currentFileName;
	FireCube::SharedPtr<FireCube::Material> material;	
	AuxRenderWindow auxRenderWindow;
	bool isOpen;
};