#include "FireCube.h"
#include "SphereDescriptor.h"

using namespace FireCube;

SphereDescriptor::SphereDescriptor() : ComponentDescriptor(ComponentType::SPHERE), collisionQueryMask(0xFFFFFFFF), castShadow(true), lightMask(0xFFFFFFFF)
{

}

SphereDescriptor::~SphereDescriptor()
{

}

void SphereDescriptor::CreateComponent(Node *node, Engine *engine)
{
	SharedPtr<Mesh> mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateSphere(engine, radius, rings, columns), BoundingBox(-vec3(radius), vec3(radius)), engine->GetResourceCache()->GetResource<Material>(materialFileName));
	auto staticModel = node->CreateComponent<StaticModel>(mesh);

	staticModel->SetCastShadow(castShadow);
	staticModel->SetLightMask(lightMask);
	component = staticModel;
}

ComponentDescriptor *SphereDescriptor::Clone()
{
	SphereDescriptor *clone = new SphereDescriptor();
	clone->radius = radius;
	clone->rings = rings;
	clone->columns = columns;
	clone->castShadow = castShadow;
	clone->lightMask = lightMask;
	clone->collisionQueryMask = collisionQueryMask;
	return clone;
}

void SphereDescriptor::SetRadius(float radius, FireCube::Engine *engine)
{
	this->radius = radius;
	if (component)
	{
		auto staticModel = static_cast<StaticModel *>(component);
		SharedPtr<Mesh> mesh = new Mesh(engine);
		mesh->AddGeometry(GeometryGenerator::GenerateSphere(engine, radius, rings, columns), BoundingBox(-vec3(radius), vec3(radius)), engine->GetResourceCache()->GetResource<Material>(materialFileName));
		staticModel->CreateFromMesh(mesh);
	}
}

float SphereDescriptor::GetRadius() const
{
	return radius;
}

void SphereDescriptor::SetColumns(unsigned int columns, FireCube::Engine *engine)
{
	this->columns = columns;
	if (component)
	{
		auto staticModel = static_cast<StaticModel *>(component);
		SharedPtr<Mesh> mesh = new Mesh(engine);
		mesh->AddGeometry(GeometryGenerator::GenerateSphere(engine, radius, rings, columns), BoundingBox(-vec3(radius), vec3(radius)), engine->GetResourceCache()->GetResource<Material>(materialFileName));
		staticModel->CreateFromMesh(mesh);
	}
}

unsigned int SphereDescriptor::GetColumns() const
{
	return columns;
}

void SphereDescriptor::SetRings(unsigned int rings, FireCube::Engine *engine)
{
	this->rings = rings;
	if (component)
	{
		auto staticModel = static_cast<StaticModel *>(component);
		SharedPtr<Mesh> mesh = new Mesh(engine);
		mesh->AddGeometry(GeometryGenerator::GenerateSphere(engine, radius, rings, columns), BoundingBox(-vec3(radius), vec3(radius)), engine->GetResourceCache()->GetResource<Material>(materialFileName));
		staticModel->CreateFromMesh(mesh);
	}
}

unsigned int SphereDescriptor::GetRings() const
{
	return rings;
}

bool SphereDescriptor::GetCastShadow() const
{
	return castShadow;
}

void SphereDescriptor::SetCastShadow(bool castShadow)
{
	this->castShadow = castShadow;
	if (component)
	{
		((StaticModel *)component)->SetCastShadow(castShadow);
	}
}

unsigned int SphereDescriptor::GetLightMask() const
{
	return lightMask;
}

void SphereDescriptor::SetLightMask(unsigned int lightMask)
{
	this->lightMask = lightMask;
	if (component)
	{
		((StaticModel *)component)->SetLightMask(lightMask);
	}
}

unsigned int SphereDescriptor::GetCollisionQueryMask() const
{
	return collisionQueryMask;
}

void SphereDescriptor::SetCollisionQueryMask(unsigned int collisionQueryMask)
{
	this->collisionQueryMask = collisionQueryMask;
}

void SphereDescriptor::SetMaterialFileName(const std::string &materialFileName, FireCube::Engine *engine)
{
	this->materialFileName = materialFileName;
	if (component)
	{
		((StaticModel *)component)->SetMaterial(engine->GetResourceCache()->GetResource<Material>(materialFileName));
	}
}

std::string SphereDescriptor::GetMaterialFileName()
{
	return materialFileName;
}