#pragma once

namespace FireCube
{

class FIRECUBE_API TerrainNode : public Node
{
public:
	TerrainNode();	
	Terrain &GetTerrain();
	MaterialPtr GetMaterial();
protected:
	virtual void PopulateRenderQueue(RenderQueue &renderQueue, CameraPtr camera);
	virtual BoundingBox GetLocalBoundingBox() const;
private:
	Terrain terrain;	
};

typedef std::shared_ptr<TerrainNode> TerrainNodePtr;

}