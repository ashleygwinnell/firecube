#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include "Dependencies/glew.h"

#include "Utils/utils.h"
//#include "Utils/Logger.h"
//#include "Utils/Filesystem.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Rendering/Renderer.h"
#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Rendering/RenderQueue.h"
//#include "Dependencies/tinyxml.h"
//#include "Geometry/m3dsLoader.h"
//#include "Geometry/ObjLoader.h"
//#include "Geometry/ColladaLoader.h"
//#include "Rendering/Buffer.h"
//#include "Rendering/Shaders.h"
//#include "Math/Plane.h"
//#include "Math/Frustum.h"
//#include "Scene/Camera.h"
#include "Scene/LightNode.h"

using namespace FireCube;

LightNode::LightNode() : Node()
{

}

LightNode::LightNode(const string &name) : Node(name)
{

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