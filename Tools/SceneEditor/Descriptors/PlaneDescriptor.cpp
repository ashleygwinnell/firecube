#include "FireCube.h"
#include "PlaneDescriptor.h"

using namespace FireCube;

PlaneDescriptor::PlaneDescriptor() : ComponentDescriptor(ComponentType::PLANE), collisionQueryMask(0xFFFFFFFF), castShadow(true), lightMask(0xFFFFFFFF)
{

}

PlaneDescriptor::~PlaneDescriptor()
{

}

void PlaneDescriptor::CreateComponent(Node *node, Engine *engine)
{
	SharedPtr<Mesh> mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GeneratePlane(engine, size), BoundingBox(-vec3(size.x, 1, size.y) * 0.5f, vec3(size.x, 1, size.y) * 0.5f), engine->GetResourceCache()->GetResource<Material>(materialFileName));
	auto staticModel = node->CreateComponent<StaticModel>(mesh);

	staticModel->SetCastShadow(castShadow);
	staticModel->SetLightMask(lightMask);
	component = staticModel;
}

ComponentDescriptor *PlaneDescriptor::Clone()
{
	PlaneDescriptor *clone = new PlaneDescriptor();
	clone->size = size;
	clone->castShadow = castShadow;
	clone->lightMask = lightMask;
	clone->collisionQueryMask = collisionQueryMask;
	return clone;
}

void PlaneDescriptor::SetSize(vec2 size, FireCube::Engine *engine)
{
	this->size = size;
	if (component)
	{
		auto staticModel = static_cast<StaticModel *>(component);
		SharedPtr<Mesh> mesh = new Mesh(engine);
		mesh->AddGeometry(GeometryGenerator::GeneratePlane(engine, size), BoundingBox(-vec3(size.x, 1, size.y) * 0.5f, vec3(size.x, 1, size.y) * 0.5f), engine->GetResourceCache()->GetResource<Material>(materialFileName));
		staticModel->CreateFromMesh(mesh);
	}
}

vec2 PlaneDescriptor::GetSize() const
{
	return size;
}

bool PlaneDescriptor::GetCastShadow() const
{
	return castShadow;
}

void PlaneDescriptor::SetCastShadow(bool castShadow)
{
	this->castShadow = castShadow;
	if (component)
	{
		((StaticModel *)component)->SetCastShadow(castShadow);
	}
}

unsigned int PlaneDescriptor::GetLightMask() const
{
	return lightMask;
}

void PlaneDescriptor::SetLightMask(unsigned int lightMask)
{
	this->lightMask = lightMask;
	if (component)
	{
		((StaticModel *)component)->SetLightMask(lightMask);
	}
}

unsigned int PlaneDescriptor::GetCollisionQueryMask() const
{
	return collisionQueryMask;
}

void PlaneDescriptor::SetCollisionQueryMask(unsigned int collisionQueryMask)
{
	this->collisionQueryMask = collisionQueryMask;
}

void PlaneDescriptor::SetMaterialFileName(const std::string &materialFileName, FireCube::Engine *engine)
{
	this->materialFileName = materialFileName;
	if (component)
	{
		((StaticModel *)component)->SetMaterial(engine->GetResourceCache()->GetResource<Material>(materialFileName));
	}
}

std::string PlaneDescriptor::GetMaterialFileName()
{
	return materialFileName;
}