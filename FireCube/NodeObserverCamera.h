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
	NodeObserverCamera();
	virtual mat4 GetViewMatrix();
	virtual Frustum &GetFrustum();
	void SetTarget(NodePtr node);
	NodePtr GetTarget() const;
	void SetMinZoom(float v);
	float GetMinZoom() const;
	void SetMaxZoom(float v);
	float GetMaxZoom() const;
	void SetMinAngX(float v);
	float GetMinAngX() const;
	void SetMaxAngX(float v);
	float GetMaxAngX() const;
	void SetZoom(float v);
	float GetZoom() const;	
	void Zoom(float v);
	void RotateX(float v);
	void RotateY(float v);
	virtual vec3 GetPosition() const;
private:
	void CheckRanges();
	vec3 lastTargetPosition;
	NodePtr target;
	float zoom;
	float minZoom;
	float maxZoom;
	float minAngX;
	float maxAngX;	
};

/**
* A shared pointer to a NodeObserverCameraResource.
*/
typedef boost::shared_ptr<NodeObserverCamera> NodeObserverCameraPtr;

}
#pragma warning(pop)
#endif