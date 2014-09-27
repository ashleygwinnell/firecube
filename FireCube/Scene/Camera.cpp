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
	if (this->fov != fov)
	{
		this->fov = fov;
		projectionMatrixChanged = true;
		frustumChanged = true;
	}
}

void Camera::SetNearPlane(float nearPlane)
{
	if (this->nearPlane != nearPlane)
	{
		this->nearPlane = nearPlane;
		projectionMatrixChanged = true;
		frustumChanged = true;
	}
}

void Camera::SetFarPlane(float farPlane)
{
	if (this->farPlane != farPlane)
	{
		this->farPlane = farPlane;
		projectionMatrixChanged = true;
		frustumChanged = true;
	}
}

void Camera::SetAspectRatio(float aspectRatio)
{
	if (this->aspectRatio != aspectRatio)
	{
		this->aspectRatio = aspectRatio;
		projectionMatrixChanged = true;
		frustumChanged = true;
	}
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

vec3 Camera::Unproject(vec3 pos)
{
	mat4 inverse = GetProjectionMatrix() * GetViewMatrix();
	inverse.Inverse();
	vec4 tmp = vec4(pos, 1.0f);	

	vec4 obj = inverse * tmp;
	obj /= obj.w;
	return obj.ToVec3();
}

Ray Camera::GetPickingRay(vec2 pos, float width, float height)
{
	vec3 ndcPos(pos.x / width, pos.y / height, 0.5f);
	ndcPos = ndcPos * 2.0f - 1.0f;
	vec3 worldPos = Unproject(ndcPos);
	vec3 cameraPos = node->GetWorldPosition();
	return Ray(cameraPos, (worldPos - cameraPos).Normalized());
}