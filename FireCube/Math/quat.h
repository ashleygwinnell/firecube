#pragma once

#include "Utils/utils.h"
#include "Math/mat3.h"
#include "Math/vec3.h"

namespace FireCube
{

/**
* A quaternion
*/
class FIRECUBE_API quat
{
public:
	quat() {}

	/**
	* Constructs a vector for the specified value.
	*/
	quat(float nx, float ny, float nz, float nw)
	{
		x = nx;
		y = ny;
		z = nz;
		w = nw;
	}

	quat(const mat3 &rotMatrix);

	/** 
	* Construct from euler angles 
	*/
	quat(float rotx, float roty, float rotz);

	/** 
	* Construct from an axis-angle pair 
	*/
	quat(vec3 axis, float angle);
	

	/** Returns a matrix representation of the quaternion */
	mat3 GetMatrix() const;

	bool operator== (const quat &o) const;
	bool operator!= (const quat &o) const;
	vec3 operator*(const vec3 &v) const;

	/**
	* Multiplies two quaternions and assign to this one.
	*/
	void operator*=(const quat &src);

	/** 
	* Normalize the quaternion 
	*/
	void Normalize();

	/**
	* Compute quaternion conjugate 
	*/
	quat Conjugate();

	/** 
	* Rotate a point by this quaternion 
	*/
	vec3 Rotate(const vec3 &in);
	
	/** Performs a spherical interpolation between two quaternions and writes the result into the third.
	* @param pOut Target object to received the interpolated rotation.
	* @param pStart Start rotation of the interpolation at factor == 0.
	* @param pEnd End rotation, factor == 1.
	* @param pFactor Interpolation factor between 0 and 1. Values outside of this range yield undefined results.
	*/
	static void Interpolate(quat &out, const quat &start, const quat &end, float factor);

	void Identity();

	float x, y, w, z;

	/**
	* An identity quaternion.
	*/
	static const quat IDENTITY;
};

/**
* Multiply two quaternions
*/
quat FIRECUBE_API operator*(const quat &a, const quat &b);

}
