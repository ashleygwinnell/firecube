#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>

#include "Math/Frustum.h"
#include "Math/Math.h"
#include "Math/Ray.h"
#include "Core/Component.h"

namespace FireCube
{

/**
* A class representing a camera.
*/
class FIRECUBE_API Camera : public Component
{
	OBJECT(Camera)
public:
	/**
	* Constructs a camera.    
	*/
	Camera(Engine *engine);

	/**
	* Returns the projection matrix of this camera.
	*/
	mat4 GetProjectionMatrix();

	/**
	* Returns the view matrix of this camera.
	*/
	mat4 GetViewMatrix();

	/**
	* Returns the view frustum of this camera.
	*/
	Frustum &GetFrustum();	

	void SetFOV(float fov);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	void SetAspectRatio(float aspectRatio);
	void SetPerspectiveProjectionParameters(float fov, float aspectRatio, float nearPlane, float farPlane);
	void SetOrthographicProjectionParameters(float leftPlane, float rightPlane, float bottomPlane, float topPlane, float nearPlane, float farPlane);
	vec3 Unproject(vec3 pos);
	Ray GetPickingRay(vec2 pos, float width, float height);
	float GetFOV() const;
	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetApectRatio() const;

	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);
	virtual Component *Clone() const;
protected:
	
	Camera(const Camera &other);

	/**
	* Calculated the frustum of this camera.
	*/
	void UpdateFrustum();	

	/** a boolean indicating the view matrix has changed.*/
	bool viewMatrixChanged;

	bool projectionMatrixChanged;
	
	/** The view matrix of this camera.*/
	mat4 viewMatrix;
	
	/** The projection matrix of this camera.*/
	mat4 projectionMatrix;
	
	/** The view frustum of this camera.*/
	Frustum frustum;
	
	float fov;
	float nearPlane;
	float farPlane;
	float aspectRatio;

	float leftPlane;
	float rightPlane;
	float topPlane;
	float bottomPlane;

	/** a boolean indicating the view frustum has changed.*/
	bool frustumChanged;	
	
	bool orthographic;
};

}
#pragma warning(pop)
