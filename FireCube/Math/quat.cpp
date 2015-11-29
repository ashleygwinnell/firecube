#include "Math/Math.h"

using namespace FireCube;
#include <cmath>

const quat quat::IDENTITY(0, 0, 0, 1);

bool quat::operator== (const quat &o) const
{
	return x == o.x && y == o.y && z == o.z && w == o.w;
}

bool quat::operator!= (const quat &o) const
{
	return !(*this == o);
}

quat::quat(const mat3 &rotMatrix)
{
	float t = 1 + rotMatrix.m[0] + rotMatrix.m[4] + rotMatrix.m[8];

	// large enough
	if (t > 0.001f)
	{
		float s = sqrt(t) * 2.0f;
		x = (rotMatrix.m[5] - rotMatrix.m[7]) / s;
		y = (rotMatrix.m[6] - rotMatrix.m[2]) / s;
		z = (rotMatrix.m[1] - rotMatrix.m[3]) / s;
		w = 0.25f * s;
	} // else we have to check several cases
	else if (rotMatrix.m[0] > rotMatrix.m[4] && rotMatrix.m[0] > rotMatrix.m[8])
	{
		// Column 0: 
		float s = sqrt(1.0f + rotMatrix.m[0] - rotMatrix.m[4] - rotMatrix.m[8]) * 2.0f;
		x = 0.25f * s;
		y = (rotMatrix.m[1] + rotMatrix.m[3]) / s;
		z = (rotMatrix.m[6] + rotMatrix.m[2]) / s;
		w = (rotMatrix.m[5] - rotMatrix.m[7]) / s;
	}
	else if (rotMatrix.m[4] > rotMatrix.m[8])
	{
		// Column 1: 
		float s = sqrt(1.0f + rotMatrix.m[4] - rotMatrix.m[0] - rotMatrix.m[8]) * 2.0f;
		x = (rotMatrix.m[1] + rotMatrix.m[3]) / s;
		y = 0.25f * s;
		z = (rotMatrix.m[5] + rotMatrix.m[7]) / s;
		w = (rotMatrix.m[6] - rotMatrix.m[2]) / s;
	}
	else
	{
		// Column 2:
		float s = sqrt(1.0f + rotMatrix.m[8] - rotMatrix.m[0] - rotMatrix.m[4]) * 2.0f;
		x = (rotMatrix.m[6] + rotMatrix.m[2]) / s;
		y = (rotMatrix.m[5] + rotMatrix.m[7]) / s;
		z = 0.25f * s;
		w = (rotMatrix.m[1] - rotMatrix.m[3]) / s;
	}
}

quat::quat(float rotx, float roty, float rotz)
{
	float c1 = std::cos(rotx * 0.5f);
	float c2 = std::cos(roty * 0.5f);
	float c3 = std::cos(rotz * 0.5f);
	float s1 = std::sin(rotx * 0.5f);
	float s2 = std::sin(roty * 0.5f);
	float s3 = std::sin(rotz * 0.5f);
	x = s1 * c2 * c3 + c1 * s2 * s3;
	y = c1 * s2 * c3 - s1 * c2 * s3;
	z = c1 * c2 * s3 + s1 * s2 * c3;
	w = c1 * c2 * c3 - s1 * s2 * s3;
}

mat3 quat::GetMatrix() const
{
	mat3 resMatrix;
	resMatrix.m[0] = 1.0f - 2.0f * (y * y + z * z);
	resMatrix.m[3] = 2.0f * (x * y - z * w);
	resMatrix.m[6] = 2.0f * (x * z + y * w);
	resMatrix.m[1] = 2.0f * (x * y + z * w);
	resMatrix.m[4] = 1.0f - 2.0f * (x * x + z * z);
	resMatrix.m[7] = 2.0f * (y * z - x * w);
	resMatrix.m[2] = 2.0f * (x * z - y * w);
	resMatrix.m[5] = 2.0f * (y * z + x * w);
	resMatrix.m[8] = 1.0f - 2.0f * (x * x + y * y);

	return resMatrix;
}

quat::quat(vec3 axis, float angle)
{
	axis.Normalize();

	const float sin_a = sin(angle / 2);
	const float cos_a = cos(angle / 2);
	x = axis.x * sin_a;
	y = axis.y * sin_a;
	z = axis.z * sin_a;
	w = cos_a;
}


void quat::Interpolate(quat &out, const quat &start, const quat &end, float factor)
{
	// calc cosine theta
	float cosom = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;

	// adjust signs (if necessary)
	quat lEnd;
	if (cosom < 0.0f)
	{
		cosom = -cosom;
		lEnd.x = -end.x;   // Reverse all signs
		lEnd.y = -end.y;
		lEnd.z = -end.z;
		lEnd.w = -end.w;
	}
	else
	{
		lEnd = end;
	}

	// Calculate coefficients
	float sclp, sclq;
	if ((1.0f - cosom) > 0.0001f) // 0.0001 -> some epsillon
	{
		// Standard case (slerp)
		float omega, sinom;
		omega = acos(cosom); // extract theta from dot product's cos theta
		sinom = sin(omega);
		sclp = sin((1.0f - factor) * omega) / sinom;
		sclq = sin(factor * omega) / sinom;
	}
	else
	{
		// Very close, do linear interp (because it's faster)
		sclp = 1.0f - factor;
		sclq = factor;
	}

	out.x = sclp * start.x + sclq * lEnd.x;
	out.y = sclp * start.y + sclq * lEnd.y;
	out.z = sclp * start.z + sclq * lEnd.z;
	out.w = sclp * start.w + sclq * lEnd.w;
}

void quat::Normalize()
{
	// compute the magnitude and divide through it
	const float mag = sqrt(x * x + y * y + z * z + w * w);
	if (mag)
	{
		const float invMag = 1.0f / mag;
		x *= invMag;
		y *= invMag;
		z *= invMag;
		w *= invMag;
	}	
}

quat FireCube::operator*(const quat &a, const quat &b)
{
	return quat(a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y,
		a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z,
		a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x,
		a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z);
}

quat quat::Conjugate()
{
	quat ret;
	ret.x = -x;
	ret.y = -y;
	ret.z = -z;
	ret.w = w;
	return ret;
}

vec3 quat::Rotate(const vec3 &v)
{
	quat q2(0.0f, v.x, v.y, v.z), q = this->Conjugate(), qinv = q;	

	q = q * q2 * qinv;
	return vec3(q.x, q.y, q.z);

}

vec3 quat::operator*(const vec3 &v) const
{
	vec3 ret;	
	
	// q*v
	float ix = w * v.x + y * v.z - z * v.y,
		iy = w * v.y + z * v.x - x * v.z,
		iz = w * v.z + x * v.y - y * v.x,
		iw = -x * v.x - y * v.y - z * v.z;

	ret.x = ix * w + iw * -x + iy * -z - iz * -y;
	ret.y = iy * w + iw * -y + iz * -x - ix * -z;
	ret.z = iz * w + iw * -z + ix * -y - iy * -x;

	return ret;
}

void quat::Identity()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

void quat::operator*=(const quat &b)
{
	quat a = *this;
	x = a.x * b.w + a.w * b.x + a.y * b.z - a.z * b.y;
	y = a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z;
	z = a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x;
	w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
}