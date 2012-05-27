#ifndef FIRECUBE_H
#define FIRECUBE_H

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <iostream>
#include <queue>
#include <set>
#include <windows.h>
#include <gl/gl.h>

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "Utils/ResourcePool.h"
#include "Utils/Timer.h"
#include "Math/MyMath.h"
#include "Math/BoundingBox.h"
#include "Rendering/Texture.h"
#include "Rendering/Buffer.h"
#include "Rendering/Shaders.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "Rendering/FrameBuffer.h"
#include "Utils/Image.h"
#include "Rendering/Font.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderQueue.h"
#include "Application/Input.h"
#include "Application/Application.h"
#include "Scene/Light.h"
#include "Scene/Node.h"
#include "Scene/GeometryNode.h"
#include "Scene/LightNode.h"
#include "Geometry/Terrain.h"
#include "Scene/TerrainNode.h"
#include "Geometry/GeometryGenerator.h"
#include "Math/Plane.h"
#include "Math/Frustum.h"
#include "Scene/Camera.h"
#include "Scene/NodeObserverCamera.h"

#endif