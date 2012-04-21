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
	/**
	* Constructs a frustum.
	*/
	Frustum();

	/**
	* Extracts a frustum for given view matrix and projection matrix.
	* @param viewMatrix The view matrix.
	* @param projectionMatrix The projection matrix.
	*/
	void Extract(const mat4 &viewMatrix, const mat4 &projectionMatrix);

	/**
	* Check whether a bounding box intersects this frustum.
	* @param boundingBox The bounding box to check.
	*/
	bool Contains(const BoundingBox &boundingBox) const;

private:
	Plane rightPlane, leftPlane, bottomPlane, topPlane,farPlane, nearPlane;	
};

}
#pragma warning(pop)
#endif