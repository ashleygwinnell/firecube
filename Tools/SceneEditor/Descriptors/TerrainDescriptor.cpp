#include "FireCube.h"
#include "TerrainDescriptor.h"

using namespace FireCube;

TerrainDescriptor::TerrainDescriptor() : ComponentDescriptor(ComponentType::TERRAIN), collisionQueryMask(0xFFFFFFFF), castShadow(true), lightMask(0xFFFFFFFF),
verticesSpacing(1.0f, 64.0f, 1.0f), generateHardNormals(false)
{

}

void TerrainDescriptor::CreateComponent(Node *node, Engine *engine)
{
	auto terrain = node->CreateComponent<Terrain>();
	if (materialFileName.empty() == false)
	{
		terrain->SetMaterial(engine->GetResourceCache()->GetResource<Material>(materialFileName));
	}

	terrain->SetVerticesSpacing(verticesSpacing);
	terrain->SetGenerateHardNormals(generateHardNormals);

	if (heightmapFilename.empty() == false)
	{
		terrain->CreateFromHeightMap(engine->GetResourceCache()->GetResource<FireCube::Image>(heightmapFilename));
	}

	terrain->SetCastShadow(castShadow);
	terrain->SetLightMask(lightMask);
	component = terrain;
}

ComponentDescriptor *TerrainDescriptor::Clone()
{
	TerrainDescriptor *clone = new TerrainDescriptor();
	clone->heightmapFilename = heightmapFilename;
	clone->castShadow = castShadow;
	clone->lightMask = lightMask;
	clone->collisionQueryMask = collisionQueryMask;
	return clone;
}

TerrainDescriptor::~TerrainDescriptor()
{

}

void TerrainDescriptor::SetHeightmapFilename(const std::string &filename, Engine *engine)
{
	this->heightmapFilename = filename;
	if (component)
	{
		((Terrain *)component)->CreateFromHeightMap(engine->GetResourceCache()->GetResource<FireCube::Image>(filename));
	}
}

std::string TerrainDescriptor::GetHeightmapFilename() const
{
	return heightmapFilename;
}

bool TerrainDescriptor::GetCastShadow() const
{
	return castShadow;
}

void TerrainDescriptor::SetCastShadow(bool castShadow)
{
	this->castShadow = castShadow;
	if (component)
	{
		((Terrain *)component)->SetCastShadow(castShadow);
	}
}

unsigned int TerrainDescriptor::GetLightMask() const
{
	return lightMask;
}

void TerrainDescriptor::SetLightMask(unsigned int lightMask)
{
	this->lightMask = lightMask;
	if (component)
	{
		((Terrain *)component)->SetLightMask(lightMask);
	}
}

unsigned int TerrainDescriptor::GetCollisionQueryMask() const
{
	return collisionQueryMask;
}

void TerrainDescriptor::SetCollisionQueryMask(unsigned int collisionQueryMask)
{
	this->collisionQueryMask = collisionQueryMask;
}

void TerrainDescriptor::SetMaterialFileName(const std::string &materialFileName, FireCube::Engine *engine)
{
	this->materialFileName = materialFileName;
	if (component)
	{
		((Terrain *)component)->SetMaterial(engine->GetResourceCache()->GetResource<Material>(materialFileName));
	}
}

std::string TerrainDescriptor::GetMaterialFileName()
{
	return materialFileName;
}

void TerrainDescriptor::SetVerticesSpacing(vec3 spacing, Engine *engine)
{
	this->verticesSpacing = spacing;

	if (heightmapFilename.empty() == false && component)
	{
		((Terrain *) component)->SetVerticesSpacing(spacing);
		((Terrain *) component)->CreateFromHeightMap(engine->GetResourceCache()->GetResource<FireCube::Image>(heightmapFilename));
	}
}

FireCube::vec3 TerrainDescriptor::GetVerticesSpacing() const
{
	return verticesSpacing;
}

void TerrainDescriptor::SetGenerateHardNormals(bool hardNormals, Engine *engine)
{
	this->generateHardNormals = hardNormals;

	if (heightmapFilename.empty() == false && component)
	{
		((Terrain *)component)->SetGenerateHardNormals(hardNormals);
		((Terrain *)component)->CreateFromHeightMap(engine->GetResourceCache()->GetResource<FireCube::Image>(heightmapFilename));
	}
}

bool TerrainDescriptor::GetGenerateHardNormals() const
{
	return generateHardNormals;
}
