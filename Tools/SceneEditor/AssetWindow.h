#pragma once

#include "FireCube.h"
#include "AssetUtils.h"

class NodeDescriptor;
class EditorState;

class AssetWindow : public FireCube::Object
{
	FIRECUBE_OBJECT(AssetWindow);
public:
	AssetWindow(FireCube::Engine *engine);
	void Render();
	void SetScene(NodeDescriptor *rootDesc, EditorState *editorState);
	bool *GetIsOpenPtr();
private:
	void AssetSelected(const std::string &asset);
	void RenderTextureAsset();
	void RenderMeshAsset();

	NodeDescriptor *rootDesc;
	EditorState *editorState;
	bool isOpen;
	std::string currentAsset;
	AssetType assetType;
	FireCube::Texture2D *texture;
	FireCube::Image *image;
	std::set<FireCube::SharedPtr<FireCube::Material>> meshMaterials;
	std::set<std::string> meshMaterialNames;
	FireCube::MeshMetadata meshMetadata;
};