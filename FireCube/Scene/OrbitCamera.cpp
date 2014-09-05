#include "Scene/OrbitCamera.h"
#include "Scene/Node.h"

using namespace FireCube;

OrbitCamera::OrbitCamera(Engine *engine) : Camera(engine)
{
	rotation.Set(0.0f, 0.0f, 0.0f);
	distance = 10.0f;
	minDistance = 0.1f;
	maxDistance = 100.0f;
	minAngX = -(float)PI / 2.0f + 0.001f;
	maxAngX = (float)PI / 2.0f - 0.001f;
	zoomFactor = 1.0f;
}

OrbitCamera::OrbitCamera(Engine *engine, InputManager &inputManager) : Camera(engine)
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

void OrbitCamera::RegisterWithInputManager(InputManager &inputManager)
{
	inputManager.AddInputListener(this);
	inputManager.AddMapping(Key::MOUSE_LEFT_BUTTON, InputMappingType::STATE, "NodeObserverCamera_Rotate", KeyModifier::NONE);
	inputManager.AddMapping(Key::MOUSE_MIDDLE_BUTTON, InputMappingType::STATE, "NodeObserverCamera_Zoom", KeyModifier::NONE);
	inputManager.AddMapping(AnalogInput::MOUSE_AXIS_X_RELATIVE, "NodeObserverCamera_MouseX");
	inputManager.AddMapping(AnalogInput::MOUSE_AXIS_Y_RELATIVE, "NodeObserverCamera_MouseY");
	inputManager.AddMapping(AnalogInput::MOUSE_WHEEL_Y_RELATIVE, "NodeObserverCamera_MouseWheelY");
}

void OrbitCamera::SetMinDistance(float v)
{
	minDistance = v;
	UpdateNode();
}

float OrbitCamera::GetMinDistance() const
{
	return minDistance;
}

void OrbitCamera::SetMaxDistance(float v)
{
	maxDistance = v;
	UpdateNode();
}

float OrbitCamera::GetMaxDistance() const
{
	return maxDistance;
}

void OrbitCamera::SetMinAngX(float v)
{
	minAngX = v;
	UpdateNode();
}

float OrbitCamera::GetMinAngX() const
{
	return minAngX;
}

void OrbitCamera::SetMaxAngX(float v)
{
	maxAngX = v;
	UpdateNode();
}

float OrbitCamera::GetMaxAngX() const
{
	return maxAngX;
}

void OrbitCamera::CheckRanges()
{
	if (distance < minDistance)
	{
		distance = minDistance;		
	}
	else if (distance > maxDistance)
	{
		distance = maxDistance;		
	}

	if (rotation.x < minAngX)
	{
		rotation.x = minAngX;		
	}
	else if (rotation.x > maxAngX)
	{
		rotation.x = maxAngX;		
	}
}

void OrbitCamera::SetDistance(float v)
{
	distance = v;
	UpdateNode();
}

float OrbitCamera::GetDistance() const
{
	return distance;
}

void OrbitCamera::Zoom(float v)
{
	distance -= v;
	UpdateNode();
}

void OrbitCamera::RotateX(float v)
{
	rotation.x += v;
	UpdateNode();
}

void OrbitCamera::RotateY(float v)
{
	rotation.y += v;
	UpdateNode();
}

void OrbitCamera::SetZoomFactor(float factor)
{
	zoomFactor = factor;
}

void OrbitCamera::SetRotation(vec3 rotation)
{
	this->rotation = rotation;
	UpdateNode();
}

void OrbitCamera::HandleInput(float time, const MappedInput &input)
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

	UpdateNode();
}

void OrbitCamera::UpdateNode()
{
	CheckRanges();
	vec3 dir;
	dir.FromAngles(rotation.x, rotation.y);
	dir *= -distance;	
	node->LookAt(dir, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));	
	viewMatrixChanged = true;
	frustumChanged = true;
}