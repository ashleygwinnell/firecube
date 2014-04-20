#include "Scene/Camera.h"
#include "Scene/Node.h"

using namespace FireCube;

Camera::Camera(Engine *engine) : Component(engine), viewMatrixChanged(true), frustumChanged(true), projectionMatrixChanged(true), aspectRatio(1.0f), fov(60.0f), 
								 nearPlane(0.1f), farPlane(500.0f)
{

}

mat4 Camera::GetProjectionMatrix()
{
	if (projectionMatrixChanged)
	{
		projectionMatrixChanged = false;
		projectionMatrix.GeneratePerspective(fov, aspectRatio, nearPlane, farPlane);
	}
	return projectionMatrix;
}

mat4 Camera::GetViewMatrix()
{
	if (!viewMatrixChanged)
		return viewMatrix;
	
	if (node)
	{
		viewMatrix = node->GetWorldTransformation();
		viewMatrix.Inverse();
	}
	else
		viewMatrix = mat4::IDENTITY;

	viewMatrixChanged = false;
	return viewMatrix;
}

void Camera::UpdateFrustum()
{
	frustum.Extract(GetViewMatrix(), GetProjectionMatrix());	
}

Frustum &Camera::GetFrustum()
{	
	if (frustumChanged)
	{
		UpdateFrustum();
		frustumChanged = false;
	}
	return frustum;
}

void Camera::LookAt(const vec3 &target, const vec3 &up)
{
	/*mat4 t;	
	t.LookAt(position, target, up);
	rotation = t.ExtractEulerAngles() * -1.0f;
	viewMatrixChanged = true;
	frustumChanged = true;*/
}

void Camera::MarkedDirty()
{
	viewMatrixChanged = true;
	frustumChanged = true;
}

void Camera::NodeChanged()
{
	viewMatrixChanged = true;
	frustumChanged = true;
}

void Camera::SetFOV(float fov)
{
	this->fov = fov;
	projectionMatrixChanged = true;
	frustumChanged = true;
}

void Camera::SetNearPlane(float nearPlane)
{
	this->nearPlane = nearPlane;
	projectionMatrixChanged = true;
	frustumChanged = true;
}

void Camera::SetFarPlane(float farPlane)
{
	this->farPlane = farPlane;
	projectionMatrixChanged = true;
	frustumChanged = true;
}

void Camera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	projectionMatrixChanged = true;
	frustumChanged = true;
}

void Camera::SetPerspectiveProjectionParameters(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	this->fov = fov;
	this->aspectRatio = aspectRatio;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;	
	projectionMatrixChanged = true;
	frustumChanged = true;
}