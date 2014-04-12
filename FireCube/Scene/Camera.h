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
public:
	/**
	* Constructs a camera.    
	*/
	Camera(Engine *engine);

	/**
	* Sets the projection matrix of this camera.
	* @param mat The new projection matrix.
	*/
	void SetProjectionMatrix(const mat4 &mat);

	/**
	* Returns the projection matrix of this camera.
	*/
	mat4 GetProjectionMatrix() const;

	/**
	* Returns the view matrix of this camera.
	*/
	virtual mat4 GetViewMatrix();

	/**
	* Returns the view frustum of this camera.
	*/
	virtual Frustum &GetFrustum();

	/**
	* Constructs a Look-at camera.
	* @param target The position the camera will be facing.
	* @param up The up vector.
	*/
	void LookAt(const vec3 &target, const vec3 &up);

	virtual void MarkedDirty();
	virtual void NodeChanged();
protected:
	
	/**
	* Calculated the frustum of this camera.
	*/
	void UpdateFrustum();	

	/** a boolean indicating the view matrix has changed.*/
	bool viewMatrixChanged;
	
	/** The view matrix of this camera.*/
	mat4 viewMatrix;
	
	/** The projection matrix of this camera.*/
	mat4 projectionMatrix;
	
	/** The view frustum of this camera.*/
	Frustum frustum;
	
	/** a boolean indicating the view frustum has changed.*/
	bool frustumChanged;	
};

}
#pragma warning(pop)
