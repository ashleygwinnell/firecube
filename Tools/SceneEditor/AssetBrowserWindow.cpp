#define NOMINMAX
#include "AssetBrowserWindow.h"
#include "imgui.h"
#include "EditorState.h"
#include "SceneReader.h"
#include "Descriptors/NodeDescriptor.h"

using namespace FireCube;

AssetBrowserWindow::AssetBrowserWindow(Engine *engine) : Object(engine), selectedItem(nullptr), texturePreview(nullptr), firstRender(true), auxRenderWindow(engine)
{

}

void AssetBrowserWindow::Render()
{
	NodeDescriptor *selectedNode = editorState->GetSelectedNode();

	ImGui::SetNextDock(ImGuiDockSlot_Bottom);
	if (ImGui::BeginDock("Asset Browser", nullptr, 0, ImVec2(50, -1)))
	{
		ImGui::Columns(3, "assetBrowserColumns", true);

		if (firstRender)
		{
			ImGui::SetColumnWidth(-1, 200);
		}

		auto assetsFolder = Filesystem::GetAssetsFolder();
		if (assetsFolder.empty() == false)
		{
			ImGui::BeginChild("Tree", ImVec2(0, 0), false);
			RenderDirectoryTree(assetsFolder);
			ImGui::EndChild();
		}
		ImGui::NextColumn();
		if (itemsInSelectedPath.empty() == false)
		{
			ImGui::BeginChild("List", ImVec2(0, 0), false);
			for (auto &item : itemsInSelectedPath)
			{
				if (ImGui::Selectable(item.label.c_str(), selectedItem == &item, ImGuiSelectableFlags_AllowDoubleClick))
				{
					if (ImGui::IsMouseDoubleClicked(0))
					{
						if (item.isDirectory)
						{
							selectedPath = item.path;
							selectedItem = nullptr;
							itemsInSelectedPath = GetItemsInPath(selectedPath);
							break;
						}
					}
					else
					{
						selectedItem = &item;
						texturePreview = nullptr;
						if (item.assetType == AssetType::TEXTURE)
						{
							texturePreview = engine->GetResourceCache()->GetResource<Texture2D>(item.path);
						}
						else if (item.assetType == AssetType::MATERIAL)
						{
							std::string materialPath = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), item.path);

							auxRenderWindow.Reset();
							auto root = auxRenderWindow.GetRoot();
							auto sphereNode = root->CreateChild();

							auto material = engine->GetResourceCache()->GetResource<Material>(materialPath);

							auto geometry = GeometryGenerator::GenerateSphere(engine, 0.5f, 16, 16);
							SharedPtr<Mesh> sphereMesh = new Mesh(engine);
							sphereMesh->AddGeometry(geometry, BoundingBox(vec3(-0.5f), vec3(0.5f)), material);

							sphereNode->CreateComponent<StaticModel>(sphereMesh);

							auto lightNode1 = root->CreateChild();
							auto light = lightNode1->CreateComponent<Light>();
							light->SetLightType(LightType::DIRECTIONAL);
							light->SetColor(1.0f);
							lightNode1->Rotate(vec3((float)-PI * 0.25f, (float)-PI * 0.25f, 0.0f));

							auto lightNode2 = root->CreateChild();
							light = lightNode2->CreateComponent<Light>();
							light->SetLightType(LightType::DIRECTIONAL);
							light->SetColor(1.0f);
							lightNode2->Rotate(vec3((float)PI * 0.25f, (float)PI * 0.25f, 0.0f));
						}
						else if (item.assetType == AssetType::MESH)
						{
							std::string meshPath = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), item.path);

							auxRenderWindow.Reset();
							auto root = auxRenderWindow.GetRoot();
							auto meshNode = root->CreateChild();

							auto mesh = engine->GetResourceCache()->GetResource<Mesh>(meshPath);
							meshNode->CreateComponent<StaticModel>(mesh);
							auto bbox = mesh->GetBoundingBox();
							float maxSize = std::max(bbox.GetWidth(), std::max(bbox.GetHeight(), bbox.GetDepth()));
							meshNode->Scale(vec3(1.0f / maxSize));

							auto lightNode1 = root->CreateChild();
							auto light = lightNode1->CreateComponent<Light>();
							light->SetLightType(LightType::DIRECTIONAL);
							light->SetColor(1.0f);
							lightNode1->Rotate(vec3((float)-PI * 0.25f, (float)-PI * 0.25f, 0.0f));

							auto lightNode2 = root->CreateChild();
							light = lightNode2->CreateComponent<Light>();
							light->SetLightType(LightType::DIRECTIONAL);
							light->SetColor(1.0f);
							lightNode2->Rotate(vec3((float)PI * 0.25f, (float)PI * 0.25f, 0.0f));
						}
						else if (item.assetType == AssetType::PREFAB)
						{
							std::string prefabPath = Filesystem::MakeRelativeTo(Filesystem::GetAssetsFolder(), item.path);

							auxRenderWindow.Reset();
							auto root = auxRenderWindow.GetRoot();

							std::map<Node *, NodeDescriptor *> nodeMap;
							::SceneReader sceneReader(engine);
							auto prefab = sceneReader.ReadPrefab(prefabPath);
							auto instance = prefab->Instantiate(nullptr, engine, nodeMap);
							instance->SetTranslation(vec3(0.0f));
							instance->SetScale(vec3(1.0f));

							std::vector<StaticModel *> staticModels;
							std::vector<AnimatedModel *> animatedModels;
							instance->GetComponents(staticModels, true);
							BoundingBox bbox;
							for (auto &staticModel : staticModels)
							{
								bbox.Expand(staticModel->GetWorldBoundingBox());
							}

							for (auto &animatedModel : animatedModels)
							{
								bbox.Expand(animatedModel->GetWorldBoundingBox());
							}

							float scale = 1.0f / std::max(std::max(bbox.GetWidth(), bbox.GetHeight()), bbox.GetDepth());

							instance->SetTranslation(-bbox.GetCenter() * scale);
							instance->SetScale(vec3(scale));
							root->AddChild(instance);

							auto lightNode1 = root->CreateChild();
							auto light = lightNode1->CreateComponent<Light>();
							light->SetLightType(LightType::DIRECTIONAL);
							light->SetColor(1.0f);
							lightNode1->Rotate(vec3((float)-PI * 0.25f, (float)-PI * 0.25f, 0.0f));

							auto lightNode2 = root->CreateChild();
							light = lightNode2->CreateComponent<Light>();
							light->SetLightType(LightType::DIRECTIONAL);
							light->SetColor(1.0f);
							lightNode2->Rotate(vec3((float)PI * 0.25f, (float)PI * 0.25f, 0.0f));
						}
					}
				}
			}
			ImGui::EndChild();
		}
		ImGui::NextColumn();

		if (selectedItem)
		{
			ImGui::BeginChild("Preview", ImVec2(0, 0), false);
			if (selectedItem->assetType == AssetType::TEXTURE)
			{
				ImVec2 size = ImGui::GetContentRegionAvail();
				int width = texturePreview->GetWidth();
				int height = texturePreview->GetHeight();
				float maxDimension = std::min(size.x, size.y);

				if (width > maxDimension || height > maxDimension)
				{
					if (width > height)
					{
						size.x = maxDimension;
						size.y = (float)height / (float)width * maxDimension;
					}
					else
					{
						size.y = maxDimension;
						size.x = (float)width / (float)height * maxDimension;
					}
				}
				else
				{
					size = ImVec2((float)width, (float)height);
				}

				ImGui::Image((void *)(texturePreview->GetObjectId()), size, ImVec2(0, 1), ImVec2(1, 0));
			}
			else if (selectedItem->assetType == AssetType::MATERIAL || selectedItem->assetType == AssetType::MESH || selectedItem->assetType == AssetType::PREFAB)
			{
				auxRenderWindow.Render();
			}
			ImGui::EndChild();
		}
	}
	ImGui::EndDock();

	firstRender = false;
}

