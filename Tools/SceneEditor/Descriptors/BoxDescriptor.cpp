#include "FireCube.h"
#include "BoxDescriptor.h"

using namespace FireCube;

BoxDescriptor::BoxDescriptor() : ComponentDescriptor(ComponentType::BOX), collisionQueryMask(0xFFFFFFFF), castShadow(true), lightMask(0xFFFFFFFF)
{

}

BoxDescriptor::~BoxDescriptor()
{

}

void BoxDescriptor::CreateComponent(Node *node, Engine *engine)
{	
	SharedPtr<Mesh> mesh = new Mesh(engine);
	mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, size), BoundingBox(-size * 0.5f, size * 0.5f), engine->GetResourceCache()->GetResource<Material>("Materials/Default.xml"));
	auto staticModel = node->CreateComponent<StaticModel>(mesh);
	
	staticModel->SetCastShadow(castShadow);
	staticModel->SetLightMask(lightMask);
	component = staticModel;
}

ComponentDescriptor *BoxDescriptor::Clone()
{
	BoxDescriptor *clone = new BoxDescriptor();
	clone->size = size;
	clone->castShadow = castShadow;
	clone->lightMask = lightMask;
	clone->collisionQueryMask = collisionQueryMask;
	return clone;
}

void BoxDescriptor::SetSize(vec3 size, FireCube::Engine *engine)
{
	this->size = size;
	if (component) 
	{
		auto staticModel = static_cast<StaticModel *>(component);
		SharedPtr<Mesh> mesh = new Mesh(engine);		
		mesh->AddGeometry(GeometryGenerator::GenerateBox(engine, size), BoundingBox(-size * 0.5f, size * 0.5f), engine->GetResourceCache()->GetResource<Material>("Materials/Default.xml"));
		staticModel->CreateFromMesh(mesh);
	}
}

vec3 BoxDescriptor::GetSize() const
{
	return size;
}

bool BoxDescriptor::GetCastShadow() const
{
	return castShadow;
}

void BoxDescriptor::SetCastShadow(bool castShadow)
{
	this->castShadow = castShadow;
	if (component)
	{
		((StaticModel *)component)->SetCastShadow(castShadow);
	}
}

unsigned int BoxDescriptor::GetLightMask() const
{
	return lightMask;
}

void BoxDescriptor::SetLightMask(unsigned int lightMask)
{
	this->lightMask = lightMask;
	if (component)
	{
		((StaticModel *)component)->SetLightMask(lightMask);
	}
}

unsigned int BoxDescriptor::GetCollisionQueryMask() const
{
	return collisionQueryMask;
}

void BoxDescriptor::SetCollisionQueryMask(unsigned int collisionQueryMask)
{
	this->collisionQueryMask = collisionQueryMask;	
}

void BoxDescriptor::SetMaterialFileName(const std::string &materialFileName, FireCube::Engine *engine)
{
	this->materialFileName = materialFileName;
	if (component)
	{
		((StaticModel *)component)->SetMaterial(engine->GetResourceCache()->GetResource<Material>(materialFileName));
	}
}

std::string BoxDescriptor::GetMaterialFileName()
{
	return materialFileName;
}