#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sstream>
using namespace std;

#include "Utils/utils.h"
#include "Utils/Logger.h"
#include "Math/MyMath.h"
#include "Math/Plane.h"
#include "Math/BoundingBox.h"
#include "Math/Frustum.h"

using namespace FireCube;

Frustum::Frustum()
{
	
}
void Frustum::Extract(const mat4 &viewMatrix, const mat4 &projectionMatrix)
{
	mat4 viewProj = projectionMatrix * viewMatrix;
	float distance, length;
	vec3 normal;
	/* Extract the numbers for the RIGHT plane */
	normal.x = viewProj.m[ 3] - viewProj.m[ 0];
	normal.y = viewProj.m[ 7] - viewProj.m[ 4];
	normal.z = viewProj.m[11] - viewProj.m[ 8];
	distance = viewProj.m[15] - viewProj.m[12];

	length = normal.Length();
	normal /= length;
	distance /= length;
	rightPlane = Plane(normal, distance);

	/* Extract the numbers for the LEFT plane */
	normal.x = viewProj.m[ 3] + viewProj.m[ 0];
	normal.y = viewProj.m[ 7] + viewProj.m[ 4];
	normal.z = viewProj.m[11] + viewProj.m[ 8];
	distance = viewProj.m[15] + viewProj.m[12];

	/* Normalize the result */
	length = normal.Length();
	normal /= length;
	distance /= length;
	leftPlane = Plane(normal, distance);

	/* Extract the BOTTOM plane */
	normal.x = viewProj.m[ 3] + viewProj.m[ 1];
	normal.y = viewProj.m[ 7] + viewProj.m[ 5];
	normal.z = viewProj.m[11] + viewProj.m[ 9];
	distance = viewProj.m[15] + viewProj.m[13];

	/* Normalize the result */
	length = normal.Length();
	normal /= length;
	distance /= length;
	bottomPlane = Plane(normal, distance);

	/* Extract the TOP plane */
	normal.x = viewProj.m[ 3] - viewProj.m[ 1];
	normal.y = viewProj.m[ 7] - viewProj.m[ 5];
	normal.z = viewProj.m[11] - viewProj.m[ 9];
	distance = viewProj.m[15] - viewProj.m[13];

	/* Normalize the result */
	length = normal.Length();
	normal /= length;
	distance /= length;
	topPlane = Plane(normal, distance);


	/* Extract the FAR plane */
	normal.x = viewProj.m[ 3] - viewProj.m[ 2];
	normal.y = viewProj.m[ 7] - viewProj.m[ 6];
	normal.z = viewProj.m[11] - viewProj.m[10];
	distance = viewProj.m[15] - viewProj.m[14];

	/* Normalize the result */
	length = normal.Length();
	normal /= length;
	distance /= length;
	farPlane = Plane(normal, distance);

	/* Extract the NEAR plane */
	normal.x = viewProj.m[ 3] + viewProj.m[ 2];
	normal.y = viewProj.m[ 7] + viewProj.m[ 6];
	normal.z = viewProj.m[11] + viewProj.m[10];
	distance = viewProj.m[15] + viewProj.m[14];

	/* Normalize the result */
	length = normal.Length();
	normal /= length;
	distance /= length;
	nearPlane = Plane(normal, distance);
}
bool Frustum::Contains(const BoundingBox &boundingBox) const
{
	const Plane *planes[6] = {&rightPlane, &leftPlane, &bottomPlane, &topPlane, &farPlane, &nearPlane};
	vec3 min = boundingBox.GetMin();
	vec3 max = boundingBox.GetMax();
	for (unsigned int i = 0; i < 6; i++)
	{
		if (planes[i]->GetDistance(vec3(min.x, min.y, min.z)) > 0.0f) continue;
		if (planes[i]->GetDistance(vec3(max.x, min.y, min.z)) > 0.0f) continue;
		if (planes[i]->GetDistance(vec3(min.x, max.y, min.z)) > 0.0f) continue;
		if (planes[i]->GetDistance(vec3(max.x, max.y, min.z)) > 0.0f) continue;
		if (planes[i]->GetDistance(vec3(min.x, min.y, max.z)) > 0.0f) continue;
		if (planes[i]->GetDistance(vec3(max.x, min.y, max.z)) > 0.0f) continue;
		if (planes[i]->GetDistance(vec3(min.x, max.y, max.z)) > 0.0f) continue;
		if (planes[i]->GetDistance(vec3(max.x, max.y, max.z)) > 0.0f) continue;		
		return false;
	}
	return true;
	
}