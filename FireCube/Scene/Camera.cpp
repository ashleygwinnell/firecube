#include "Scene/Camera.h"
#include "Scene/Node.h"

using namespace FireCube;

Camera::Camera(Engine *engine) : Component(engine), viewMatrixChanged(true), frustumChanged(true)
{

}

void Camera::SetProjectionMatrix(const mat4 &mat)
{
	projectionMatrix = mat;
	frustumChanged = true;
}

mat4 Camera::GetProjectionMatrix() const
{
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
