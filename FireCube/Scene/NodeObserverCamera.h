#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <memory>

#include "Utils/utils.h"
#include "Camera.h"
#include "Application/Input.h"

namespace FireCube
{

// Forward declarations.
class Node;

/**
* A class representing a node observer camera.
*/
class FIRECUBE_API NodeObserverCamera : public Camera, public InputListener
{
public:
	/**
	* Constructs a NodeObserverCamera.
	*/
	NodeObserverCamera(Engine *engine);

	/**
	* Constructs a NodeObserverCamera and add default key behavior.
	*/
	NodeObserverCamera(Engine *engine, InputManager &inputManager);

	/**
	* Returns the view matrix of this camera.
	*/
	virtual mat4 GetViewMatrix();

	/**
	* Returns the view frustum of this camera.
	*/
	virtual Frustum &GetFrustum();

	/**
	* Sets the target node of this camera.
	* @param node The target node.
	*/
	void SetTarget(Node *node);

	/**
	* Returns the target node of this camera.
	*/
	Node *GetTarget() const;

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
	* Returns the position of this camera.
	*/
	virtual vec3 GetPosition() const;

	void SetZoomFactor(float factor);

	void HandleInput(float time, const MappedInput &input);

	void RegisterWithInputManager(InputManager &inputManager);

private:
	
	void CheckRanges();
	vec3 lastTargetPosition;
	Node *target;
	vec3 rotation;
	vec3 position;
	float distance;
	float minDistance;
	float maxDistance;
	float minAngX;
	float maxAngX;
	float zoomFactor;	
};

}
#pragma warning(pop)
