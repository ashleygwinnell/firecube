#include "CameraDescriptor.h"

using namespace FireCube;

CameraDescriptor::CameraDescriptor() : ComponentDescriptor(ComponentType::CAMERA)
{

}

void CameraDescriptor::CreateComponent(FireCube::Node *node, FireCube::Engine *engine)
{
	auto camera = node->CreateComponent<Camera>();

	if (orthographic)
	{
		camera->SetOrthographicProjectionParameters(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
	}
	else
	{
		camera->SetPerspectiveProjectionParameters(fov, 1.0f, nearPlane, farPlane);
	}

	component = camera;
}

ComponentDescriptor *CameraDescriptor::Clone()
{
	CameraDescriptor *clone = new CameraDescriptor();

	clone->orthographic = orthographic;
	clone->leftPlane = leftPlane;
	clone->rightPlane = rightPlane;
	clone->bottomPlane = bottomPlane;
	clone->topPlane = topPlane;
	clone->nearPlane = nearPlane;
	clone->farPlane = farPlane;
	clone->fov = fov;

	return clone;
}

CameraDescriptor::~CameraDescriptor()
{

}

void CameraDescriptor::SetOrthographic(bool orthographic)
{
	this->orthographic = orthographic;
	if (component)
	{
		Camera *camera = (Camera *)component;
		if (orthographic)
		{
			camera->SetOrthographicProjectionParameters(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
		}
		else
		{
			camera->SetPerspectiveProjectionParameters(fov, 1.0f, nearPlane, farPlane);
		}
	}
}

bool CameraDescriptor::GetOrthographic() const
{
	return orthographic;
}

void CameraDescriptor::SetFOV(float fov)
{
	this->fov = fov;
	if (component)
	{
		Camera *camera = (Camera *)component;
		camera->SetFOV(fov);
	}
}

float CameraDescriptor::GetFOV() const
{
	return fov;
}

void CameraDescriptor::SetNearPlane(float nearPlane)
{
	this->nearPlane = nearPlane;
	if (component)
	{
		Camera *camera = (Camera *)component;
		camera->SetNearPlane(nearPlane);
	}
}

float CameraDescriptor::GetNearPlane() const
{
	return nearPlane;
}

void CameraDescriptor::SetFarPlane(float farPlane)
{
	this->farPlane = farPlane;
	if (component)
	{
		Camera *camera = (Camera *)component;
		camera->SetFarPlane(farPlane);
	}
}

float CameraDescriptor::GetFarPlane() const
{
	return farPlane;
}

void CameraDescriptor::SetLeftPlane(float leftPlane)
{
	this->leftPlane = leftPlane;
	if (component)
	{
		Camera *camera = (Camera *)component;
		camera->SetOrthographicProjectionParameters(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
	}
}

float CameraDescriptor::GetLeftPlane() const
{
	return leftPlane;
}

void CameraDescriptor::SetRightPlane(float rightPlane)
{
	this->rightPlane = rightPlane;
	if (component)
	{
		Camera *camera = (Camera *)component;
		camera->SetOrthographicProjectionParameters(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
	}
}

float CameraDescriptor::GetRightPlane() const
{
	return rightPlane;
}

void CameraDescriptor::SetTopPlane(float topPlane)
{
	this->topPlane = topPlane;
	if (component)
	{
		Camera *camera = (Camera *)component;
		camera->SetOrthographicProjectionParameters(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
	}
}

float CameraDescriptor::GetTopPlane() const
{
	return topPlane;
}

void CameraDescriptor::SetBottomPlane(float bottomPlane)
{
	this->bottomPlane = bottomPlane;
	if (component)
	{
		Camera *camera = (Camera *)component;
		camera->SetOrthographicProjectionParameters(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
	}
}

float CameraDescriptor::GetBottomPlane() const
{
	return bottomPlane;
}

