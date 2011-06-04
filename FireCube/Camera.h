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
	Camera();
	void SetPosition(const vec3 &position);
	virtual vec3 GetPosition() const;
	void SetRotation(const vec3 &rotation);
	vec3 GetRotation() const;
	void Move(const vec3 &translation);
	void Rotate(const vec3 &rotation);
	void SetProjectionMatrix(const mat4 &mat);
	mat4 GetProjectionMatrix() const;
	virtual mat4 GetViewMatrix();
	virtual Frustum &GetFrustum();
	void LookAt(const vec3 &target, const vec3 &up);
protected:
	void UpdateFrustum();
	vec3 position;
	vec3 rotation;
	bool viewMatrixChanged;
	mat4 viewMatrix;
	mat4 projectionMatrix;
	Frustum frustum;
	bool frustumChanged;
};

/**
* A shared pointer to a CameraResource.
*/
typedef boost::shared_ptr<Camera> CameraPtr;

}
#pragma warning(pop)
#endif