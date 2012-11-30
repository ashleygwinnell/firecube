#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

class FIRECUBE_API GeometryNode : public Node
{
public:
	GeometryNode();	

	/**
	* Constructs a light node.
	* @param name The name of this node.
	*/
	GeometryNode(const std::string &name);

	/**
	* Sets the geometry of this node.
	* @param geometry The geometry to set.
	*/
	void SetGeometry(GeometryPtr geometry);

	/**
	* Gets the geometry of this node.
	* @return The geometry of this node.
	*/	
	GeometryPtr GetGeometry();

	virtual NodePtr Clone() const;
protected:
	virtual BoundingBox GetLocalBoundingBox() const;
	virtual void PopulateRenderQueue(RenderQueue &renderQueue, CameraPtr camera);		
	
private:
	GeometryPtr geometry;
};

typedef std::shared_ptr<GeometryNode> GeometryNodePtr;

}