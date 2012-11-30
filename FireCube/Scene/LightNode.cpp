#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
#include <memory>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include "Dependencies/glew.h"

#include "Utils/utils.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Geometry/Geometry.h"
#include "Rendering/Renderer.h"
#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Rendering/RenderQueue.h"
#include "Scene/LightNode.h"

using namespace FireCube;

LightNode::LightNode() : Node()
{
	type = Node::LIGHT;
}

LightNode::LightNode(const string &name) : Node(name)
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