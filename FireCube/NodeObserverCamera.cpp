#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <sstream>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include "GLee.h"

#include "utils.h"
#include "Logger.h"
#include "MyMath.h"
#include "Plane.h"
#include "Frustum.h"
#include "Camera.h"
#include "NodeObserverCamera.h"
#include "BoundingBox.h"
#include "Renderer.h"
#include "Buffer.h"
#include "Light.h"
#include "Node.h"

using namespace FireCube;

NodeObserverCamera::NodeObserverCamera()
{
	rotation.Set(0.0f, 0.0f, 0.0f);
	zoom = 10.0f;
	minZoom = 0.1f;
	maxZoom = 100.0f;
	minAngX = -(float)PI / 2.0f + 0.001f;
	maxAngX = (float)PI / 2.0f - 0.001f;
}
mat4 NodeObserverCamera::GetViewMatrix()
{		
	if (!target)			
		return mat4::identity;
	vec3 targetPos = target->GetWorldPosition();

	if (lastTargetPosition == targetPos && !viewMatrixChanged)
		return viewMatrix;
	lastTargetPosition = targetPos;
	CheckRanges();
	vec3 dir;
	dir.FromAngles(rotation.x, rotation.y);
	dir*=-zoom;
	position = targetPos + dir;
	mat4 t;
	viewMatrix.LookAt(position, target->GetWorldPosition(), vec3(0.0f, 1.0f, 0.0f));	
	viewMatrixChanged = false;
	return viewMatrix;
}
void NodeObserverCamera::SetTarget(NodePtr node)
{
	target = node;
	viewMatrixChanged = true;
}
NodePtr NodeObserverCamera::GetTarget() const
{
	return target;
}
Frustum &NodeObserverCamera::GetFrustum()
{
	vec3 targetPos = target->GetWorldPosition();
	if (lastTargetPosition != targetPos)
		frustumChanged = true;
	UpdateFrustum();
	return frustum;
}
void NodeObserverCamera::SetMinZoom(float v)
{
	minZoom = v;
	CheckRanges();
}
float NodeObserverCamera::GetMinZoom() const
{
	return minZoom;
}
void NodeObserverCamera::SetMaxZoom(float v)
{
	maxZoom = v;
	CheckRanges();
}
float NodeObserverCamera::GetMaxZoom() const
{
	return maxZoom;
}
void NodeObserverCamera::SetMinAngX(float v)
{
	minAngX = v;
	CheckRanges();
}
float NodeObserverCamera::GetMinAngX() const
{
	return minAngX;
}
void NodeObserverCamera::SetMaxAngX(float v)
{
	maxAngX = v;
	CheckRanges();
}
float NodeObserverCamera::GetMaxAngX() const
{
	return maxAngX;
}
void NodeObserverCamera::CheckRanges()
{
	if (zoom < minZoom)
	{
		zoom = minZoom;
		viewMatrixChanged = true;
		frustumChanged = true;
	}
	else if (zoom > maxZoom)
	{
		zoom = maxZoom;
		viewMatrixChanged = true;
		frustumChanged = true;
	}

	if (rotation.x < minAngX)
	{
		rotation.x = minAngX;
		viewMatrixChanged = true;
		frustumChanged = true;
	}
	else if (rotation.x > maxAngX)
	{
		rotation.x = maxAngX;
		viewMatrixChanged = true;
		frustumChanged = true;
	}

}
void NodeObserverCamera::SetZoom(float v)
{
	zoom = v;
	viewMatrixChanged = true;
	frustumChanged = true;
}
float NodeObserverCamera::GetZoom() const
{
	return zoom;
}
void NodeObserverCamera::Zoom(float v)
{
	zoom -= v;
	viewMatrixChanged = true;
	frustumChanged = true;
}
void NodeObserverCamera::RotateX(float v)
{
	rotation.x += v;
	viewMatrixChanged = true;
	frustumChanged = true;
}
void NodeObserverCamera::RotateY(float v)
{
	rotation.y += v;
	viewMatrixChanged = true;
	frustumChanged = true;
}
vec3 NodeObserverCamera::GetPosition() const
{
	if (!target)			
		return vec3(0.0f, 0.0f, 0.0f);
	vec3 targetPos = target->GetWorldPosition();	
	vec3 dir;
	dir.FromAngles(rotation.x, rotation.y);
	dir*=-zoom;	
	return targetPos + dir;
}
