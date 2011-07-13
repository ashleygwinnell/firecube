#ifndef CAMERA_H
#define CAMERA_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

/**
* A class representing a camera.
*/
class FIRECUBE_API Camera
{
public:
	/**
    * Constructs a camera.    
    */
	Camera();

	/**
	* Sets the positions of this camera.
	* @param position The new position of the camera.
	*/
	void SetPosition(const vec3 &position);

	/**
	* Returns the position of this camera.
	*/
	virtual vec3 GetPosition() const;

	/**
	* Sets the rotation of this camera.
	* @param rotation The new rotation(in Euler angles) of the camera.
	*/
	void SetRotation(const vec3 &rotation);

	/**
	* Returns the rotation of this camera.
	*/
	vec3 GetRotation() const;

	/**
	* Moves the camera.
	* @param translation The translation vector.
	*/
	void Move(const vec3 &translation);

	/**
	* Rotates the camera.
	* @param rotation The rotation vector(in Euler angles).
	*/
	void Rotate(const vec3 &rotation);

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

protected:
	
	/**
	* Calculated the frustum of this camera.
	*/
	void UpdateFrustum();

	/** The position of this camera.*/
	vec3 position;

	/** The rotation of this camera.*/
	vec3 rotation;

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

/**
* A shared pointer to a Camera.
*/
typedef boost::shared_ptr<Camera> CameraPtr;

}
#pragma warning(pop)
#endif