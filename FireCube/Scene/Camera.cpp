#include "Scene/Camera.h"

using namespace FireCube;

Camera::Camera() : viewMatrixChanged(true), frustumChanged(true), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f)
{

}

void Camera::SetPosition(const vec3 &position)
{
	this->position = position;
	viewMatrixChanged = true;
	frustumChanged = true;
}

vec3 Camera::GetPosition() const
{
	return position;
}

void Camera::SetRotation(const vec3 &rotation)
{
	this->rotation = rotation;
	viewMatrixChanged = true;
	frustumChanged = true;
}

vec3 Camera::GetRotation() const
{
	return rotation;
}

void Camera::Move(const vec3 &translation)
{
	position += translation;
	viewMatrixChanged = true;
	frustumChanged = true;
}

void Camera::Rotate(const vec3 &rotation)
{
	this->rotation += rotation;
	viewMatrixChanged = true;
	frustumChanged = true;
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

	viewMatrix.Identity();	
	viewMatrix.RotateX(-rotation.x);
	viewMatrix.RotateY(-rotation.y);
	viewMatrix.RotateZ(-rotation.z);
	viewMatrix.Translate(-position);	
	
	viewMatrixChanged = false;
	return viewMatrix;
}

void Camera::UpdateFrustum()
{
	if (frustumChanged)
	{
		frustum.Extract(GetViewMatrix(), GetProjectionMatrix());
		frustumChanged = false;
	}	
}

Frustum &Camera::GetFrustum()
{	
	UpdateFrustum();
	return frustum;
}

void Camera::LookAt(const vec3 &target, const vec3 &up)
{
	mat4 t;
	t.LookAt(position, target, up);
	rotation = t.ExtractEulerAngles() * -1.0f;
	viewMatrixChanged = true;
	frustumChanged = true;
}