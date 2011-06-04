#ifndef FRUSTUM_H
#define FRUSTUM_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

// Forward declarations.
class BoundingBox;

/**
* A class representing a frustum.
*/
class FIRECUBE_API Frustum
{
public:
	Frustum();
	void Extract(const mat4 &viewMatrix, const mat4 &projectionMatrix);
	bool Contains(const BoundingBox &boundingBox) const;
private:
	Plane rightPlane, leftPlane, bottomPlane, topPlane,farPlane, nearPlane;	
};

}
#pragma warning(pop)
#endif