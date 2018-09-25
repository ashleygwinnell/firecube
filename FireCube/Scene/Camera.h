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
	FIRECUBE_OBJECT(Camera)
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
	* Sets the FOV of the camera
	* @param fov The field of view in radians  
	*/
	void SetFOV(float fov);
	
	/**
	* Sets the distance of the near plane
	* @param nearPlnae The distance of the near plane
	*/	
	void SetNearPlane(float nearPlane);
	
	/**
	* Sets the distance of the far plane
	* @param farPlane The distance of the far plane
	*/
	void SetFarPlane(float farPlane);
	
	/**
	* Sets the apspect ratio of the camera
	* @param aspectRatio The apsect ratio
	*/
	void SetAspectRatio(float aspectRatio);
	
	/**
	* Sets the camera to use perspective projection
	* @param fov The field of view
	* @param aspectRation The aspect ratio
	* @param nearPlane The distance of the near plane
	* @param farPlane The distance of the far plane	
	*/
	void SetPerspectiveProjectionParameters(float fov, float aspectRatio, float nearPlane, float farPlane);
	
	/**
	* Sets the camera to use orthographic projection
	* @param leftPlane The distance of the left plane
	* @param rightPlane The distance of the right plane
	* @param bottomPlane The distance of the bottom plane
	* @param topPlane The distance of the top plane
	* @param nearPlane The distance of the near plane
	* @param farPlane The distance of the far plane
	*/
	void SetOrthographicProjectionParameters(float leftPlane, float rightPlane, float bottomPlane, float topPlane, float nearPlane, float farPlane);
	
	/**
	* Un porjects a point from ndc space to camera space
	* @param pos The position in ndc space
	* @returns The unprojected point in camera space 
	*/
	vec3 Unproject(vec3 pos);
	
	/**
	* Returns a picking ray. This is usually used to retrieve a ray pointing from the camera position twards the mousr curser
	* @param pos The position of the point in window space
	* @param width The width of the window
	* @param height The height of the window
	* @returns A ray originating from the camera position tworads the given point 
	*/
	Ray GetPickingRay(vec2 pos, float width, float height);
	
	/**
	* @returns The field of view of the camera
	*/
	float GetFOV() const;
	
	/**
	* @returns The distance of the near plane from the camera
	*/
	float GetNearPlane() const;
	
	/**
	* @returns The distance of the far plane from the camera
	*/
	float GetFarPlane() const;
	
	/**
	* @returns The aspect ratio of the camera
	*/
	float GetApectRatio() const;
	
	/**
	* @returns The distance of the left plane from the camera
	*/
	float GetLeftPlane() const;
	
	/**
	* @returns The distance of the right plane from the camera
	*/
	float GetRightPlane() const;
	
	/**
	* @returns The distance of the top plane from the camera
	*/
	float GetTopPlane() const;
	
	/**
	* @returns The distance of the bottom plane from the camera
	*/
	float GetBottomPlane() const;

	/**
	 * @returns Whether this camera uses an orthographic projection
	 */
	bool IsOrthographic() const;

	/**
	* Renders debug geometry of this camera
	* @param debugRenderer The debug renderer
	*/
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
	
	/**
	* Clones this component
	*/
	virtual Component *Clone() const;
protected:
	
	Camera(const Camera &other);

	/**
	* Calculated the frustum of this camera.
	*/
	void UpdateFrustum();	

	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);

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
