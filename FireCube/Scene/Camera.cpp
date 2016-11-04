#include "Scene/Camera.h"
#include "Scene/Node.h"
#include "Rendering/DebugRenderer.h"

using namespace FireCube;

Camera::Camera(Engine *engine) : Component(engine), viewMatrixChanged(true), frustumChanged(true), projectionMatrixChanged(true), aspectRatio(1.0f), fov(60.0f), 
								 nearPlane(0.1f), farPlane(200.0f), orthographic(false)
{

}

Camera::Camera(const Camera &other) : Component(other), viewMatrixChanged(true), frustumChanged(true), projectionMatrixChanged(true), aspectRatio(other.aspectRatio), fov(other.fov),
									  nearPlane(other.nearPlane), farPlane(other.farPlane), leftPlane(other.leftPlane), rightPlane(other.rightPlane), topPlane(other.topPlane), 
									  bottomPlane(other.bottomPlane), orthographic(other.orthographic)
{

}

mat4 Camera::GetProjectionMatrix()
{
	if (projectionMatrixChanged)
	{
		projectionMatrixChanged = false;
		if (orthographic)
		{
			if (aspectRatio >= 1.0f)
			{
				projectionMatrix.GenerateOrthographic(leftPlane * aspectRatio, rightPlane * aspectRatio, bottomPlane, topPlane, nearPlane, farPlane);
			}
			else
			{
				projectionMatrix.GenerateOrthographic(leftPlane, rightPlane, bottomPlane / aspectRatio, topPlane / aspectRatio, nearPlane, farPlane);
			}
		}
		else
		{
			projectionMatrix.GeneratePerspective(fov, aspectRatio, nearPlane, farPlane);
		}
		
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

void Camera::SceneChanged(Scene *oldScene)
{

}

void Camera::SetFOV(float fov)
{
	if (this->fov != fov)
	{
		this->fov = fov;
		if (orthographic == false)
		{
			projectionMatrixChanged = true;
			frustumChanged = true;
		}
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
	orthographic = false;
	this->fov = fov;
	this->aspectRatio = aspectRatio;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;	
	projectionMatrixChanged = true;
	frustumChanged = true;
}

void Camera::SetOrthographicProjectionParameters(float leftPlane, float rightPlane, float bottomPlane, float topPlane, float nearPlane, float farPlane)
{
	orthographic = true;
	this->leftPlane = leftPlane;
	this->rightPlane = rightPlane;
	this->topPlane = topPlane;
	this->bottomPlane = bottomPlane;
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

float Camera::GetFOV() const
{
	return fov;
}

float Camera::GetNearPlane() const
{
	return nearPlane;
}

float Camera::GetFarPlane() const
{
	return farPlane;
}

float Camera::GetApectRatio() const
{
	return aspectRatio;
}

Component *Camera::Clone() const
{
	Camera *clone = new Camera(*this);
	return clone;
}

float Camera::GetLeftPlane() const
{
	return leftPlane;
}

float Camera::GetRightPlane() const
{
	return rightPlane;
}

float Camera::GetTopPlane() const
{
	return topPlane;
}

float Camera::GetBottomPlane() const
{
	return bottomPlane;
}

void Camera::RenderDebugGeometry(DebugRenderer *debugRenderer)
{
	Frustum frustum;
	frustum.Extract(GetViewMatrix(), GetProjectionMatrix());
	debugRenderer->AddFrustum(frustum, vec3(0, 1, 0));
}