void AssetBrowserWindow::RenderDirectoryTree(const std::string &path)
{
	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (selectedPath == path ? ImGuiTreeNodeFlags_Selected : 0);
	std::vector<std::string> items = GetDirectoriesInPath(path);
	if (items.empty())
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	ImGui::PushID(path.c_str());
	std::string label;
	if (path == Filesystem::GetAssetsFolder())
	{
		nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
		label = "Assets";
	}
	else
	{
		label = Filesystem::GetLastPathComponent(path);
	}

	bool nodeOpen = ImGui::TreeNodeEx(label.c_str(), nodeFlags);
	ImGui::PopID();
	if (ImGui::IsItemClicked())
	{
		selectedPath = path;
		selectedItem = nullptr;
		itemsInSelectedPath = GetItemsInPath(path);
	}

	if (nodeOpen && items.empty() == false)
	{
		for (auto &p : items)
		{
			RenderDirectoryTree(p);
		}
		ImGui::TreePop();
	}
}

void AssetBrowserWindow::Reset()
{
	selectedPath = "";
	selectedItem = nullptr;
	itemsInSelectedPath.clear();
}

void AssetBrowserWindow::SetScene(NodeDescriptor *rootDesc, EditorState *editorState)
{
	this->rootDesc = rootDesc;
	this->editorState = editorState;
}

std::vector<std::string> AssetBrowserWindow::GetDirectoriesInPath(const std::string &path)
{
	std::vector<std::string> ret;
	std::string pattern(path);
	pattern.append("\\*");
	WIN32_FIND_DATAA data;
	HANDLE hFind;
	if ((hFind = FindFirstFileA(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				std::string p = data.cFileName;
				if (p != "." && p != "..")
				{
					ret.push_back(Filesystem::JoinPath(path, p));
				}
			}
		} while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
	}

	return ret;
}

std::vector<AssetBrowserWindow::FileInfo> AssetBrowserWindow::GetItemsInPath(const std::string &path)
{
	std::vector<FileInfo> ret;
	std::string pattern(path);
	pattern.append("\\*");
	WIN32_FIND_DATAA data;
	HANDLE hFind;
	if ((hFind = FindFirstFileA(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::string p = data.cFileName;
			if (p != "." && p != "..")
			{
				FileInfo item;
				item.path = Filesystem::JoinPath(path, p);
				item.label = Filesystem::GetLastPathComponent(p);
				item.isDirectory = data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
				if (!item.isDirectory)
				{
					item.assetType = AssetUtils::GetAssetTypeByPath(item.path);
				}
				ret.push_back(item);
			}
		} while (FindNextFileA(hFind, &data) != 0);
		FindClose(hFind);
	}

	return ret;
}