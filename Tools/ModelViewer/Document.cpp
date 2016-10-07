#include <set>
#include <FireCube.h>
using namespace FireCube;
#include "Document.h"

Document::Document() : modelNode(nullptr), gridNode(nullptr), model(nullptr), normalsGeometry(nullptr), tangentsGeometry(nullptr)
{

}

Document::~Document()
{
	delete scene;
}
void Document::CreateRootNode(Engine *engine)
{
	scene = new Scene(engine);
	root = scene->GetRootNode();
	
	gridMaterial = FireCube::SharedPtr<FireCube::Material>(new Material(engine));
	gridMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE_NAME, vec3(1.0f));
	gridMaterial->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/Unlit.xml"));

	tangentsMaterial = FireCube::SharedPtr<FireCube::Material>(new Material(engine));
	tangentsMaterial->SetParameter(PARAM_MATERIAL_DIFFUSE_NAME, vec3(1.0f, 0.0f, 0.0f));
	tangentsMaterial->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/Unlit.xml"));
}

bool Document::Load(const std::string &filename, Engine *engine)
{	
	if (!modelNode)
		modelNode = root->CreateChild("Model");
	modelNode->RemoveAllComponents();
	model = modelNode->CreateComponent<StaticModel>();
	model->CreateFromMesh(engine->GetResourceCache()->GetResource<Mesh>(filename));
			
	verticesCount = 0;
	facesCount = 0;
	CountElements(verticesCount, facesCount);
	
	return true;
}
Node *Document::GetRoot()
{
	return root;
}

FireCube::Scene *Document::GetScene()
{
	return scene;
}
void Document::CountElements(unsigned int &verticesCount, unsigned int &facesCount)
{
	if (model)
	{	
		std::set<VertexBuffer *> buffers;
		for (auto g : model->GetGeometries())
		{
			buffers.insert(g->GetVertexBuffer());
			facesCount += g->GetPrimitiveCount();
		}
		for (auto buffer : buffers)
		{
			verticesCount += buffer->GetVertexCount();
		}
	}
}
void Document::GenerateNormals(float l)
{
	if (model)
	{
		bool enabled = false;
		if (normalsGeometry)
		{
			enabled = normalsGeometry->IsEnabled();
			modelNode->RemoveComponent(normalsGeometry);
		}

		std::set<VertexBuffer *> buffers;
		for (auto g : model->GetGeometries())
		{
			buffers.insert(g->GetVertexBuffer());
			facesCount += g->GetPrimitiveCount();
		}
		normalsGeometry = modelNode->CreateComponent<CustomGeometry>();
		normalsGeometry->SetPrimitiveType(PrimitiveType::LINES);
		normalsGeometry->SetMaterial(gridMaterial);
		for (auto buffer : buffers)
		{
			if (buffer->HasAttribute(VertexAttributeType::NORMAL))
			{
				auto &vertexData = buffer->GetShadowData();
				for (unsigned int i = 0; i < buffer->GetVertexCount(); ++i)
				{
					vec3 pos = *((vec3 *) &vertexData[i * buffer->GetVertexSize()]);
					vec3 normal = *((vec3 *)&vertexData[i * buffer->GetVertexSize() + buffer->GetVertexAttributeOffset(VertexAttributeType::NORMAL)]);					
					normalsGeometry->AddVertex(pos);
					normalsGeometry->AddVertex(pos + normal * l);
				}
			}
		}
		normalsGeometry->UpdateGeometry();
		normalsGeometry->SetEnabled(enabled);
	}	
}

void Document::GenerateTangents(float l)
{
	if (model)
	{
		hasTangents = false;
		bool enabled = false;
		if (tangentsGeometry)
		{
			enabled = tangentsGeometry->IsEnabled();
			modelNode->RemoveComponent(tangentsGeometry);
		}

		std::set<VertexBuffer *> buffers;
		for (auto g : model->GetGeometries())
		{
			buffers.insert(g->GetVertexBuffer());
			facesCount += g->GetPrimitiveCount();
		}
		tangentsGeometry = modelNode->CreateComponent<CustomGeometry>();
		tangentsGeometry->SetPrimitiveType(PrimitiveType::LINES);
		tangentsGeometry->SetMaterial(tangentsMaterial);
		for (auto buffer : buffers)
		{
			if (buffer->HasAttribute(VertexAttributeType::TANGENT) && buffer->HasAttribute(VertexAttributeType::NORMAL))
			{
				hasTangents = true;
				auto &vertexData = buffer->GetShadowData();
				for (unsigned int i = 0; i < buffer->GetVertexCount(); ++i)
				{
					vec3 pos = *((vec3 *)&vertexData[i * buffer->GetVertexSize()]);
					vec3 tangent = *((vec3 *)&vertexData[i * buffer->GetVertexSize() + buffer->GetVertexAttributeOffset(VertexAttributeType::TANGENT)]);
					vec3 normal = *((vec3 *)&vertexData[i * buffer->GetVertexSize() + buffer->GetVertexAttributeOffset(VertexAttributeType::NORMAL)]);
					vec3 bitangent = FireCube::Cross(normal, tangent);
					tangentsGeometry->AddVertex(pos);
					tangentsGeometry->AddVertex(pos + tangent * l);
					tangentsGeometry->AddVertex(pos);
					tangentsGeometry->AddVertex(pos + bitangent * l);
				}
			}
		}
		tangentsGeometry->UpdateGeometry();
		tangentsGeometry->SetEnabled(enabled);
	}
}

bool Document::HasTangents() const
{
	return hasTangents;
}
unsigned int Document::GetVertexCount() const
{
	return verticesCount;
}
unsigned int Document::GetFaceCount() const
{
	return facesCount;
}

std::vector<FireCube::SharedPtr<FireCube::Material>> Document::GetAllMaterials()
{		
	if (model)
		return model->GetMaterials();
	else
		return std::vector<FireCube::SharedPtr<FireCube::Material>>();
}

void Document::CreateGrid(float size, DWORD numberOfCells)
{
	if (!gridNode)
		gridNode = root->CreateChild("GridNode");	
	gridNode->RemoveAllComponents();
	gridGeometry = gridNode->CreateComponent<CustomGeometry>();	
	for (DWORD i = 0; i < numberOfCells + 1; i++)
	{
		vec3 pos1((float)i * size - size * (float)numberOfCells / 2.0f, 0, -size * (float)numberOfCells / 2.0f);
		vec3 pos2((float)i * size - size * (float)numberOfCells / 2.0f, 0, size * (float)numberOfCells / 2.0f);
		vec3 pos3(-size * (float)numberOfCells / 2.0f, 0, (float)i * size - size * (float)numberOfCells / 2.0f);
		vec3 pos4(size * (float)numberOfCells / 2.0f, 0, (float)i * size - size * (float)numberOfCells / 2.0f);
		gridGeometry->AddVertex(pos1);
		gridGeometry->AddVertex(pos2);
		gridGeometry->AddVertex(pos3);
		gridGeometry->AddVertex(pos4);
	}
	gridGeometry->SetPrimitiveType(PrimitiveType::LINES);
	gridGeometry->SetMaterial(gridMaterial);
	gridGeometry->UpdateGeometry();
}

FireCube::CustomGeometry *Document::GetNormalsGeometry()
{
	return normalsGeometry;
}

FireCube::CustomGeometry *Document::GetTangentsGeometry()
{
	return tangentsGeometry;
}