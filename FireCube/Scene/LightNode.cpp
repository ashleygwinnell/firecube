#include "Rendering/RenderQueue.h"
#include "Scene/LightNode.h"

using namespace FireCube;

LightNode::LightNode() : Node()
{
	type = Node::LIGHT;
}

LightNode::LightNode(const std::string &name) : Node(name)
{
	type = Node::LIGHT;
}

Light &LightNode::GetLight()
{
	return light;
}

void LightNode::PopulateRenderQueue(RenderQueue &renderQueue, CameraPtr camera)
{
	renderQueue.AddLight(GetWorldTransformation(), light);

	Node::PopulateRenderQueue(renderQueue, camera);
}