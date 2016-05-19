#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>

#include "Utils/utils.h"
#include "Camera.h"
#include "Application/Input.h"

namespace FireCube
{

/**
* A class representing a camera which orbits around a point and handles mouse input for orbiting
*/
class FIRECUBE_API OrbitCamera : public Camera
{
	FIRECUBE_OBJECT(OrbitCamera)
public:
	/**
	* Constructs a NodeObserverCamera.
	*/
	OrbitCamera(Engine *engine);

	/**
	* Constructs a NodeObserverCamera and add default key behavior.
	*/
	OrbitCamera(Engine *engine, InputManager &inputManager);	

	/**
	* Sets the minimum distance from the target.
	* @param v The new minimum distance.
	*/
	void SetMinDistance(float v);

	/**
	* Returns the minimum distance from the target.
	*/
	float GetMinDistance() const;

	/**
	* Sets the maximum distance from the target.
	* @param v The new maximum distance.
	*/
	void SetMaxDistance(float v);

	/**
	* Returns the maximum distance from the target.
	*/
	float GetMaxDistance() const;

	/**
	* Sets the minimum angle of rotation around the x-axis.
	* @param v The new minimum angle.
	*/
	void SetMinAngX(float v);

	/**
	* Returns the minimum angle of rotation around the x-axis.
	*/
	float GetMinAngX() const;

	/**
	* Sets the maximum angle of rotation around the x-axis.
	* @param v The new maximum angle.
	*/
	void SetMaxAngX(float v);

	/**
	* Returns the maximum angle of rotation around the x-axis.
	*/
	float GetMaxAngX() const;

	/**
	* Sets the distance from the target.
	* @param v The new distance.
	*/
	void SetDistance(float v);
	
	/**
	* Returns the distance from the target.
	*/
	float GetDistance() const;	
	
	/**
	* Zooms in/out.
	* @param v The amount to zoom(positive to zoom in, negative to zoom out).
	*/
	void Zoom(float v);

	/**
	* Rotates the camera around the x-axis.
	* @param v The amount to rotate(in radians).
	*/
	void RotateX(float v);

	/**
	* Rotates the camera around the y-axis.
	* @param v The amount to rotate(in radians).
	*/
	void RotateY(float v);	
	
	/**
	* Sets the rotation of the camera
	* @param rotation The new rotation to set (Euler angles)
	*/
	void SetRotation(vec3 rotation);

	/**
	* Sets the zoom factor for the camera. Controls the speed of zooming in / out is
	* @param factor The zoom factor
	*/
	void SetZoomFactor(float factor);

	void HandleInput(float time, const MappedInput &input);

	static void AddInputManagerMappings(InputManager &inputManager);
	
	/**
	* Clones this camera
	*/
	virtual Component *Clone() const;
private:

	OrbitCamera(const OrbitCamera &other);

	static bool addedInputMapping;
	void CheckRanges();
	void UpdateNode();
	vec3 rotation;	
	float distance;
	float minDistance;
	float maxDistance;
	float minAngX;
	float maxAngX;
	float zoomFactor;	
};

}
#pragma warning(pop)
