#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

class FIRECUBE_API LightNode : public Node
{
public:
	LightNode();	

	/**
	* Constructs a light node.
	* @param name The name of this node.
	*/
	LightNode(const std::string &name);

	Light &GetLight();
protected:
	virtual void PopulateRenderQueue(RenderQueue &renderQueue, CameraPtr camera);
private:
	Light light;
};

typedef std::shared_ptr<LightNode> LightNodePtr;

}