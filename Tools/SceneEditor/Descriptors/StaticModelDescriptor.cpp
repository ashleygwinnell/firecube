#include "FireCube.h"
#include "StaticModelDescriptor.h"

using namespace FireCube;

StaticModelDescriptor::StaticModelDescriptor() : ComponentDescriptor(ComponentType::STATIC_MODEL), collisionQueryMask(0xFFFFFFFF), castShadow(true), lightMask(0xFFFFFFFF)
{

}

void StaticModelDescriptor::CreateComponent(Node *node, Engine *engine)
{
	auto staticModel = node->CreateComponent<StaticModel>(engine->GetResourceCache()->GetResource<Mesh>(meshFilename));
	staticModel->SetCastShadow(castShadow);
	staticModel->SetLightMask(lightMask);
	component = staticModel;
}

ComponentDescriptor *StaticModelDescriptor::Clone()
{
	StaticModelDescriptor *clone = new StaticModelDescriptor();
	clone->meshFilename = meshFilename;
	clone->castShadow = castShadow;
	clone->lightMask = lightMask;
	clone->collisionQueryMask = collisionQueryMask;
	return clone;
}

StaticModelDescriptor::~StaticModelDescriptor()
{

}

void StaticModelDescriptor::SetMeshFilename(const std::string &meshFilename, Engine *engine)
{
	this->meshFilename = meshFilename;
	if (component)
	{
		((StaticModel *) component)->CreateFromMesh(engine->GetResourceCache()->GetResource<FireCube::Mesh>(meshFilename));
	}
}

std::string StaticModelDescriptor::GetMeshFilename() const
{
	return meshFilename;
}

bool StaticModelDescriptor::GetCastShadow() const
{
	return castShadow;
}

void StaticModelDescriptor::SetCastShadow(bool castShadow)
{
	this->castShadow = castShadow;
	if (component)
	{
		((StaticModel *)component)->SetCastShadow(castShadow);
	}
}

unsigned int StaticModelDescriptor::GetLightMask() const
{
	return lightMask;
}

void StaticModelDescriptor::SetLightMask(unsigned int lightMask)
{
	this->lightMask = lightMask;
	if (component)
	{
		((StaticModel *)component)->SetLightMask(lightMask);
	}
}

unsigned int StaticModelDescriptor::GetCollisionQueryMask() const
{
	return collisionQueryMask;
}

void StaticModelDescriptor::SetCollisionQueryMask(unsigned int collisionQueryMask)
{
	this->collisionQueryMask = collisionQueryMask;
	if (component)
	{
		((StaticModel *)component)->SetCollisionQueryMask(collisionQueryMask);
	}
}
