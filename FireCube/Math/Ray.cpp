#include "Math/Ray.h"
#include "MathUtils.h"

using namespace FireCube;

Ray::Ray()
{

}

Ray::Ray(const vec3 &origin, const vec3 &direction) : origin(origin), direction(direction)
{

}

bool Ray::IntersectTriangle(const vec3 &vert0, const vec3 &vert1, const vec3 &vert2, float &u, float &v, float &t) const
{
	vec3 edge1;
	vec3 edge2;
	vec3 tvec;
	vec3 pvec;
	vec3 qvec;
	float inv_det;
	float det;

	edge1 = vert1 - vert0;
	edge2 = vert2 - vert0;


	pvec.Cross(direction, edge2);


	det = edge1.Dot(pvec);

	if ((det > -0.001f) && (det < 0.001f))
	{
		return false;
	}
	inv_det = 1 / det;

	tvec = origin - vert0;

	u = tvec.Dot(pvec) * inv_det;
	if ((u < 0) || (u > 1))
	{
		return false;

	}
	qvec.Cross(tvec, edge1);

	v = direction.Dot(qvec) * inv_det;
	if ((v < 0) || (u + v > 1))
	{
		return false;
	}

	t = edge2.Dot(qvec) * inv_det;

	return true;
}

bool Ray::IntersectBoundingBox(const BoundingBox &boundingBox, float &distance) const
{	
	if (Inside(boundingBox))
	{
		distance = 0;
		return true;
	}
	float tmin, tmax, tymin, tymax, tzmin, tzmax, divx, divy, divz;

	divx = 1 / direction.x;
	if (divx >= 0) 
	{
		tmin = (boundingBox.GetMin().x - origin.x) * divx;
		tmax = (boundingBox.GetMax().x - origin.x) * divx;
	}
	else 
	{		
		tmin = (boundingBox.GetMax().x - origin.x) * divx;
		tmax = (boundingBox.GetMin().x - origin.x) * divx;
	}


	divy = 1 / direction.y;
	if (divy >= 0) 
	{
		tymin = (boundingBox.GetMin().y - origin.y) * divy;
		tymax = (boundingBox.GetMax().y - origin.y) * divy;
	}
	else 
	{		
		tymin = (boundingBox.GetMax().y - origin.y) * divy;
		tymax = (boundingBox.GetMin().y - origin.y) * divy;
	}

	if ( (tmin > tymax) || (tymin > tmax) )
		return false;	
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;

	divz = 1 / direction.z;
	if (divz >= 0) 
	{
		tzmin = (boundingBox.GetMin().z - origin.z) * divz;
		tzmax = (boundingBox.GetMax().z - origin.z) * divz;
	}
	else 
	{		
		tzmin = (boundingBox.GetMax().z - origin.z) * divz;
		tzmax = (boundingBox.GetMin().z - origin.z) * divz;
	}

	if ( (tmin > tzmax) || (tzmin > tmax) )
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	distance = tmin;

	return tmax > 0;
}

bool Ray::IntersectBoundingBox(const BoundingBox &boundingBox, float &distance, vec3 &normal) const
{
	if (Inside(boundingBox))
	{
		distance = 0;
		return true;
	}

	vec3 min = boundingBox.GetMin();
	vec3 max = boundingBox.GetMax();

	float t1 = (min.x - origin.x) / (std::fabs(direction.x) < EPSILON ? EPSILON : direction.x);
	float t2 = (max.x - origin.x) / (std::fabs(direction.x) < EPSILON ? EPSILON : direction.x);
	float t3 = (min.y - origin.y) / (std::fabs(direction.y) < EPSILON ? EPSILON : direction.y);
	float t4 = (max.y - origin.y) / (std::fabs(direction.y) < EPSILON ? EPSILON : direction.y);
	float t5 = (min.z - origin.z) / (std::fabs(direction.z) < EPSILON ? EPSILON : direction.z);
	float t6 = (max.z - origin.z) / (std::fabs(direction.z) < EPSILON ? EPSILON : direction.z);

	float tmin = fmaxf(fmaxf(fminf(t1, t2), fminf(t3, t4)), fminf(t5, t6));
	float tmax = fminf(fminf(fmaxf(t1, t2), fmaxf(t3, t4)), fmaxf(t5, t6));
	
	if (tmax < 0)
	{
		return false;
	}

	if (tmin > tmax)
	{
		return false;
	}

	distance = tmin;

	if (tmin < 0.0f)
	{
		distance = tmax;
	}

	vec3 normals[] = {
		vec3(-1, 0, 0),
		vec3(1, 0, 0),
		vec3(0, -1, 0),
		vec3(0, 1, 0),
		vec3(0, 0, -1),
		vec3(0, 0, 1)
	};
	float t[] = { t1, t2, t3, t4, t5, t6 };

	for (int i = 0; i < 6; ++i)
	{
		if (std::fabs(distance - t[i]) < EPSILON)
		{
			normal = normals[i];
		}
	}

	return true;
}

