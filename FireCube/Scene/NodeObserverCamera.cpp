#include "Scene/NodeObserverCamera.h"
#include "Scene/Node.h"

using namespace FireCube;

NodeObserverCamera::NodeObserverCamera(Engine *engine) : Camera(engine)
{
	rotation.Set(0.0f, 0.0f, 0.0f);
	distance = 10.0f;
	minDistance = 0.1f;
	maxDistance = 100.0f;
	minAngX = -(float)PI / 2.0f + 0.001f;
	maxAngX = (float)PI / 2.0f - 0.001f;
	zoomFactor = 1.0f;
}

NodeObserverCamera::NodeObserverCamera(Engine *engine, InputManager &inputManager) : Camera(engine)
{
	rotation.Set(0.0f, 0.0f, 0.0f);
	distance = 10.0f;
	minDistance = 0.1f;
	maxDistance = 100.0f;
	minAngX = -(float)PI / 2.0f + 0.001f;
	maxAngX = (float)PI / 2.0f - 0.001f;
	zoomFactor = 1.0f;

	RegisterWithInputManager(inputManager);
}

void NodeObserverCamera::RegisterWithInputManager(InputManager &inputManager)
{
	inputManager.AddInputListener(this);
	inputManager.AddMapping(KEY_MOUSE_LEFT_BUTTON, STATE, "NodeObserverCamera_Rotate", MODIFIER_NONE);
	inputManager.AddMapping(KEY_MOUSE_MIDDLE_BUTTON, STATE, "NodeObserverCamera_Zoom", MODIFIER_NONE);
	inputManager.AddMapping(MOUSE_AXIS_X_RELATIVE, "NodeObserverCamera_MouseX");
	inputManager.AddMapping(MOUSE_AXIS_Y_RELATIVE, "NodeObserverCamera_MouseY");
	inputManager.AddMapping(MOUSE_WHEEL_Y_RELATIVE, "NodeObserverCamera_MouseWheelY");
}

mat4 NodeObserverCamera::GetViewMatrix()
{		
	if (!target)			
		return mat4::IDENTITY;
	vec3 targetPos = target->GetWorldPosition();

	if (lastTargetPosition == targetPos && !viewMatrixChanged)
		return viewMatrix;
	lastTargetPosition = targetPos;
	CheckRanges();
	vec3 dir;
	dir.FromAngles(rotation.x, rotation.y);
	dir*=-distance;
	position = targetPos + dir;	
	viewMatrix.LookAt(position, target->GetWorldPosition(), vec3(0.0f, 1.0f, 0.0f));	
	viewMatrixChanged = false;
	return viewMatrix;
}

void NodeObserverCamera::SetTarget(Node *node)
{
	target = node;
	viewMatrixChanged = true;
	frustumChanged = true;
}

Node *NodeObserverCamera::GetTarget() const
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

void NodeObserverCamera::SetMinDistance(float v)
{
	minDistance = v;
	CheckRanges();
}

float NodeObserverCamera::GetMinDistance() const
{
	return minDistance;
}

void NodeObserverCamera::SetMaxDistance(float v)
{
	maxDistance = v;
	CheckRanges();
}

float NodeObserverCamera::GetMaxDistance() const
{
	return maxDistance;
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
	if (distance < minDistance)
	{
		distance = minDistance;
		viewMatrixChanged = true;
		frustumChanged = true;
	}
	else if (distance > maxDistance)
	{
		distance = maxDistance;
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

void NodeObserverCamera::SetDistance(float v)
{
	distance = v;
	viewMatrixChanged = true;
	frustumChanged = true;
}

float NodeObserverCamera::GetDistance() const
{
	return distance;
}

void NodeObserverCamera::Zoom(float v)
{
	distance -= v;
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
	dir*=-distance;	
	return targetPos + dir;
}

void NodeObserverCamera::SetZoomFactor(float factor)
{
	zoomFactor = factor;
}

void NodeObserverCamera::HandleInput(float time, const MappedInput &input)
{
	if (input.HasValue("NodeObserverCamera_MouseWheelY"))
	{
		Zoom(time * input.GetValue("NodeObserverCamera_MouseWheelY") * zoomFactor);
	}
	if (input.IsStateOn("NodeObserverCamera_Rotate"))
	{
		RotateX(-input.GetValue("NodeObserverCamera_MouseY") * time);
		RotateY(-input.GetValue("NodeObserverCamera_MouseX") * time);
	}
	if (input.IsStateOn("NodeObserverCamera_Zoom"))
	{
		Zoom(-input.GetValue("NodeObserverCamera_MouseY") * time);		
	}
}
