#include "Skybox.h"
#include "Geometry/GeometryGenerator.h"

using namespace FireCube;

Skybox::Skybox(Engine *engine) : Renderable(engine)
{
	renderableParts.resize(1);
	renderableParts[0].geometry = GeometryGenerator::GenerateBox(engine, vec3(2));
	renderableParts[0].material = nullptr;
	SetCastShadow(false);
	SetCollisionQueryMask(0);
}

Skybox::Skybox(const Skybox &other) : Renderable(other)
{
	renderableParts = other.renderableParts;
	SetCastShadow(false);
	SetCollisionQueryMask(0);
}

void Skybox::UpdateWorldBoundingBox()
{
	worldBoundingBox = BoundingBox(vec3(-1e6), vec3(1e6));
}

void Skybox::SetMaterial(Material *material)
{
	renderableParts[0].material = material;
}

Component *Skybox::Clone() const
{
	Skybox *clone = new Skybox(*this);
	return clone;
}
