#include "Math/BoundingBox.h"
#include <algorithm>

using namespace FireCube;

BoundingBox::BoundingBox() : bmin((std::numeric_limits<float>::max)()), bmax(-(std::numeric_limits<float>::max)())
{
}

BoundingBox::BoundingBox(const vec3 &min, const vec3 &max)
{
	bmin = min;
	bmax = max;
}

void BoundingBox::Expand(const BoundingBox &bb)
{
	bmin.x = std::min(bmin.x, bb.bmin.x);
	bmin.y = std::min(bmin.y, bb.bmin.y);
	bmin.z = std::min(bmin.z, bb.bmin.z);

	bmax.x = std::max(bmax.x, bb.bmax.x);
	bmax.y = std::max(bmax.y, bb.bmax.y);
	bmax.z = std::max(bmax.z, bb.bmax.z);
}

void BoundingBox::Expand(const vec3 &v)
{
	bmin.x = std::min(bmin.x, v.x);
	bmin.y = std::min(bmin.y, v.y);
	bmin.z = std::min(bmin.z, v.z);

	bmax.x = std::max(bmax.x, v.x);
	bmax.y = std::max(bmax.y, v.y);
	bmax.z = std::max(bmax.z, v.z);
}

float BoundingBox::GetWidth() const
{
	return bmax.x - bmin.x;
}

float BoundingBox::GetHeight() const
{
	return bmax.y - bmin.y;
}

float BoundingBox::GetDepth() const
{
	return bmax.z - bmin.z;
}

vec3 BoundingBox::GetCenter() const
{
	return (bmin + bmax) / 2.0f;
}

bool BoundingBox::Contains(const BoundingBox &bb) const
{
	if (bb.bmin.x >= bmin.x && bb.bmin.y >= bmin.y && bb.bmin.z >= bmin.z && bb.bmax.x <= bmax.x && bb.bmax.y <= bmax.y && bb.bmax.z <= bmax.z)
		return true;
	return false;
}

bool BoundingBox::Intersects(const BoundingBox &bb) const
{
	if (bmax.x < bb.bmin.x ||
		bmax.y < bb.bmin.y ||
		bmax.z < bb.bmin.z ||
		bmin.x > bb.bmax.x ||
		bmin.y > bb.bmax.y ||
		bmin.z > bb.bmax.z)
	{
		return false;
	}
	return true;
}

void BoundingBox::SetMin(const vec3 &min)
{
	bmin = min;
}

vec3 BoundingBox::GetMin() const
{
	return bmin;
}

void BoundingBox::SetMax(const vec3 &max)
{
	bmax = max;
}

vec3 BoundingBox::GetMax() const
{
	return bmax;
}

void BoundingBox::Transform(const mat4 &mat)
{
	// Apply the transformation to the eight corners of the bounding box
	// and create a new one containing all of these
	float w=GetWidth();
	float h=GetHeight();
	float d=GetDepth();
	vec3 v[8]={GetMin(),
		GetMin()+vec3(w,0,0),
		GetMin()+vec3(w,0,d),
		GetMin()+vec3(0,0,d),
		GetMin()+vec3(0,h,0),
		GetMin()+vec3(w,h,0),
		GetMin()+vec3(w,h,d),
		GetMin()+vec3(0,h,d)};
	bmin = bmax = v[0] * mat;
	for (unsigned int i = 1; i < 8; i++)
		Expand(v[i] * mat);
}

vec3 BoundingBox::GetSize() const
{
	return bmax - bmin;
}

bool BoundingBox::Valid() const
{
	return bmin.x <= bmax.x && bmin.y <= bmax.y && bmin.z <= bmax.z;
}
