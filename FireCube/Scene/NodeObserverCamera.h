#ifndef NODE_OBSERVER_CAMERA_H
#define NODE_OBSERVER_CAMERA_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

// Forward declarations.
class Node;
typedef boost::shared_ptr<Node> NodePtr;

/**
* A class representing a node observer camera.
*/
class FIRECUBE_API NodeObserverCamera : public Camera
{
public:
	/**
	* Constructs a NodeObserverCamera.
	*/
	NodeObserverCamera();

	/**
	* Constructs a NodeObserverCamera and add default key behavior.
	*/
	NodeObserverCamera(InputManager &inputManager);

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
	void SetTarget(NodePtr node);

	/**
	* Returns the target node of this camera.
	*/
	NodePtr GetTarget() const;

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

private:
	class NodeObserverCameraInputListener : public InputListener
	{		
	public:
		void SetCamera(NodeObserverCamera *cam);
		void HandleInput(float time, const MappedInput &input);
	private:
		NodeObserverCamera *camera;
	};
	void CheckRanges();
	vec3 lastTargetPosition;
	NodePtr target;
	float distance;
	float minDistance;
	float maxDistance;
	float minAngX;
	float maxAngX;
	NodeObserverCameraInputListener inputListener;
};

/**
* A shared pointer to a NodeObserverCamera.
*/
typedef boost::shared_ptr<NodeObserverCamera> NodeObserverCameraPtr;

}
#pragma warning(pop)
#endif