#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>

#include "Math/Frustum.h"
#include "Math/Math.h"
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

	/**
	* Constructs a Look-at camera.
	* @param target The position the camera will be facing.
	* @param up The up vector.
	*/
	void LookAt(const vec3 &target, const vec3 &up);

	void SetFOV(float fov);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	void SetAspectRatio(float aspectRatio);
	void SetPerspectiveProjectionParameters(float fov, float aspectRatio, float nearPlane, float farPlane);

	virtual void MarkedDirty();
	virtual void NodeChanged();
protected:
	
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

	/** a boolean indicating the view frustum has changed.*/
	bool frustumChanged;	
};

}
#pragma warning(pop)