bool Ray::IntersectMesh(const char *vertexData, unsigned int vertexSize, const char *indexData, unsigned int indicesCount, float &distance, vec3 &normal) const
{
	bool found = false;
	unsigned int *indices = (unsigned int *)indexData;
	for (unsigned int i = 0; i < indicesCount; i += 3)
	{
		const vec3 &v0 = *((const vec3 *) &vertexData[indices[i + 0] * vertexSize]);
		const vec3 &v1 = *((const vec3 *) &vertexData[indices[i + 1] * vertexSize]);
		const vec3 &v2 = *((const vec3 *) &vertexData[indices[i + 2] * vertexSize]);
		float u,v,t;
		vec3 n = Cross(v1 - v0, v2 - v0);
		if (n.Dot(direction) < 0)
		{		
			bool intersect = IntersectTriangle(v0, v1, v2, u, v, t);
			if (intersect && (t > 0) && (!found || t < distance))
			{
				found = true;
				distance = t;
				normal = n;
			}
		}
	}
	if (found)
		normal.Normalize();
	return found;
}

bool Ray::IntersectMesh(const char *vertexData, unsigned int vertexCount, unsigned int vertexSize, float &distance, vec3 &normal) const
{
	bool found = false;
	if (vertexCount % 3 != 0)
		return false;
	for (unsigned int i = 0; i < vertexCount; i += 3)
	{
		const vec3 &v0 = *((const vec3 *)&vertexData[(i + 0) * vertexSize]);
		const vec3 &v1 = *((const vec3 *)&vertexData[(i + 1) * vertexSize]);
		const vec3 &v2 = *((const vec3 *)&vertexData[(i + 2) * vertexSize]);
		float u, v, t;
		vec3 n = Cross(v1 - v0, v2 - v0);
		if (n.Dot(direction) < 0)
		{
			bool intersect = IntersectTriangle(v0, v1, v2, u, v, t);
			if (intersect && (t > 0) && (!found || t < distance))
			{
				found = true;
				distance = t;
				normal = n;
			}
		}
	}
	if (found)
		normal.Normalize();
	return found;
}

Ray Ray::Transformed(const mat4 &transformation) const
{
	Ray result;
	result.origin = transformation * origin;
	result.direction = direction.TransformNormal(transformation);
	return result;
}

bool Ray::Inside(const BoundingBox &boundingBox) const
{
	return origin.x >= boundingBox.GetMin().x && origin.x <= boundingBox.GetMax().x &&
		   origin.y >= boundingBox.GetMin().y && origin.y <= boundingBox.GetMax().y &&
		   origin.z >= boundingBox.GetMin().z && origin.z <= boundingBox.GetMax().z;
}

bool Ray::IntersectPlane(const Plane &plane, float &distance) const
{
	float denom = direction.Dot(plane.GetNormal());
	if (fabs(denom) < 0.0001f)
		return false;

	distance = (plane.GetDistance() - origin.Dot(plane.GetNormal())) / denom;
	return true;
}