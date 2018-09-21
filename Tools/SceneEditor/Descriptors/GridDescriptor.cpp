#include "FireCube.h"
#include "GridDescriptor.h"

using namespace FireCube;

GridDescriptor::GridDescriptor() : ComponentDescriptor(ComponentType::GRID), collisionQueryMask(0xFFFFFFFF), castShadow(true), lightMask(0xFFFFFFFF)
{

}

GridDescriptor::~GridDescriptor()
{

}

void GridDescriptor::CreateComponent(Node *node, Engine *engine)
{
	SharedPtr<Mesh> mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateGrid(engine, size, countX, countZ), BoundingBox(-vec3(size.x, 1, size.y) * 0.5f, vec3(size.x, 1, size.y) * 0.5f), engine->GetResourceCache()->GetResource<Material>(materialFileName));
	auto staticModel = node->CreateComponent<StaticModel>(mesh);

	staticModel->SetCastShadow(castShadow);
	staticModel->SetLightMask(lightMask);
	component = staticModel;
}

ComponentDescriptor *GridDescriptor::Clone()
{
	GridDescriptor *clone = new GridDescriptor();
	clone->size = size;
	clone->countX = countX;
	clone->countZ = countZ;
	clone->castShadow = castShadow;
	clone->lightMask = lightMask;
	clone->collisionQueryMask = collisionQueryMask;
	clone->materialFileName = materialFileName;
	return clone;
}

void GridDescriptor::SetSize(vec2 size, FireCube::Engine *engine)
{
	this->size = size;
	Recreate(engine);
}

void GridDescriptor::SetCountX(unsigned int count, FireCube::Engine *engine)
{
	countX = count;
	Recreate(engine);
}
void GridDescriptor::SetCountZ(unsigned int count, FireCube::Engine *engine)
{
	countZ = count;
	Recreate(engine);
}

void GridDescriptor::Recreate(FireCube::Engine *engine)
{
	if (component)
	{
		auto staticModel = static_cast<StaticModel *>(component);
		SharedPtr<Mesh> mesh = new Mesh(engine);
		mesh->AddGeometry(GeometryGenerator::GenerateGrid(engine, size, countX, countZ), BoundingBox(-vec3(size.x, 1, size.y) * 0.5f, vec3(size.x, 1, size.y) * 0.5f), engine->GetResourceCache()->GetResource<Material>(materialFileName));
		staticModel->CreateFromMesh(mesh);
	}
}

vec2 GridDescriptor::GetSize() const
{
	return size;
}

unsigned int GridDescriptor::GetCountX() const
{
	return countX;
}

unsigned int GridDescriptor::GetCountZ() const
{
	return countZ;
}

bool GridDescriptor::GetCastShadow() const
{
	return castShadow;
}

void GridDescriptor::SetCastShadow(bool castShadow)
{
	this->castShadow = castShadow;
	if (component)
	{
		((StaticModel *)component)->SetCastShadow(castShadow);
	}
}

unsigned int GridDescriptor::GetLightMask() const
{
	return lightMask;
}

void GridDescriptor::SetLightMask(unsigned int lightMask)
{
	this->lightMask = lightMask;
	if (component)
	{
		((StaticModel *)component)->SetLightMask(lightMask);
	}
}

unsigned int GridDescriptor::GetCollisionQueryMask() const
{
	return collisionQueryMask;
}

void GridDescriptor::SetCollisionQueryMask(unsigned int collisionQueryMask)
{
	this->collisionQueryMask = collisionQueryMask;
}

void GridDescriptor::SetMaterialFileName(const std::string &materialFileName, FireCube::Engine *engine)
{
	this->materialFileName = materialFileName;
	if (component)
	{
		((StaticModel *)component)->SetMaterial(engine->GetResourceCache()->GetResource<Material>(materialFileName));
	}
}

std::string GridDescriptor::GetMaterialFileName()
{
	return materialFileName;
}